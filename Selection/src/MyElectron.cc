#include "MiniTree/Selection/interface/MyElectron.h"

MyElectron::MyElectron():
  name(""),
  charge(-999.), 
  gen_id(0),
  gen_mother_id(0),
  isEcalDriven(-1),
  isTrackerDriven(-1),
  isPFlow(-1),
  eSuperClusterOverP(-999.),
  deltaEtaSeedClusterTrackAtCalo(-999.),
  deltaPhiSeedClusterTrackAtCalo(-999.),
  deltaEtaSuperClusterTrackAtVtx(-999.),
  deltaPhiSuperClusterTrackAtVtx(-999.),
  hadronicOverEm(-999.),
  sigmaIetaIeta(-999.),
  fbrem(-999.),
  sieta(-999.),
  siphi(-999.),
  seedEnergy(-999.),
  e3x3(-999.),
  e5x5(-999.),
  rookbfrac(-999.),
  s4s1(-999.),
  electronSCEt(-999.),
  electronSCEta(-999.),
  outOfTimeFlag(-999.),
  seedTime(-999.),
  isEE(-1),
  isEB(-1),
  nHits(-999.),
  nLostHits(-999.),
  nLostPixelHits(-999.),
  normChi2(-999.),
  TrkIso(-999.),
  ECalIso(-999.),
  HCalIso(-999.),
  RelIso(-999.),
  TrkIso04(-999.),
  ECalIso04(-999.),
  HCalIso04(-999.),
  RelIso04(-999.),
  IP3D(-999),
  IP3DError(-999.),
  D0(-999.),
  D0Error(-999.),
  trigger_ele_pt(0),
  quality(0)
{
}

MyElectron::~MyElectron()
{
}


void MyElectron::Reset()
{
  p4.SetCoordinates(0.0,0.0,0.0,0.0);
  vertex.SetCoordinates(0.0,0.0,0.0);
  
  name = "";
  charge = -999.; 
  gen_id = 0;
  gen_mother_id = 0;
  isEcalDriven = -1;
  isTrackerDriven = -1;
  isPFlow = -1;
  eSuperClusterOverP = -999.;
  deltaEtaSeedClusterTrackAtCalo = -999.;
  deltaPhiSeedClusterTrackAtCalo = -999.;
  deltaEtaSuperClusterTrackAtVtx = -999.;
  deltaPhiSuperClusterTrackAtVtx = -999.;
  hadronicOverEm = -999.;
  sigmaIetaIeta = -999.;
  fbrem = -999.;
  sieta = -999.;
  siphi = -999.;
  seedEnergy = -999.;
  e3x3 = -999.;
  e5x5 = -999.;
  fbrem = -999.;
  rookbfrac = -999.;
  s4s1 = -999.;
  electronSCEt = -999.;
  electronSCEta = -999.;
  outOfTimeFlag = -999.;
  seedTime = -999.;
  isEE = -1;
  isEB = -1;
  nHits = -999.;
  nLostHits = -999.;
  nLostPixelHits = -999.;
  normChi2 = -999.;
  TrkIso = -999.;
  ECalIso = -999.;
  HCalIso = -999.;
  RelIso = -999.;
  TrkIso04 = -999.;
  ECalIso04 = -999.;
  HCalIso04 = -999.;
  RelIso04 = -999.;
  IP3D = -999;
  IP3DError = -999.;
  D0 = -999.;
  D0Error = -999.;
  trigger_ele_pt = 0;
  quality = 0;

  eidWPs.clear();
}
