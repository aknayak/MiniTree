#ifndef __MYJET_H__
#define __MYJET_H__

#include "TROOT.h"
#include <map>
#include <vector>
#include <string>

#include "MomentumVec.h"

class MyJet 
{
 public:
  MyJet();
  ~MyJet();
  
  void   Reset();
  
  MyLorentzVector p4;         // 4 vector of jet.
  std::map<std::string, double>JECs;
  double JECUncertainty;
  std::string jetName;
  int parton_id;
  int parton_mother_id;
  int partonFlavour;
  double triggerJet_pt;
  std::map<std::string, double>bDiscriminator;
  double DistanceToClosestLepton;
  double jetCharge;
  
  // for Jet IDentification: 
  //PFJet
  double emEnergyFraction;   // jet electromagnetic energy fraction  // ( not defined for pfjet)
  double hadEnergyFraction;  // jet hadronic energy fraction
  double CHEF;               // Charged Hadronic Energy Fraction  
  double CEEF;               // Charged Electromagnetic Energy Fraction  (for Pf jet ID)
  double NHEF;               // Neutral Hadronic Energy Fraction  
  double NEEF;               // Neutral Electromagnetic Energy Fraction  
  int chargeMultiplicity;
  int NumberOfDoughters;
  
  //CaloJet Specific
  double n90Hits;            // Number of recHits (i.e. cells) needed to make up 90% of the jet's energy 
  double fHPD;               // fraction of jet energy from the highest energy Hybrid Photo Diode (not for pfjet)
  double fRBX;
  double RestrictedEMF;
  int nHCALTowers;
  int nECALTowers;
  
  //int  isLOOSE;              // JetId criteria (LOOSE PURE09 for CaloJet, LOOSE FIRSTDATA for PfJet)  
  //int  isTIGHT;              // JetId criteria (TIGHT PURE09 for CaloJet, TIGHT FIRSTDATA for PfJet)  
  bool jetIDLoose;             //Loose jetId

  //Pileup JetID
  int puIDMVALoose;
  int puIDMVAMedium;
  int puIDMVATight;
  float puIDMVADiscr;

  // Tracks associated to the jet
  int    nTracks;
  double lead_track_pt;
  double lead_track_charge;
  double lead_track_d0;
  double lead_track_d0_Significance;
  
  double etaetaMoment;
  double phiphiMoment;

  int quality;

  Point3D vertex;
  
  //needed for tau-fake rate studies
  Point3D tau_vertex;
  float tau_againstElectronLoose; 
  float tau_againstElectronMedium; 
  float tau_againstElectronTight; 
  float tau_againstElectronMVA; 
  float tau_againstMuonLoose; 
  float tau_againstMuonMedium; 
  float tau_againstMuonTight; 

 private :

}; 
#endif
