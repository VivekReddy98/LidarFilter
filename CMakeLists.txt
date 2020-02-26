cmake_minimum_required(VERSION 2.6)

project(LidarFilters)

enable_language(CXX)

# Check the GNU Compiler Version Compatibilty for OpenMP support.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND ENABLE_OPENMP)
  # using Clang
  # required atleast version 6.1 (OpenMP 4.5 Support)
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.1)
        message(WARNING "This Version of GCC Doesn't Support OpenMP 4.5")
  endif()
endif()

# If Enable OPENMP provided by the user, set these values
if (ENABLE_OPENMP)
    set(OMP_NUM_THREADS 4)
    set(CMAKE_CXX_FLAGS "-fopenmp")
endif()

# set the corresponding executables and flags depending on the mode specified by the user
# IN Debug mode, gdb flags are set and also a different main file is compiled.
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
   set(CMAKE_CXX_FLAGS "-g ")
   add_executable(LSFtest.exe src/testMain.cpp src/karri-vivek/karri-vivek.cpp)
else()
   set(CMAKE_CXX_FLAGS "-g ")
   add_executable(LSF.exe src/main.cpp src/karri-vivek/karri-vivek.cpp)
endif()
