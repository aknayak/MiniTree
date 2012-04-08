#include "MiniTree/Selection/interface/MyJet.h"

MyJet::MyJet():
  jetName(""),
  parton_id(0),
  parton_mother_id(0),
  partonFlavour(0),
  triggerJet_pt(0.),
  DistanceToClosestLepton(-999.),
  jetCharge(0),
  emEnergyFraction(0.0),
  hadEnergyFraction(0.),
  CHEF(0.),
  CEEF(0.),
  NHEF(0.),
  NEEF(0.),
  chargeMultiplicity(-999),
  NumberOfDoughters(-999),
  n90Hits(0.),
  fHPD(0.),
  fRBX(0.),
  RestrictedEMF(0.),
  nHCALTowers(0.),
  nECALTowers(0.),
  nTracks(0),
  lead_track_pt(0),
  lead_track_charge(0),
  lead_track_d0(-999.),
  lead_track_d0_Significance(-999.),
  //isLOOSE(-1),
  //isTIGHT(-1),
  jetIDLoose(false),
  etaetaMoment(0.),
  phiphiMoment(0.),
  quality(0),
  tau_againstElectronLoose(0), 
  tau_againstElectronMedium(0), 
  tau_againstElectronTight(0), 
  tau_againstElectronMVA(0), 
  tau_againstMuonLoose(0), 
  tau_againstMuonMedium(0), 
  tau_againstMuonTight(0) 

{
}

MyJet::~MyJet() 
{
}

void MyJet::Reset()
{
  p4.SetCoordinates(0.0, 0.0, 0.0, 0.0);
  vertex.SetCoordinates(-999.0,-999.0,-999.0);
  tau_vertex.SetCoordinates(-999.0,-999.0,-999.0);

  jetName = "";
  parton_id = 0;
  parton_mother_id = 0;
  partonFlavour = 0;
  triggerJet_pt = 0.;
  DistanceToClosestLepton = -999.;
  jetCharge = 0;
  emEnergyFraction = 0.0;
  hadEnergyFraction = 0.;
  CHEF = 0.;
  CEEF = 0.;
  NHEF = 0.;
  NEEF = 0.;
  chargeMultiplicity = -999;
  NumberOfDoughters = -999;
  n90Hits = 0.;
  fHPD = 0.;
  fRBX = 0.;
  RestrictedEMF = 0.;
  nHCALTowers = 0.;
  nECALTowers = 0.;
  nTracks = 0;
  lead_track_pt = 0;
  lead_track_charge = 0;
  lead_track_d0 = -999.;
  lead_track_d0_Significance = -999.;
  //isLOOSE = -1;
  //isTIGHT = -1;
  jetIDLoose = false;
  etaetaMoment = 0.;
  phiphiMoment = 0.;

  JECs.clear();
  JECUncertainty = 1.0;
  bDiscriminator.clear();
  quality = 0;

  tau_againstElectronLoose = 0; 
  tau_againstElectronMedium = 0; 
  tau_againstElectronTight = 0; 
  tau_againstElectronMVA = 0; 
  tau_againstMuonLoose = 0; 
  tau_againstMuonMedium = 0; 
  tau_againstMuonTight = 0; 
}
