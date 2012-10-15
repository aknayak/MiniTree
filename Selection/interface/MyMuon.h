#ifndef __MYMUON_H__
#define __MYMUON_H__

#include "TROOT.h"
#include <map>
#include <vector>
#include <string>

#include "MomentumVec.h"


class MyMuon 
{
 public:
  MyMuon();
  ~MyMuon();
  
  void Reset();
  
  int type;
  std::string GetName() { return name; };
  
  std::string name;
  
  MyLorentzVector p4; //momentum
  Point3D    vertex;   // Beam spot  position
  int gen_id;
  int gen_mother_id;
  double charge;
  
  // variables for lepton ID:
  int idPattern;
    
  
  double HadEnergy;
  double EmEnergy;
  
  double TrkIso;
  double ECaloIso; 
  double HCaloIso; 
  double RelIso;
  double ChHadIso; 
  double PhotonIso;  
  double NeuHadIso;  
  double PileupIso;
  double pfRelIso;
  double UserPFRelIso;
  
  int GlobalMuonPromptTight;
  double normChi2;
  int nHits;
  int nMuonHits;
  int nPixelHits;
  int nMatchedStations;
  int nTrackerLayers;
  double inTrk_normChi2;
  double inTrk_nHits;
  
  double IP3D;
  double IP3DErr;
  double D0;          // Transverse IP of globalTrack (tracker+mu) wrt BS (or 0 if !BS)
  double D0Inner;     // Transverse IP of InnerTrack (tracker) wrt BS (or 0 if !BS)

  double trigger_mu_pt;
  int quality;
 private :

};
#endif
