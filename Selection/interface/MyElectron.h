#ifndef __MYELECTRON_H__
#define __MYELECTRON_H__

#include "TROOT.h"
#include <map>
#include <vector>
#include <string>

#include "MomentumVec.h"

class MyElectron 
{
 public:
  MyElectron();
  ~MyElectron();
  
  void Reset();
  
  std::string GetName() { return name; };
  
  std::string name;
  int charge;
  int gen_id;
  int gen_mother_id;
  
  MyLorentzVector p4;
  Point3D       vertex;   // vertex position
  
  int    isEcalDriven;
  int    isTrackerDriven;
  int    isPFlow;
  
  //Electron Ids
  std::map<std::string, float>eidWPs;
  
  // variables for lepton ID:
  double eSuperClusterOverP;
  double deltaEtaSeedClusterTrackAtCalo;
  double deltaPhiSeedClusterTrackAtCalo;
  double deltaEtaSuperClusterTrackAtVtx;
  double deltaPhiSuperClusterTrackAtVtx;
  double hadronicOverEm;
  double sigmaIetaIeta;
  double fbrem;
  
  
  double sieta; //sigma ieta-ieta 
  double siphi; //sigma iphi-iphi
  double seedEnergy;
  double e3x3;
  double e5x5;
  double rookbfrac;  //rook5B-fraction
  double s4s1;  //s4/s1 - swiss cross
  double electronSCEt;
  double electronSCEta;
  double outOfTimeFlag;
  double seedTime;
  
  int    isEE;
  int    isEB;
  
  int nHits;
  int nLostHits;
  int nLostPixelHits;
  double normChi2;
  
  double TrkIso;     //summed track pt in a cone of deltaR<0.3
  double ECalIso;
  double HCalIso;
  double RelIso;
  
  double TrkIso04;     //summed track pt in a cone of deltaR<0.4
  double ECalIso04;
  double HCalIso04;
  double RelIso04;
  
  double IP3D;
  double IP3DError;
  double D0;
  double D0Error;
  
  double trigger_ele_pt;
  int quality;

 private :

};
#endif
