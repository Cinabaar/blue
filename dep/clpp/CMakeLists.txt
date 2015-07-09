include_directories(${OpenCL_INCLUDE_DIRS})

SET(CLPP_SOURCES
  clppContext.cpp
  clppCount.cpp
  clpp.cpp
  clppProgram.cpp
  clppScan_Default.cpp
  clppScan_GPU.cpp
  clppSort_BitonicSort.cpp
  clppSort_BitonicSortGPU.cpp
  clppSort.cpp
  clppSort_CPU.cpp
  clppSort_RadixSort.cpp
  clppSort_RadixSortGPU.cpp
  StopWatch.cpp
  )

SET(CLPP_HEADERS
  clppContext.h
  clppCount.h
  clpp.h
  clppProgram.h
  clppScan_Default_CLKernel.h
  clppScan_Default.h
  clppScan_GPU_CLKernel.h
  clppScan_GPU.h
  clppScan.h
  clppSort_BitonicSort_CLKernel.h
  clppSort_BitonicSortGPU_CLKernel.h
  clppSort_BitonicSortGPU.h
  clppSort_BitonicSort.h
  clppSort_CPU.h
  clppSort.h
  clppSort_RadixSort_CLKernel.h
  clppSort_RadixSortGPU_CLKernel.h
  clppSort_RadixSortGPU.h
  clppSort_RadixSort.h
  StopWatch.h
  )

SET(CLPP_KERNELS
  clppCount.cl
  clppScan_Default.cl
  clppScan_GPU.cl
  clppSort_BitonicSort.cl
  clppSort_BitonicSortGPU.cl
  clppSort_RadixSort.cl
  clppSort_RadixSortGPU.cl

  CACHE STRING "clpp_kernels"
  )

ADD_LIBRARY(clpp SHARED ${CLPP_SOURCES})

TARGET_LINK_LIBRARIES(clpp ${OpenCL_LIBRARIES})
