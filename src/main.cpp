#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>
#include <cstring>
#include <typeinfo>
#include "karri-vivek/karri-vivek.h"

std::vector<float> inParse(std::string );

int main(){
    int numTimeSteps, TMFvar;
    float max, min;

    std::cin >> numTimeSteps >> TMFvar >> max >> min;

    //LSF::RangeFilter *rf = new LSF::RangeFilter((float)max, (float)min);

    LSF::MedianFilter *mf = new LSF::MedianFilter((int)TMFvar);

    std::string inputString;
    std::vector<float> inpScan;

    for (int i = 0; i <= numTimeSteps; i++){
        std::getline(std::cin, inputString);
        inpScan = inParse(inputString);
        mf->update(inpScan);
        std::ostream_iterator<float> out_it (std::cout," ");
        std::copy ( inpScan.begin(), inpScan.end(), out_it );
        std::cout << "\n";
    }

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
