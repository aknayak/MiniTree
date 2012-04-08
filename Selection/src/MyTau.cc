#include "MiniTree/Selection/interface/MyTau.h"

MyTau::MyTau():
  tauAlgo(""),
  charge(0.),
  gen_id(0),
  gen_mother_id(0),
  nSignalChargeHadron(-999.),
  leadChargedHadronPt(-999.),
  leadChargedHadronP(-999.),
  leadingParticlePt(-999.),
  leadingParticleP(-999.),
  matchJetPt(-999.),
  matchJetEta(-999.),
  matchJetPhi(-999.),
  matchJetEMF(-999.),
  tauEMF(-999.),
  d0_max(-999.),
  d0_max_err(-999.),
  decayModeFinding(-999.),
  LooseIsolation(-999.),
  LooseIsolationDeltaBetaCorr(-999.),
  LooseCombinedIsolationDeltaBetaCorr(-999.),
  MediumIsolation(-999.),
  MediumIsolationDeltaBetaCorr(-999.),
  MediumCombinedIsolationDeltaBetaCorr(-999.),
  TightIsolation(-999.),
  TightIsolationDeltaBetaCorr(-999.),
  TightCombinedIsolationDeltaBetaCorr(-999.),
  againstElectronLoose(-999.),
  againstElectronMedium(-999.),
  againstElectronTight(-999.),
  againstElectronMVA(-999.),
  againstMuonLoose(-999.),
  againstMuonMedium(-999.),
  againstMuonTight(-999.),
  decayMode(-999.)
{
}

MyTau::~MyTau()
{
}

void MyTau::Reset()
{
  p4.SetCoordinates(0.0,0.0,0.0,0.0);
  vertex.SetCoordinates(0.0,0.0,0.0);
  
  tauAlgo = "";
  charge = 0.;
  gen_id = 0;
  gen_mother_id = 0;
  nSignalChargeHadron = -999.;
  leadChargedHadronPt = -999.;
  leadChargedHadronP = -999.;
  leadingParticlePt = -999.;
  leadingParticleP = -999.;
  matchJetPt = -999.;
  matchJetEta = -999.;
  matchJetPhi = -999.;
  matchJetEMF = -999.;
  tauEMF = -999.;
  d0_max = -999.;
  d0_max_err = -999.;
  decayModeFinding = -999.;
  LooseIsolation = -999.;
  LooseIsolationDeltaBetaCorr = -999.;
  LooseCombinedIsolationDeltaBetaCorr = -999.;
  MediumIsolation = -999.;
  MediumIsolationDeltaBetaCorr = -999.;
  MediumCombinedIsolationDeltaBetaCorr = -999.;
  TightIsolation = -999.;
  TightIsolationDeltaBetaCorr = -999.;
  TightCombinedIsolationDeltaBetaCorr = -999.;
  againstElectronLoose = -999.;
  againstElectronMedium = -999.;
  againstElectronTight = -999.;
  againstElectronMVA = -999.;
  againstMuonLoose = -999.;
  againstMuonMedium = -999.;
  againstMuonTight = -999.;
  decayMode = -999.;

}

