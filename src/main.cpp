#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>
#include <cstring>
#include <typeinfo>
#include "karri-vivek/karri-vivek.h"

/// Input Parse Function, Given a string with whitespace sperated tokens, return a vector of tokens//
std::vector<float> inParse(std::string );

// Entry point for normal/release mode usage of the application
int main(int argc, char **argv) {

    // Polymorphism in play
    LSF::Filter *lsf;

    /// Specify arguments accoridng to the requirement //
    if(argc != 6 && argc != 5) throw std::runtime_error("Agruments to be Specified: {rf} {timeSteps} {N} {MAX} {MIN} \
                                                                          or {mf} {timeSteps} {N} {D} ");

    /// Initializing variable and the class //
    int numTimeSteps = atoi(argv[2]);
    int scanSize = atoi(argv[3]);
    if (strcmp(argv[1], "rf") == 0){
        float max = atof(argv[4]);
        float min = atof(argv[5]);
        lsf = new LSF::RangeFilter((float)max, (float)min);
    }
    else if(strcmp(argv[1], "mf") == 0){
        int D = atoi(argv[4]);
        lsf = new LSF::MedianFilter((int)D, (int)scanSize);
    }
    else{
       throw std::runtime_error("Filter should be one of rf/mf");
    }

    std::string inputString;
    std::vector<float> inpScan;

    /// Loop over the number of timestamps, parse the string input, call the update method on the Filter class \
        and finally print the filtered Sequence //
    for (int i = 0; i < numTimeSteps; i++){
        std::getline(std::cin, inputString);
        inpScan = inParse(inputString);
        lsf->update(inpScan);
        if (inpScan.size() == 0) continue;
        std::ostream_iterator<float> out_it (std::cout," ");
        std::cout << "Filtered Sequence: " ;
        std::copy ( inpScan.begin(), inpScan.end(), out_it );
        std::cout << "\n";
    }

    // Free the Pointer to the class
    free(lsf);

    return 0;
}

// String Parser Class,
std::vector<float> inParse(std::string inputString){

      std::vector<float> eval;

      int n = inputString.length();

      char char_array[n + 1];

      strcpy(char_array, inputString.c_str());

      char *token = strtok(char_array, " ");

      // Loop over until tokens are left //
      while (token != NULL) {
          float f = atof(token);
          eval.push_back(f);
          //std::cout << f << std::endl;
          token = strtok(NULL, " ");
      }

      // Return the vector of tokens
      return eval;
}
