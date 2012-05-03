#include "MiniTree/Selection/interface/MyEventSelection.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"

std::vector<MyElectron> MyEventSelection::getElectrons(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  
  std::vector<MyElectron> selElectrons; 
  selElectrons.clear();
  
  try{
    //config parameters
    std::vector<edm::InputTag> sources = configParamsElectrons_.getParameter<std::vector<edm::InputTag> >("sources");
    double minEt = configParamsElectrons_.getParameter<double>("minEt");
    double minSCEt = configParamsElectrons_.getParameter<double>("minSCEt");
    double maxEta = configParamsElectrons_.getParameter<double>("maxEta");
    std::string id = configParamsElectrons_.getParameter<std::string>("id");
    double maxD0 = configParamsElectrons_.getParameter<double>("maxD0");
    double maxRelIso = configParamsElectrons_.getParameter<double>("maxRelIso");
    //bool useDefaultIso = configParamsElectrons_.getParameter<bool>("useDefaultIso");
    //double minDRmu2el = configParamsElectrons_.getParameter<double>("minDeltaRtoMuons");
    int maxLostHits = configParamsElectrons_.getParameter<int>("maxTrackLostHits");
    //double minSigmaIetaIeta = configParamsElectrons_.getParameter<double>("minSigmaIetaIeta");
    //double minS4S1 = configParamsElectrons_.getParameter<double>("minS4S1");
    //bool ecalOnly = configParamsElectrons_.getParameter<bool>("ecalOnly");
    std::string triggerMatch = configParamsElectrons_.getParameter<std::string>("triggerMatch");
    
    EcalClusterLazyTools lazytools( iEvent, iSetup,
				    configParamsElectrons_.getParameter<edm::InputTag>("ebRecHits"),
				    configParamsElectrons_.getParameter<edm::InputTag>("eeRecHits") );
    iEvent.getByLabel(configParamsElectrons_.getParameter<edm::InputTag>("ebRecHits"),ebRecHits_);
    iEvent.getByLabel(configParamsElectrons_.getParameter<edm::InputTag>("eeRecHits"),eeRecHits_);
    
    //get triger match
    edm::Handle< pat::TriggerEvent > triggerEvent;
    iEvent.getByLabel( configParamsJets_.getParameter<edm::InputTag>("triggerEvent"), triggerEvent );
    //const pat::TriggerObjectRefVector triggerElectrons( triggerEvent->objects( trigger::TriggerElectron ) );
    
    //collect Electrons
    for(std::vector<edm::InputTag>::iterator sit = sources.begin();
        sit != sources.end();
          sit++)
        {
	  TString rawtag=sit->label();
          rawtag.ReplaceAll("pat","");
          rawtag.ReplaceAll("cleanPat","");
          rawtag.ReplaceAll("selectedPat","");
          std::string tag(rawtag);

          edm::Handle<pat::ElectronCollection>ieles;
          try{
             iEvent.getByLabel( *sit, ieles);
          }catch(std::exception &e){
            continue;
          }
          if(!ieles.isValid()) continue;
          if(ieles->size() == 0)continue;

          for(size_t iEle = 0; iEle < ieles->size(); iEle++)
            {
              const pat::Electron eIt = ((*ieles)[iEle]);
              MyElectron newElectron = MyElectronConverter(eIt, lazytools, rawtag);
	      newElectron.name = tag;

	      std::string labelMatcher = tag+triggerMatch;
              pat::helper::TriggerMatchHelper tmhelper;
              const pat::TriggerObjectRef objRef(tmhelper.triggerMatchObject( ieles, iEle, labelMatcher, iEvent, *triggerEvent ));
              if(objRef.isAvailable()){newElectron.trigger_ele_pt = objRef->pt();}

	      //make selections
              bool passKin = true, passId = true, passIso = true;
              if(newElectron.p4.Et() < minEt || 
		 fabs(newElectron.p4.Eta()) > maxEta || 
		 newElectron.electronSCEt < minSCEt) passKin = false;
              //id
	      int eid=-1;
              if( !id.empty() ) eid = (int) eIt.electronID(id);
              if(tag.find("PFlow") != std::string::npos) eid = (int)eIt.electronID("eidVeryLooseMC");
	      bool isFromConversion(false);
              if(id.find("simple") != std::string::npos && eid>=0) isFromConversion = !((eid>>2) & 0x1);
	      if(eid>=0 &&  !(eid & 0x1) ) passId = false;
	      if(isFromConversion) passId = false;
	      if(newElectron.nLostHits > maxLostHits || fabs(newElectron.D0*10000) >  maxD0)passId = false;
	      //iso
	      if(newElectron.RelIso > maxRelIso) passIso = false;

	      int quality = 0;
              if(passKin)quality  = 1;
              //std::cout<<"electron quality "<<quality<<std::endl;
              if(passId)quality |= 1<<1;
              //std::cout<<"electron quality "<<quality<<std::endl;
              if(passIso)quality |= 1<<2;
              //std::cout<<"electron quality "<<quality<<std::endl;
              newElectron.quality = quality;
	      
              if(passKin)selElectrons.push_back(newElectron);
            }
        }
  }catch(std::exception &e){
    std::cout << "[Electron Selection] : check selection " << e.what() << std::endl;
  }
  
  return selElectrons;
}
  
    
MyElectron MyEventSelection::MyElectronConverter(const pat::Electron& iEle, EcalClusterLazyTools& lazytools, TString& dirtag)
{
  MyElectron newElectron;
  newElectron.Reset();

  newElectron.p4.SetCoordinates(iEle.px(), iEle.py(), iEle.pz(), iEle.energy());
  reco::PFCandidateRef pfRef=iEle.pfCandidateRef();
  if(!pfRef.isNull())
    {
       newElectron.p4.SetCoordinates(pfRef->px(), pfRef->py(), pfRef->pz(), pfRef->energy());
    }
  
  newElectron.vertex.SetCoordinates(iEle.vx(), iEle.vy(), iEle.vz());

  newElectron.charge = iEle.charge(); 
  
  myhistos_["pt_"+dirtag]->Fill(iEle.pt());
  myhistos_["lowpt_"+dirtag]->Fill(iEle.pt());
  myhistos_["eta_"+dirtag]->Fill(iEle.eta());
  myhistos_["phi_"+dirtag]->Fill(iEle.phi());
  
  const reco::GenParticle *gen = iEle.genLepton();
  if(gen){
    newElectron.gen_id = gen->pdgId();
    if(gen->numberOfMothers() > 0)
      newElectron.gen_mother_id = gen->mother()->pdgId();
  }

  newElectron.isEcalDriven = iEle.ecalDrivenSeed();
  newElectron.isTrackerDriven = iEle.trackerDrivenSeed();
  newElectron.isPFlow = -1;
  newElectron.eSuperClusterOverP = iEle.eSuperClusterOverP();
  newElectron.deltaEtaSeedClusterTrackAtCalo = iEle.deltaEtaEleClusterTrackAtCalo();
  newElectron.deltaPhiSeedClusterTrackAtCalo = iEle.deltaPhiEleClusterTrackAtCalo();
  newElectron.deltaEtaSuperClusterTrackAtVtx = iEle.deltaEtaSuperClusterTrackAtVtx();
  newElectron.deltaPhiSuperClusterTrackAtVtx = iEle.deltaPhiSuperClusterTrackAtVtx();
  newElectron.hadronicOverEm = iEle.hadronicOverEm();
  newElectron.sigmaIetaIeta = iEle.sigmaIetaIeta();
  newElectron.fbrem = iEle.fbrem();
  
  reco::SuperClusterRef sc = iEle.superCluster(); //pf sc is returned if it is tracker driven
  double electronSCEta = sc->eta();
  double electronSCEt = sc->energy()/cosh(sc->eta());
  float sieta(-1),siphi(-1),s4s1(-1),e5x5(-1),e3x3(-1),seedEn(-1),rook5bfrac(-1),seedTime(-111111);
  bool outOfTimeFlag(false);
  try{
    std::vector<float> covs = lazytools.scLocalCovariances(*sc); 
    sieta=sqrt(covs[0]);
    //float sietaiphi = sqrt(covs[1]);
    siphi = sqrt(covs[2]);
    e3x3=lazytools.e3x3(*sc );
    e5x5=lazytools.e5x5(*sc );
    std::pair<DetId, float> max = lazytools.getMaximum(*(sc->seed())); // swiss cross
    seedEn=max.second;
    s4s1+=lazytools.eLeft(*(sc->seed()));
    s4s1+=lazytools.eRight(*(sc->seed()));
    s4s1+=lazytools.eTop(*(sc->seed()));
    s4s1+=lazytools.eBottom(*(sc->seed()));
    s4s1 /= seedEn;
    std::vector<float> rookv;                                       //rook 5- b
    rookv.push_back(lazytools.matrixEnergy(*sc, max.first, -1, -1,  0,  0 ));
    rookv.push_back(lazytools.matrixEnergy(*sc, max.first,  1,  1,  0,  0 ));
    rookv.push_back(lazytools.matrixEnergy(*sc, max.first,  0,  0, -1, -1 ));
    rookv.push_back(lazytools.matrixEnergy(*sc, max.first,  0,  0,  1,  1 ));
    rook5bfrac =  (*std::max_element(rookv.begin(),rookv.end())) / max.second;
    const EcalRecHitCollection & rechits = ( iEle.isEB() ? *ebRecHits_ : *eeRecHits_ );
    EcalRecHitCollection::const_iterator recHitIt = rechits.find( max.first );
    if( recHitIt != rechits.end() ) {
      seedTime = recHitIt->time();
      unsigned int flags = recHitIt->recoFlag();
      if( iEle.isEB() && (max.second<130 || seedTime<0) && flags == EcalRecHit::kOutOfTime ) outOfTimeFlag=true;
    }
  }catch(std::exception &e1){}
  
  newElectron.sieta = sieta;
  newElectron.siphi = siphi;
  newElectron.seedEnergy = seedEn;
  newElectron.e3x3 = e3x3;
  newElectron.e5x5 = e5x5;
  newElectron.rookbfrac = rook5bfrac;
  newElectron.s4s1 = s4s1;
  newElectron.electronSCEt = electronSCEt;
  newElectron.electronSCEta = electronSCEta;
  newElectron.outOfTimeFlag = outOfTimeFlag;
  newElectron.seedTime = seedTime;
  
  newElectron.isEE = iEle.isEB();
  newElectron.isEB = iEle.isEE();

  const reco::GsfTrackRef & eTrack = iEle.gsfTrack();
  if(!eTrack.isNull()){
    newElectron.nHits = eTrack->numberOfValidHits();
    newElectron.nLostHits = eTrack->trackerExpectedHitsInner().numberOfLostHits();
    newElectron.nLostPixelHits = eTrack->trackerExpectedHitsInner().numberOfLostPixelHits();
    newElectron.normChi2 = eTrack->normalizedChi2();
    
    reco::TransientTrack tt = trackBuilder->build(eTrack);
    if((&refVertex_))
      {
	std::pair<bool,Measurement1D> d03Dmeas = IPTools::absoluteImpactParameter3D(tt, refVertex_);
	std::pair<bool,Measurement1D> d0meas = IPTools::absoluteTransverseImpactParameter(tt, refVertex_);
	newElectron.IP3D = d03Dmeas.second.value();
	newElectron.IP3DError = d03Dmeas.second.error();
	newElectron.D0 = d0meas.second.value();
	newElectron.D0Error = d0meas.second.error();
      }
    else{
      newElectron.D0 = eTrack->dxy();
      newElectron.D0Error = eTrack->dxyError();
    }
  }
  
  //isolation
  bool isPF = false;
  if(dirtag.Contains("PFlow"))isPF = true;
  std::vector<double> iso = defaultElectronIsolation(iEle, isPF);
  newElectron.TrkIso = iso[0];
  newElectron.ECalIso = iso[1];
  newElectron.HCalIso = iso[2];
  newElectron.RelIso = iso[3];

  myhistos_["reliso_"+dirtag]->Fill(iso[3]);
  myhistos_["lowreliso_"+dirtag]->Fill(iso[3]);

  
  //id
  std::map<std::string, float> eidWPs; eidWPs.clear();
  const std::vector<pat::Electron::IdPair> & eids = iEle.electronIDs();
  int iid_cic = 0, iid_vbtf=0;
  for(size_t id = 0; id < eids.size(); id++){
    std::string id_name = eids[id].first;
    double id_value = eids[id].second;
    eidWPs[id_name] = id_value;
    
    if(id_name.find("MC") != std::string::npos){
      iid_cic++;
      if(int(id_value) & 0x1){
	myhistos_["cic_id_"+dirtag]->Fill(iid_cic);
      }
      myhistos_["cic_id_"+dirtag]->GetXaxis()->SetBinLabel(iid_cic+1, id_name.c_str());
    }
    else{
      iid_vbtf++;
      if(int(id_value) & 0x1){
	myhistos_["vbtf_id_"+dirtag]->Fill(iid_vbtf);
      }
      myhistos_["vbtf_id_"+dirtag]->GetXaxis()->SetBinLabel(iid_vbtf+1, id_name.c_str());
    }
    
  }
  newElectron.eidWPs = eidWPs;

  return newElectron;
}

std::vector<double> MyEventSelection::defaultElectronIsolation (const pat::Electron& ele, bool isPF)
{
  std::vector<double> values(4,0);
  double ePt((double)ele.pt());
  reco::PFCandidateRef pfRef=ele.pfCandidateRef();
  if(!pfRef.isNull()) { ePt = pfRef->pt(); }
  double norm=std::max((double)20.0,(double)ePt);

  if(isPF)
    {
      double puOffsetCorrection = 0;
      values[0] = ele.chargedHadronIso();
      values[1] = ele.photonIso();
      values[2] = ele.neutralHadronIso();
      values[3] = (std::max(ele.photonIso()+ele.neutralHadronIso() - puOffsetCorrection, 0.0) + ele.chargedHadronIso())/norm;
    }
  else{
    values[0] = ele.trackIso();
    values[1] = ele.ecalIso();
    values[2] = ele.hcalIso();
    values[3] = (ele.trackIso()+ele.ecalIso()+ele.hcalIso())/norm;
  }
  return values;
}
