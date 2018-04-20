// -*- C++ -*-
// -*- coding: utf-8 -*-
//
// Author: Bryan V. Riel, Joshua Cohen
// Copyright 2017-2018

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <future>
#include <omp.h>
#include <valarray>
#include <algorithm>

// pyre
//#include <pyre/timers.h>

// isce::core
#include <isce/core/Constants.h>
#include <isce/core/LinAlg.h>

// isce::geometry
#include "Geo2rdr.h"

// pull in some isce::core namespaces
using isce::core::Raster;
using isce::core::Poly2d;
using isce::core::LinAlg;

// Run geo2rdr with no offsets
void isce::geometry::Geo2rdr::
geo2rdr(isce::core::Raster & latRaster,
        isce::core::Raster & lonRaster,
        isce::core::Raster & hgtRaster,
        isce::core::Poly2d & doppler,
        const std::string & outdir) {
    geo2rdr(latRaster, lonRaster, hgtRaster, doppler, outdir, 0.0, 0.0);
}

// Run geo2rdr - main entrypoint
void isce::geometry::Geo2rdr::
geo2rdr(isce::core::Raster & latRaster,
        isce::core::Raster & lonRaster,
        isce::core::Raster & hgtRaster,
        isce::core::Poly2d & doppler,
        const std::string & outdir,
        double azshift, double rgshift) {

    // Create reusable pyre::journal channels
    pyre::journal::warning_t warning("isce.geometry.Geo2rdr");
    pyre::journal::info_t info("isce.geometry.Geo2rdr");

    // Cache the size of the DEM images
    const size_t demWidth = hgtRaster.width();
    const size_t demLength = hgtRaster.length();
    const double rad = M_PI / 180.0;

    // Valarrays to hold line of results
    std::valarray<double> lat(demWidth), lon(demWidth), hgt(demWidth);
    std::valarray<float> rgoff(demWidth), azoff(demWidth);

    // Create output rasters
    Raster rgoffRaster = Raster(outdir + "/range.off", demWidth, demLength, 1,
        GDT_Float32, "ENVI");
    Raster azoffRaster = Raster(outdir + "/azimuth.off", demWidth, demLength, 1,
        GDT_Float32, "ENVI");
    
    // Cache sensing start
    double t0 = _meta.sensingStart.secondsSinceEpoch(_refEpoch);
    // Adjust for const azimuth shift
    t0 -= (azshift - 0.5 * (_meta.numberAzimuthLooks - 1)) / _meta.prf;

    // Cache starting range
    double r0 = _meta.rangeFirstSample;
    // Adjust for constant range shift
    r0 -= (rgshift - 0.5 * (_meta.numberRangeLooks - 1)) * _meta.slantRangePixelSpacing;

    // Compute azimuth time extents
    double dtaz = _meta.numberAzimuthLooks / _meta.prf;
    const double tend = t0 + ((_meta.length - 1) * dtaz);
    const double tmid = 0.5 * (t0 + tend);

    // Compute range extents
    const double dmrg = _meta.numberRangeLooks * _meta.slantRangePixelSpacing;
    const double rngend = r0 + ((_meta.width - 1) * dmrg);

    // Print out extents
    _printExtents(info, t0, tend, dtaz, r0, rngend, dmrg, demWidth, demLength);

    // Interpolate orbit to middle of the scene as a test
    _checkOrbitInterpolation(tmid);

    // Loop over DEM lines
    int converged = 0;
    for (size_t line = 0; line < demLength; ++line) {

        // Read line of data
        latRaster.getLine(lat, line);
        lonRaster.getLine(lon, line);
        hgtRaster.getLine(hgt, line);

        // Loop over DEM pixels
        for (size_t pixel = 0; pixel < demWidth; ++pixel) {

            // Perform geo->rdr iterations
            cartesian_t llh = {lat[pixel]*rad, lon[pixel]*rad, hgt[pixel]};
            double aztime, slantRange;
            int geostat = Geometry::geo2rdr(llh, _ellipsoid, _orbit, doppler,
                _meta, aztime, slantRange, _threshold, _numiter, 1.0e-8);

            // Check of solution is out of bounds
            bool isOutside = false;
            if ((aztime < t0) || (aztime > tend))
                isOutside = true;
            if ((slantRange < r0) || (slantRange > rngend))
                isOutside = true;
            
            // Save result if valid
            if (!isOutside) {
                rgoff[pixel] = ((slantRange - r0) / dmrg) - float(pixel);
                azoff[pixel] = ((aztime - t0) / dtaz) - float(line);
                converged += geostat;
            } else {
                rgoff[pixel] = NULL_VALUE;
                azoff[pixel] = NULL_VALUE;
            }
        }

        // Write data
        rgoffRaster.setLine(rgoff, line);
        azoffRaster.setLine(azoff, line);
    }
            
    // Print out convergence statistics
    info << "Total convergence: " << converged << " out of "
         << (demWidth * demLength) << pyre::journal::endl;

}

// Print extents and image sizes
void isce::geometry::Geo2rdr::
_printExtents(pyre::journal::info_t & info, double t0, double tend, double dtaz,
              double r0, double rngend, double dmrg, size_t demWidth, size_t demLength) {
    info << pyre::journal::newline
         << "Starting acquisition time: " << t0 << pyre::journal::newline
         << "Stop acquisition time: " << tend << pyre::journal::newline
         << "Azimuth line spacing in seconds: " << dtaz << pyre::journal::newline
         << "Near range (m): " << r0 << pyre::journal::newline
         << "Far range (m): " << rngend << pyre::journal::newline
         << "Radar image length: " << _meta.length << pyre::journal::newline
         << "Radar image width: " << _meta.width << pyre::journal::newline
         << "Geocoded lines: " << demLength << pyre::journal::newline
         << "Geocoded samples: " << demWidth << pyre::journal::newline;
}

// Check we can interpolate orbit to middle of DEM
void isce::geometry::Geo2rdr::
_checkOrbitInterpolation(double aztime) {
    cartesian_t satxyz, satvel;
    int stat = _orbit.interpolate(aztime, satxyz, satvel, _orbitMethod);
    if (stat != 0) {
        pyre::journal::error_t error("isce.core.Geo2rdr");
        error
            << pyre::journal::at(__HERE__)
            << "Error in Topo::topo - Error getting state vector for bounds computation."
            << pyre::journal::newline
            << " - requested time: " << aztime << pyre::journal::newline
            << " - bounds: " << _orbit.UTCtime[0] << " -> " << _orbit.UTCtime[_orbit.nVectors-1]
            << pyre::journal::endl;
    }
}

// end of file
