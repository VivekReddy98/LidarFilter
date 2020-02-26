#include "karri-vivek.h"
#include <iostream>
#include <queue>
#include <cmath>
#include <algorithm>

LSF::RangeFilter::RangeFilter(float max, float min){
    if (max < min) {
      throw std::runtime_error("Maximum is less than Minumum while initializing the Range Filter");
    }
    range_max = max;
    range_min = min;
}

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

LSF::MedianFilter::MedianFilter(int D, int N){
    if (D <= 0){
      throw std::runtime_error("D Must be >= 0");
    }
    Dval = D+1;
    currentTimeStamp = 0;
    rangeMeasure = N;
}

LSF::MedianFilter::~MedianFilter(){
    int limit = freeCheck.size();
    for(int i = 0; i<limit; i++){
        free(Db[i]);
    }
}

void LSF::MedianFilter::update(std::vector<float>& input_scan){
    if(input_scan.size() != rangeMeasure) return;
    if(currentTimeStamp == 0) { initSetUp(input_scan);}
    storeNewScan(input_scan);
    findMedian(input_scan);
}


void LSF::MedianFilter::findMedian(std::vector<float>& input_scan){

    int numElemBother = currentTimeStamp > Dval ? Dval : currentTimeStamp;

    int i, j;
    std::vector<float> tempVector(numElemBother, 0.0);

    for (i = 0; i < rangeMeasure; i++){
         for (j = 0; j < numElemBother; j++){
              tempVector[j] = Db[j][i];
         }
         std::sort(tempVector.begin(), tempVector.end());
         input_scan[i] = medianHelper(tempVector);
    }
}

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

void LSF::MedianFilter::storeNewScan(std::vector<float>& input_scan){
    if (currentTimeStamp < Dval){
        int k;
        #pragma omp parallel for default(shared) private(k)
        for(k = 0; k < input_scan.size(); k++){
            Db[currentTimeStamp][k] = input_scan[k];
        }
    }
    else{
       float *temp = Db[0];
       int l = 0;
       while(l < Dval-1){
            Db[l] = Db[l+1];
            l++;
       }
       Db[Dval-1] = temp;
       int k;
       #pragma omp parallel for default(shared) private(k)
       for(k = 0; k < input_scan.size(); k++){
           Db[Dval-1][k] = input_scan[k];
       }
    }
    currentTimeStamp++;
}

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
