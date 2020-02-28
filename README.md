# LidarFilter
Implemented a Range Filter and a Temporal Median Filter operated upon on Lidar Scan Sequence Data in C++, using STL and OpenMP Libraries.

## Instructions to Build the project
1) Assuming you are in the root folder of the project i.e. LidarFilter/ and on a unix machine
2) mkdir Build
3) cd build
4) Now you can choose one of the four build configurations (Release/Debug Mode) and (Enabling/Not-Enabling OpenMP)
5) After sucessfully executing one of the build configuration, there must be a Makefile prsent in the build/ folder.
6) Run make to generate the executable.

## Build Configurations
**Release W/O OpenMP** <br/>
      
       cmake -D CMAKE_CXX_COMPILER="/usr/bin/g++" -DCMAKE_BUILD_TYPE="Release"  ../            
**Release With OpenMP** <br/>
    
       cmake -D CMAKE_CXX_COMPILER="/usr/bin/g++" -DCMAKE_BUILD_TYPE="Release" -DENABLE_OPENMP=On ../ 
**Debug W/O OpenMP** <br/>
     
       cmake -D CMAKE_CXX_COMPILER="/usr/bin/g++" -DCMAKE_BUILD_TYPE="Debug"  ../
**Debug with OpenMP** <br/>

       cmake -D CMAKE_CXX_COMPILER="/usr/bin/g++" -DCMAKE_BUILD_TYPE="Debug" -DENABLE_OPENMP=On ../ 
 
 **Note** <br/>
1) Please specify your g++ compiler path in the CMAKE_CXX_COMPILER flag

## Execting the proram
1) If you have build using Release mode, use:
        
        ./LSF.exe {rf} {timeSteps} {N} {MAX} {MIN} < example.txt  or
        
        ./LSF.exe {mf} {timeSteps} {N} {D} < example.txt
        
        Eg: ./LSF.exe mf 5 5 2 < example.txt

2) If you have build using Debug mode, use:
        
        ./LSFtest.exe < ../data/F_7_3.txt

## Note:
1) Run make clean before executing any other build.


## Folder Structure:
```
src/
├── main.cpp
├── testMain.cpp
├── karri-vivek              
    ├── karri-vivek.cpp (Filter Class Defintions)        
    |── karri-vivek.h (Declaration of filter Classes)
data/ (Test Data for executable generated from Debug Mode)
```
