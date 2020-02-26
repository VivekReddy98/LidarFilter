#include "karri-vivek.h"
#include <assert.h>

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

LSF::MedianFilter::MedianFilter(int D){
    if (D <= 0){
      throw std::runtime_error("D Must be >= 0");
    }
    Dval = D;
    currentTimeStamp = 0;
    rangeMeasure = 0;
}

LSF::MedianFilter::~MedianFilter(int valD){
    int limit = freeCheck.size();
    for(int i = 0; i<limit; i++){
        free(Db[i]);
    }
}

LSF::MedianFilter::update(std::vector<float>& input_scan){
    if(currentTimeStamp == 0) { initSetUp(input_scan);}


}

LSF::MedianFilter::storeNewScan(std::vector<float>& input_scan){

}

LSF::MedianFilter::initSetUp(std::vector<float>& input_scan){
      rangeMeasure = input_scan.size();
      float *arr;
      for (int i = 0; i < Dval; i++){
          arr = calloc(rangeMeasure, sizeof(float));
          if (arr == NULL) throw std::runtime_error("Cannot Allocate Requested Memory");
          freeCheck.append(1);
          Db[i].append(arr);
      }

      if(Db.size() != Dval) throw std::runtime_error("Something wrong With Memory Allocation in the Vector Datatype");
}
