#include <vector>
#include <iostream>
#include <chrono>

#ifdef _OPENMP
#include "omp.h"
#endif

namespace LSF { // Lidar Scan Filters
    class RangeFilter;
    class MedianFilter;
}

class LSF::RangeFilter
{
  private:
    float range_max, range_min;
  public:
    RangeFilter(float , float );
    void update(std::vector<float>& );
};

class LSF::MedianFilter
{
  private:
    unsigned int currentTimeStamp, Dval, rangeMeasure;
    std::vector<float*> Db;
    std::vector<int> freeCheck;
    void initSetUp(std::vector<float>& );
    void storeNewScan(std::vector<float>& );
    void findMedian(std::vector<float>& );
    float medianHelper(std::vector<float>& tempVector);
  public:
    MedianFilter(int , int );
    ~MedianFilter();
    void update(std::vector<float>& );
};
