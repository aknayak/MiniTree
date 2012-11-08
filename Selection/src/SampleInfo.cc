#include "MiniTree/Selection/interface/SampleInfo.h"

SampleInfo::SampleInfo()
{
  sampleName="";
  mcEvtType = 0;
  pileup.clear();
  truepileup.clear();
  puWeights.clear();
  truepuWeights.clear();
}

SampleInfo::~SampleInfo()
{
}
