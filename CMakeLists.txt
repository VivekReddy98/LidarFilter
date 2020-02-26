cmake_minimum_required(VERSION 2.6)

project(LidarFilters)

enable_language(CXX)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(ENV{__DEBUG} 1)

else()
    set(default_build_type "Release")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND ENABLE_OPENMP)
  # using Clang
  # required atleast version 6.1 (OpenMP 4.5 Support)
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.1)
        message(WARNING "This Version of GCC Doesn't Support OpenMP 4.5 and the Code is not tested for Older Verisions")
  endif()
endif()

if (ENABLE_OPENMP)
    set(OMP_NUM_THREADS 4)
    set(CMAKE_CXX_FLAGS "-fopenmp")
endif()

set(CMAKE_CXX_FLAGS "-g ")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
   add_executable(tests.exe src/karri-vivek/karri-vivek.cpp test/main.cpp )
else()
   add_executable(main.exe src/main.cpp src/karri-vivek/karri-vivek.cpp)
endif()
