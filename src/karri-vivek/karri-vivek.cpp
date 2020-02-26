#include <iostream>
#include <queue>
#include <cmath>
#include <algorithm>
#include "karri-vivek.h"


/** RangeFilter Constructor:
          Purpose: Input checking and variable initialization
             Args: Max and Min Thresholds as floats
*/
LSF::RangeFilter::RangeFilter(float max, float min){
    if (max < min) {
      throw std::runtime_error("Maximum is less than Minumum while initializing the Range Filter");
    }
    range_max = max;
    range_min = min;
}

/** RangeFilter Update Method:
          Purpose: To filter the inout sequence of the array by clipping the max and min thresholds
             Args: Input Sequence as a float vector
          Working: Loops over all the values of the sequences and clip the residuals.
          Nuances: OpenMP will Paralellize the execution among four threads
                   and therefore has performance benefits for larger arrays
*/
void LSF::RangeFilter::update(std::vector<float>& input_scan){
     // auto start = chrono::steady_clock::now();
     int i;
     #pragma omp parallel for default(shared) private(i)
     for (i = 0; i < input_scan.size(); i++){
          if (input_scan[i] > range_max) {
              input_scan[i] = range_max;
          }
          else if (input_scan[i] < range_min){
              input_scan[i] = range_min;
          }
     }
}

/** MedianFilter Constructor:
        Purpose: Input checking and variable initialization
           Args: D: The Number of Previous Scans, the filter is dependent upon
                 N: Length of the Sequences.
        Working: Dval is set D+1, because the matrix has to have a size of (D+1)*(N).
*/
LSF::MedianFilter::MedianFilter(int D, int N){
    if (D < 0){
      throw std::runtime_error("D Must be >= 0");
    }
    Dval = D+1;
    currentTimeStamp = 0;
    rangeMeasure = N;
}

/** MedianFilter Destructor:
        Purpose: Freeing Up the Heap allocated Memory before dying
           Args: None
        Working: freeCheck gives the count of the allocated elements.
                 Using that information, the memory is freed up sequentially.
*/
LSF::MedianFilter::~MedianFilter(){
    int limit = freeCheck.size();
    for(int i = 0; i<limit; i++){
        free(Db[i]);
    }
}

/** MedianFilter Update Method:
        Purpose: Update the Input Sequence Based on Definition imposed by Temporal Median Filter
           Args: Input Scan Sequence
        Working: If currentTimeStamp = 0, intialization code is run to set up the matrix and necessary variables.
                 (For More Info, look at initSetUp() function)
                 Then store the newscan sequence in the already defined matrix at the last index
                 (i.e. without allocating any new memory) (how?? look into storeNewScan method)
                 Once Stored, Compute the median based on temporal relatives, using findMedian method.
Time-Complexity: O(NDLog(D) + D + N) for one timeStep
*/
void LSF::MedianFilter::update(std::vector<float>& input_scan){
    if(input_scan.size() != rangeMeasure) return;
    if(currentTimeStamp == 0) { initSetUp(input_scan);}
    storeNewScan(input_scan);
    findMedian(input_scan);
}

/**
  MedianFilter findMedian Method:
        Purpose: Given an input sequence and D previous Scans find, the median.
           Args: input Scan and the matrix
        Working: Sort the elements in the matrix for every column and then find the median, Update in the corresponding location of input_scan;
  Time-Complexity: O(DLog(D))*N
*/
void LSF::MedianFilter::findMedian(std::vector<float>& input_scan){

    int numElemBother = currentTimeStamp > Dval ? Dval : currentTimeStamp;

    int i, j;
    std::vector<float> tempVector(numElemBother, 0.0);
    #pragma omp parallel for default(shared) private(i, j, tempVector)
    for (i = 0; i < rangeMeasure; i++){
         for (j = 0; j < numElemBother; j++){
              tempVector[j] = Db[j][i];
         }
         std::sort(tempVector.begin(), tempVector.end());
         input_scan[i] = medianHelper(tempVector);
    }
}

/** MedianFilter medianHelper Method:
        Purpose: Given a Sorted Array, return the median.
           Args: Sorted Array
        Working: If the length is odd, return the middle one, else return the avg of the mid two elements
  Time-Complexity: O(1)
*/

float LSF::MedianFilter::medianHelper(std::vector<float>& tempVector){
      int length = tempVector.size();
      if(length%2 != 0) {
          int index = (int)std::floor(length/2);
          return tempVector[index];
      }
      else {
        int indexR = (int)floor(length/2);
        int indexL = indexR - 1;
        //std::cout << indexR << " " << indexL << std::endl;
        return (tempVector[indexR] + tempVector[indexL])/2;
      }
}

/** MedianFilter storeNewScan Method:
        Purpose: Implements a way of maintaining a constant space requirement and efficiently accomodating a new sequence.
           Args: Input Scan Sequence
        Working: Circular Rotation of Pointers. So, every sequence is a continuos allocation in memory i.e. a row in the matrix and thus has a pointer to the head.
                 For Eg: When a new sequence comes in, the oldest have to be kicked out,
                 i.e. in simplest case, the new array has to be put in the location of oldest location but an index has to be maintained and this circular rotation so that the last pointer still points to the location which has the latest array.
  Time-Complexity: O(Dval + N) (D pointer exchanges and N data copy operations)
*/
void LSF::MedianFilter::storeNewScan(std::vector<float>& input_scan){

    /// When currentTimeStamp is still less that Dval, the matrix still has space to accomodate new sequences and thus this is easy to understand//
    if (currentTimeStamp < Dval){
        int k;
        #pragma omp parallel for default(shared) private(k)
        for(k = 0; k < input_scan.size(); k++){
            Db[currentTimeStamp][k] = input_scan[k];
        }
    }
    // However, when the matrix is filled up and a new sequence comes in, the last one has to be kicked off
    else{
       // Circular pointer transfer code //
       float *temp = Db[0];
       int l = 0;
       while(l < Dval-1){
            Db[l] = Db[l+1];
            l++;
       }
       Db[Dval-1] = temp;
       // ------------------------------//

       /// Once a location is freed for the new sequce then it is pretty obvious//
       int k;
       #pragma omp parallel for default(shared) private(k)
       for(k = 0; k < input_scan.size(); k++){
           Db[Dval-1][k] = input_scan[k];
       }
    }
    currentTimeStamp++;
}


/** MedianFilter initSetUp Method:
        Purpose: Create a Matrix of dimensions (Dval * rangeMeasure), where every row is a continuos allocated memory.
                 Also, checks and breaks the execution, if the initialization is not proper.
           Args: Input Scan Sequence
        Working: Simply Loop over Dval times and evertime allocate an array of size rangeMeasure.
        Nuances: Why isn't OpenMP used here? Well, Because heap allocation is process specific
                 and threads have to be queued anyway.
Time-Complexity: O(Dval*N)
*/
void LSF::MedianFilter::initSetUp(std::vector<float>& input_scan){
      float *arr;
      int i = 0;
      while (i < Dval){
          arr = (float *)calloc(rangeMeasure, sizeof(float));
          if (arr == NULL) throw std::runtime_error("Cannot Allocate Requested Memory");
          freeCheck.push_back(1);
          Db.push_back(arr);
          i++;
      }

      if(Db.size() != Dval) throw std::runtime_error("Something wrong With Memory Allocation in the Vector Datatype");
}
