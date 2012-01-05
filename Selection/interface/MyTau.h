#ifndef __MYTAU_H__
#define __MYTAU_H__


#include "TROOT.h"
#include <map>
#include <vector>
#include <string>

#include "MomentumVec.h"


class MyTau 
{
 public:
  MyTau();
  ~MyTau();
  
  void Reset();
  
  std::string GetAlgoName() { return tauAlgo; };
  
  std::string tauAlgo;
  double charge;
  int gen_id;
  int gen_mother_id;
  
  MyLorentzVector p4;
  Point3D        vertex;   // vertex position
  
  //pf tau variables
  double nSignalChargeHadron;
  double leadChargedHadronPt;
  double leadChargedHadronP;
  double leadingParticlePt;
  double leadingParticleP;
  
  double matchJetPt;
  double matchJetEta;
  double matchJetPhi;
  double matchJetEMF;
  
  double tauEMF;
  double d0_max;
  double d0_max_err;
  
  //HPS-TAU discriminators
  float decayModeFinding;
  
  float LooseIsolation;
  float LooseIsolationDeltaBetaCorr;
  float LooseCombinedIsolationDeltaBetaCorr;
  
  float MediumIsolation;
  float MediumIsolationDeltaBetaCorr;
  float MediumCombinedIsolationDeltaBetaCorr;
  
  float TightIsolation;
  float TightIsolationDeltaBetaCorr;
  float TightCombinedIsolationDeltaBetaCorr;
  
  float againsElectronLoose;
  float againstElectronMedium;
  float againstElectronTight;
  float againstMuonLoose;
  float againstMuonTight;
  
  float decayMode;

 private :

};
#endif

