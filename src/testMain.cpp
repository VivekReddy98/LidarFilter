#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>
#include <cstring>
#include <typeinfo>
#include "../src/karri-vivek/karri-vivek.h"

/// Input Parse Function, Given a string with whitespace sperated tokens, return a vector of tokens//
std::vector<float> inParse(std::string );

// Entry point for Debug mode usage of the application
int main(){
    int numTimeSteps, TMFvar, ScanSize;
    float max, min;

    /// Both the Classes are run on the input filtered sequence, and the output is printed out exactly as specified in the normal mode entry point
    std::cin >> numTimeSteps >> TMFvar >> ScanSize >> max >> min;

    LSF::RangeFilter *rf = new LSF::RangeFilter((float)max, (float)min);

    LSF::MedianFilter *mf = new LSF::MedianFilter((int)TMFvar, (int)ScanSize);

    std::string inputString;
    std::vector<float> inpScan;

    for (int i = 0; i <= numTimeSteps; i++){
        std::getline(std::cin, inputString);

        inpScan = inParse(inputString);

        if (inpScan.size() == 0) continue;


        std::cout << "             Input: ";
        for(auto elem:inpScan) std::cout << elem << " ";
        std::cout << "\n";

        rf->update(inpScan);
        std::cout << "RF: ";
        for(auto elem:inpScan) std::cout << elem << " ";
        std::cout << "\n";

        inpScan = inParse(inputString);
        mf->update(inpScan);
        std::cout << "MF: ";
        for(auto elem:inpScan) std::cout << elem << " ";
        std::cout << "\n";
    }

    free(rf);
    free(mf);

    return 0;
}

std::vector<float> inParse(std::string inputString){

      std::vector<float> eval;

      int n = inputString.length();

      char char_array[n + 1];

      strcpy(char_array, inputString.c_str());

      char *token = strtok(char_array, " ");

      while (token != NULL) {
          float f = atof(token);
          eval.push_back(f);
          //std::cout << f << std::endl;
          token = strtok(NULL, " ");
      }

      return eval;
}
