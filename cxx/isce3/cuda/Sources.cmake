set(SOURCES
core/Device.cu
core/gpuBicubicInterpolator.cu
core/gpuBilinearInterpolator.cu
core/gpuLUT1d.cu
core/gpuLUT2d.cu
core/gpuNearestNeighborInterpolator.cu
core/gpuPoly2d.cu
core/gpuProjections.cu
core/gpuSinc2dInterpolator.cu
core/gpuSpline2dInterpolator.cu
core/InterpolatorHandle.cu
core/Orbit.cu
core/ProjectionBaseHandle.cu
except/Error.cpp
focus/Backproject.cu
fft/detail/CufftWrapper.cu
geocode/Geocode.cu
geocode/MaskedMinMax.cu
geometry/Geo2rdr.cpp
geometry/gpuDEMInterpolator.cu
geometry/gpuGeo2rdr.cu
geometry/gpuGeometry.cu
geometry/gpuRTC.cu
geometry/gpuTopo.cu
geometry/gpuTopoLayers.cpp
geometry/Topo.cpp
geometry/utilities.cu
image/gpuResampSlc.cu
image/ResampSlc.cpp
matchtemplate/pycuampcor/cuAmpcorChunk.cu
matchtemplate/pycuampcor/cuAmpcorController.cu
matchtemplate/pycuampcor/cuAmpcorParameter.cu
matchtemplate/pycuampcor/cuArrays.cu
matchtemplate/pycuampcor/cuArraysCopy.cu
matchtemplate/pycuampcor/cuArraysPadding.cu
matchtemplate/pycuampcor/cuCorrFrequency.cu
matchtemplate/pycuampcor/cuCorrNormalization.cu
matchtemplate/pycuampcor/cuCorrNormalizationSAT.cu
matchtemplate/pycuampcor/cuCorrNormalizer.cu
matchtemplate/pycuampcor/cuCorrTimeDomain.cu
matchtemplate/pycuampcor/cuDeramp.cu
matchtemplate/pycuampcor/cuEstimateStats.cu
matchtemplate/pycuampcor/cuOffset.cu
matchtemplate/pycuampcor/cuOverSampler.cu
matchtemplate/pycuampcor/cuSincOverSampler.cu
matchtemplate/pycuampcor/GDALImage.cu
product/SubSwaths.cu
signal/gpuAzimuthFilter.cu
signal/gpuCrossMul.cu
signal/gpuFilter.cu
signal/gpuLooks.cu
signal/gpuRangeFilter.cu
signal/gpuSignal.cu
)
