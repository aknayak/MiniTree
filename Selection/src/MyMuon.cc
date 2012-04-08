#include "MiniTree/Selection/interface/MyMuon.h"

MyMuon::MyMuon():
  name(""),
  charge(-999.),
  type(0),
  gen_id(0),
  gen_mother_id(0),
  idPattern(0),
  HadEnergy(-999.),
  EmEnergy(-999.),
  TrkIso(-999.),
  ECaloIso(-999.),
  HCaloIso(-999.),
  RelIso(-999.),
  UserPFRelIso(-999.),
  GlobalMuonPromptTight(-1),
  normChi2(-999.),
  nHits(-999),
  nMuonHits(-999),
  inTrk_normChi2(-999.),
  inTrk_nHits(-999),
  IP3D(-999),
  IP3DErr(-999),
  D0(-999.),
  D0Inner(-999),
  trigger_mu_pt(0),
  quality(0)
{
}

MyMuon::~MyMuon()
{
}

void MyMuon::Reset()
{
  
  p4.SetCoordinates(0.0, 0.0, 0.0, 0.0);
  vertex.SetCoordinates(0.0, 0.0, 0.0);

  name = "";
  charge = -999.;
  type = 0;
  gen_id = 0;
  gen_mother_id = 0;
  idPattern = 0;
  HadEnergy = -999.;
  EmEnergy = -999.;
  TrkIso = -999.;
  ECaloIso = -999.;
  HCaloIso = -999.;
  RelIso = -999.;
  UserPFRelIso = -999.;
  GlobalMuonPromptTight = -1;
  normChi2 = -999.;
  nHits = -999;
  nMuonHits = -999;
  inTrk_normChi2 = -999.;
  inTrk_nHits = -999;
  IP3D = -999;
  IP3DErr = -999;
  D0 = -999.;
  D0Inner = -999;
  trigger_mu_pt = 0;
  quality = 0;
}
