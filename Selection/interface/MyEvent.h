#ifndef __MYEVENT_H__
#define __MYEVENT_H__

/*

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Aruna Kumar Nayak
//         Created:  Tue Aug 23 16:11:19 WEST 2011
// $Id: MyEvent.h,v 1.1 2012/01/05 11:12:21 anayak Exp $
//
//


// system include files
#include "TROOT.h"
#include <memory>
#include <string>
#include <vector>
#include <map>


// user include files
#include "TString.h"
#include "MiniTree/Selection/interface/MyJet.h"
#include "MiniTree/Selection/interface/MyMET.h"
#include "MiniTree/Selection/interface/MyTrack.h"
#include "MiniTree/Selection/interface/MyVertex.h"
#include "MiniTree/Selection/interface/MyElectron.h"
#include "MiniTree/Selection/interface/MyMuon.h"
#include "MiniTree/Selection/interface/MyTau.h"
#include "MiniTree/Selection/interface/MyMCParticle.h"
#include "MiniTree/Selection/interface/SampleInfo.h"
#include "MiniTree/Selection/interface/MyKineFitParticle.h"

class MyEvent
{
 public:
  MyEvent();
  ~MyEvent();
  
  enum SampleType {DATA = 0, TTBAR=10, ZJETS=30, WJETS=40, TOPS=50, TOPT=51, TOPW=52,QCD=60, WW=70, WZ=71, ZZ=72, OTHER=80};
  enum TTChannel {TTUNKNOWN=11, TTALLJETS, TTEJETS, TTMUJETS, TTTAUJETS, TTEE, TTEMU, TTETAU, TTMUMU, TTMUTAU, TTTAUTAU };
  enum ZChannel {ZUNKNOWN=31, ZEE=32, ZMUMU=33, ZTAUTAU=34, ZQQ=35};
  enum WChannel {WUNKNOWN=41, WENU=42, WMUNU=43, WTAUNU=44, WQQ=45};

  void Reset();
  
  // ---- General event information.
  unsigned int               runNb;
  unsigned int               eventNb;
  int                        lumiblock;
  
  std::vector<MyElectron> Electrons;
  std::vector<MyMuon> Muons;
  std::vector<MyJet> Jets;
  std::vector<MyTau> Taus;
  std::vector<MyMET> mets;
  //std::vector<MyTrack> tracks;
  std::vector<MyVertex> PrimaryVtxs;
  std::vector<MyMCParticle> mcParticles;
  std::vector<MyKineFitParticle> KineFitParticles;
  MyMET mcMET;
  SampleInfo sampleInfo;
  
  // ---- Monte Carlo information
  bool isData;
  // ---- Trigger info
  std::vector<std::string> hlt;

  int eventQuality;

 private :
  
};
#endif
