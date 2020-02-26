/**
 * LSF Filters

 * Declarations for two types of filters are available in this header file
 *
 * \author $Author Vivek Reddy Karri $
 */

#include <vector>
#include <iostream>

/**
 * _OPENMP is set when -fopenmp flag is used while compilation.
 */
#ifdef _OPENMP
#include "omp.h"
#endif

/** Separate namespace defined for Lidar Scan Filters */
namespace LSF {
    class Filter;
    class RangeFilter;
    class MedianFilter;
}

/** An interface for other Lidar Scan filters to extend from. Must implement update method. */
class LSF::Filter {
  public:
    /** A pure virtual function */
    /** Note: Update function updated the values in place and as such doesn't return anything */
    virtual void update(std::vector<float>& ) = 0;
};

/** Definition for Range Filter Class, This inherits from LSF::Filter= */
class LSF::RangeFilter: public LSF::Filter
{
  /** Private Variables Upper and Lower threshold limits */
  private:
    float range_max, range_min;
  public:
    /** Constructor for the RangeFilter Class and the strictly enforced update method by LSF::Filter */
    RangeFilter(float , float );
    void update(std::vector<float>& );
};

/** Definition for Temporal Median Filter Class, This also inherits from LSF::Filter */
class LSF::MedianFilter: public LSF::Filter
{
  private:
    /** Data Structure Required for this Class
    *  1) Variable are Obvious, Dval is D, rangeMeasure is N.
    *     currentTimeStamp maintains number of sequences filtered till that point of time.
    *  2) A Vector of Float pointers is the main Data Structure for this class.
          It is essentially a matrix of Dimensions D * N in row-major order. (So Space-Complexity of this class is O(DN) )
          freeCheck is just a helper array, assisting destructor in freeing up the heap memory.
    */
    unsigned int currentTimeStamp, Dval, rangeMeasure;
    std::vector<float*> Db;
    std::vector<int> freeCheck;

    /** All these are helper methods user by upate method
    *  Working Details of the helper methods is discussed in the src-file.
    */
    void initSetUp(std::vector<float>& );
    void storeNewScan(std::vector<float>& );
    void findMedian(std::vector<float>& );
    float medianHelper(std::vector<float>& tempVector);
  public:

    /** Public API's */
    /** A destructor is explicitly defined because this class uses heap memory */
    MedianFilter(int , int );
    ~MedianFilter();
    void update(std::vector<float>& );
};
