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
    RangeFilter(float max, float min);
    void update(std::vector<float>& input_scan);
};

class LSF::MedianFilter
{
  private:
    int currentTimeStamp, Dval, rangeMeasure;
    std::vector<float*> Db;
    std::vector<int> freeCheck;
  public:
    MedianFilter(int valD);
    ~MedianFilter();
    void update(std::vector<float>& input_scan);
    void initSetUp(std::vector<float>& input_scan);
    void storeNewScan(std::vector<float>& input_scan)
};
