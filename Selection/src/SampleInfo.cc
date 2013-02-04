#include "MiniTree/Selection/interface/SampleInfo.h"

SampleInfo::SampleInfo()
{
  sampleName="";
  mcEvtType = 0;
  pileup.clear();
  truepileup.clear();
  puWeights.clear();
  truepuWeights.clear();
  hepNUP = -99;
}

SampleInfo::~SampleInfo()
{
}
