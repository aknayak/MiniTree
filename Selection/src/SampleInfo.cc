#include "MiniTree/Selection/interface/SampleInfo.h"

SampleInfo::SampleInfo()
{
  sampleName="";
  mcEvtType = 0;
  pileup.clear();
  puWeights.clear();
}

SampleInfo::~SampleInfo()
{
}
