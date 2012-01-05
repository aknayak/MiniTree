#ifndef __MYMET_H__
#define __MYMET_H__

#include "TROOT.h"
#include <map>
#include <vector>
#include <string>

#include "MomentumVec.h"


class MyMET
{
 public:
  MyMET();
  ~MyMET();
  
  void Reset();
  
  std::string metName;
  
  MyLorentzVector p4;         // missing Et vector -->> 4D vector since no 2D object available in [root/5.14.00f-CMS3q] ...
  
  double sumEt;
  double metSignificance;
  double emEtFraction;
  double hadEtFraction;
  double muonEtFraction;
  bool isPFMET;
  bool isCaloMET;
  
 private :

};
#endif
