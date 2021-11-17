set(SRCS
antenna/antenna.cpp
antenna/EdgeMethodCostFunc.cpp
antenna/ElNullRangeEst.cpp
antenna/ElPatternEst.cpp
antenna/Frame.cpp
antenna/geometryfunc.cpp
antenna/SphGridType.cpp
container/container.cpp
container/RadarGeometry.cpp
core/core.cpp
core/Attitude.cpp
core/Basis.cpp
core/Constants.cpp
core/DateTime.cpp
core/Ellipsoid.cpp
core/EulerAngles.cpp
core/Interp1d.cpp
core/Kernels.cpp
core/Linspace.cpp
core/LookSide.cpp
core/LUT1d.cpp
core/LUT2d.cpp
core/avgLUT2dToLUT1d.cpp
core/Orbit.cpp
core/Projections.cpp
core/Quaternion.cpp
core/StateVector.cpp
core/TimeDelta.cpp
core/Poly1d.cpp
core/Poly2d.cpp
focus/Backproject.cpp
focus/Chirp.cpp
focus/DryTroposphereModel.cpp
focus/focus.cpp
focus/Presum.cpp
focus/RangeComp.cpp
geocode/geocode.cpp
geocode/GeocodeSlc.cpp
geometry/boundingbox.cpp
geometry/DEMInterpolator.cpp
geocode/GeocodeCov.cpp
geocode/GeocodePolygon.cpp
geometry/geometry.cpp
geometry/geo2rdr.cpp
geometry/rdr2geo.cpp
geometry/RTC.cpp
geometry/metadataCubes.cpp
geometry/ltpcoordinates.cpp
geometry/pntintersect.cpp
geometry/lookIncFromSr.cpp
image/image.cpp
image/ResampSlc.cpp
io/gdal/Dataset.cpp
io/gdal/GDALAccess.cpp
io/gdal/GDALDataType.cpp
io/gdal/gdal.cpp
io/gdal/Raster.cpp
io/Raster.cpp
io/serialization.cpp
io/io.cpp
polsar/symmetrize.cpp
polsar/polsar.cpp
signal/signal.cpp
signal/convolve2D.cpp
signal/Covariance.cpp
signal/Crossmul.cpp
signal/CrossMultiply.cpp
signal/flatten.cpp
signal/filter2D.cpp
product/GeoGridParameters.cpp
product/product.cpp
product/RadarGridParameters.cpp
product/Swath.cpp
unwrap/unwrap.cpp
unwrap/ICU.cpp
unwrap/Phass.cpp
isce.cpp
)

if(WITH_CUDA)
    list(APPEND SRCS
         cuda/cuda.cpp
         cuda/core/core.cpp
         cuda/core/ComputeCapability.cpp
         cuda/core/Device.cpp
         cuda/geocode/geocode.cpp
         cuda/geocode/cuGeocode.cpp
         cuda/geometry/geometry.cpp
         cuda/geometry/geo2rdr.cpp
         cuda/geometry/rdr2geo.cpp
         cuda/focus/Backproject.cpp
         cuda/focus/focus.cpp
         cuda/image/image.cpp
         cuda/image/ResampSlc.cpp
         cuda/matchtemplate/matchtemplate.cpp
         cuda/matchtemplate/pycuampcor.cpp
         cuda/signal/signal.cpp
         cuda/signal/Crossmul.cpp
         )
endif()
