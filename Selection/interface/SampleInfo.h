#ifndef __SAMPLEINFO_H__
#define __SAMPLEINFO_H__

#include "TROOT.h"
#include <string>
#include <vector>

class SampleInfo
{
 public:
  SampleInfo();
  ~SampleInfo();
  
  std::string sampleName;
  int mcEvtType;
  std::vector<double>pileup;
  std::vector<double>puWeights;

 private :

};
#endif 
