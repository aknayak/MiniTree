#include "MiniTree/Selection/interface/MyEventSelection.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"

std::vector<MyMuon> MyEventSelection::getMuons(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  
  std::vector<MyMuon> selMuons; 
  selMuons.clear();
  
  try{
    //config parameters
    std::vector<edm::InputTag> sources = configParamsMuons_.getParameter<std::vector<edm::InputTag> >("sources");
    double minPt = configParamsMuons_.getParameter<double>("minPt");
    double maxEta = configParamsMuons_.getParameter<double>("maxEta");
    std::string id = configParamsMuons_.getParameter<std::string>("id");
    double maxD0 = configParamsMuons_.getParameter<double>("maxD0");
    double maxRelIso = configParamsMuons_.getParameter<double>("maxRelIso");
    //bool useDefaultIso = configParamsMuons_.getParameter<bool>("useDefaultIso");
    double maxTrackChi2 = configParamsMuons_.getParameter<double>("maxTrackChi2");
    int minMuonHits = configParamsMuons_.getParameter<int>("minMuonHits");
    int minPixelHits = configParamsMuons_.getParameter<int>("minPixelHits");
    int minMatchStations = configParamsMuons_.getParameter<int>("minMatchStations");
    int minTrackerLayers = configParamsMuons_.getParameter<int>("minTrackerLayers");
    bool onlyGlobal = configParamsMuons_.getParameter<bool>("onlyGlobal");
    std::string triggerMatch = configParamsMuons_.getParameter<std::string>("triggerMatch");

    edm::Handle< pat::TriggerEvent > triggerEvent;
    iEvent.getByLabel( configParamsJets_.getParameter<edm::InputTag>("triggerEvent"), triggerEvent );
    //const pat::TriggerObjectRefVector triggerMuons( triggerEvent->objects( trigger::TriggerMuon ) );

    //collect Muons
    for(std::vector<edm::InputTag>::iterator sit = sources.begin();
        sit != sources.end();
          sit++)
        {
	  TString rawtag=sit->label();
	  rawtag.ReplaceAll("pat","");
          rawtag.ReplaceAll("cleanPat","");
          rawtag.ReplaceAll("selectedPat","");
          std::string tag(rawtag);

          edm::Handle<pat::MuonCollection>imuons;
          try{
             iEvent.getByLabel( *sit, imuons);
          }catch(std::exception &e){
            continue;
          }
          if(!imuons.isValid()) continue;
          if(imuons->size() == 0)continue;

          for(size_t iMuon = 0; iMuon < imuons->size(); iMuon++)
            {
              const pat::Muon mIt = ((*imuons)[iMuon]);
              MyMuon newMuon = MyMuonConverter(mIt, rawtag);
	      newMuon.name = tag;

	      std::string labelMatcher = tag+triggerMatch;
              pat::helper::TriggerMatchHelper tmhelper;
              const pat::TriggerObjectRef objRef(tmhelper.triggerMatchObject( imuons, iMuon, labelMatcher, iEvent, *triggerEvent ));
              if(objRef.isAvailable()){newMuon.trigger_mu_pt = objRef->pt();}

	      //make selections
	      bool passKin = true, passId = true, passIso = true; 
	      if(mIt.pt() < minPt || fabs(mIt.eta()) > maxEta) passKin = false;
	      //id
	      if(mIt.muonID(id) <= 0)passId = false;
	      if(newMuon.normChi2 > maxTrackChi2)passId = false;
	      if(fabs(newMuon.D0*10000) > maxD0)passId = false;
	      //if(newMuon.inTrk_nHits < minTrackHits)passId = false;
	      //if(newMuon.nHits < minHits)passId = false;
	      if(newMuon.nMuonHits <= minMuonHits)passId = false;
	      if(newMuon.nMatchedStations < minMatchStations)passId = false;
	      if(newMuon.nPixelHits <= minPixelHits) passId = false;
	      if(newMuon.nTrackerLayers < minTrackerLayers)passId = false;
	      
	      bool isGlobal=false;
	      if(mIt.isGlobalMuon() && mIt.isTrackerMuon())isGlobal=true;
	      if(onlyGlobal && !isGlobal)passId = false;
	      //iso
	      if(newMuon.pfRelIso > maxRelIso)passIso = false;
	      int quality = 0;
	      if(passKin)quality  = 1;
	      //std::cout<<"muon quality "<<quality<<std::endl;
	      if(passId)quality |= 1<<1;
	      //std::cout<<"muon quality "<<quality<<std::endl;
	      if(passIso)quality |= 1<<2;
	      //std::cout<<"muon quality "<<quality<<std::endl;
	      newMuon.quality = quality;
	      if(passKin) selMuons.push_back(newMuon);
            }
        }
  }catch(std::exception &e){
    std::cout << "[Muon Selection] : check selection " << e.what() << std::endl;
  }
  
  return selMuons;
}
  
    
MyMuon MyEventSelection::MyMuonConverter(const pat::Muon& iMuon, TString& dirtag)
{
  MyMuon newMuon;
  newMuon.Reset();
  
  newMuon.p4.SetCoordinates(iMuon.px(), iMuon.py(), iMuon.pz(), iMuon.p());
  newMuon.vertex.SetCoordinates(iMuon.vx(), iMuon.vy(), iMuon.vz());

  newMuon.charge = iMuon.charge();
  newMuon.type = iMuon.type();

  myhistos_["pt_"+dirtag]->Fill(iMuon.pt());
  myhistos_["lowpt_"+dirtag]->Fill(iMuon.pt());
  myhistos_["eta_"+dirtag]->Fill(iMuon.eta());
  myhistos_["phi_"+dirtag]->Fill(iMuon.phi());

  const reco::GenParticle *gen = iMuon.genLepton();
  if(gen){
    newMuon.gen_id = gen->pdgId();
    if(gen->numberOfMothers() > 0)
      newMuon.gen_mother_id = gen->mother()->pdgId();
  }
  std::string labels[] = {"AllArbitrated", "AllStandAloneMuons","AllTrackerMuons", "AllGlobalMuons", 
		      "TrackerMuonArbitrated", "TMLastStationLoose","TMLastStationTight",
		      "GlobalMuonPromptTight", "GMTkChiCompatibility", "GMStaChiCompatibility", 
		      "GMTkKinkTight" };
  const size_t nlabels=sizeof(labels)/sizeof(std::string);
  int idPattern(0);
  for(size_t ibin=0; ibin < nlabels; ibin++){
    idPattern |= ((iMuon.muonID(labels[ibin]) > 0) << ibin);
    if(iMuon.muonID(labels[ibin]) > 0)myhistos_["id_"+dirtag]->Fill(ibin);
  }
  newMuon.idPattern = idPattern;
  newMuon.GlobalMuonPromptTight = (iMuon.muonID("GlobalMuonPromptTight") > 0);
  

  //newMuon.normChi2 = iMuon.normChi2();  //crashing here
  //newMuon.nHits = iMuon.numberOfValidHits();  //crashing here

  if(iMuon.isGlobalMuon()){
    const reco::TrackRef gmTrack = iMuon.globalTrack();
    if(!gmTrack.isNull()){
      newMuon.normChi2 = gmTrack->normalizedChi2();
      newMuon.nHits = gmTrack->numberOfValidHits();
      newMuon.nMuonHits = gmTrack->hitPattern().numberOfValidMuonHits();
      myhistos_["trackChi2_"+dirtag]->Fill(gmTrack->normalizedChi2());
      myhistos_["nHits_"+dirtag]->Fill(gmTrack->numberOfValidHits());
      myhistos_["nMuonHits_"+dirtag]->Fill(gmTrack->hitPattern().numberOfValidMuonHits());
    }
  }

  const reco::TrackRef mTrack = iMuon.innerTrack();
  if( !mTrack.isNull() )
    {
      newMuon.inTrk_normChi2 = mTrack->normalizedChi2();
      newMuon.inTrk_nHits = mTrack->numberOfValidHits();
      newMuon.nPixelHits = mTrack->hitPattern().numberOfValidPixelHits();
      newMuon.nTrackerLayers = mTrack->hitPattern().trackerLayersWithMeasurement();
      reco::TransientTrack transienttrack = trackBuilder->build(mTrack);
      if((&refVertex_))
	{
	  std::pair<bool,Measurement1D> d03Dmeas = IPTools::absoluteImpactParameter3D(transienttrack, refVertex_);
	  std::pair<bool,Measurement1D> d0meas = IPTools::absoluteTransverseImpactParameter(transienttrack, refVertex_);
	  newMuon.IP3D = d03Dmeas.second.value();
	  newMuon.IP3DErr = d03Dmeas.second.error();
	  newMuon.D0 = d0meas.second.value();
	  newMuon.D0Inner = d0meas.second.error();

	  myhistos_["d0_"+dirtag]->Fill(d0meas.second.value()*10000);
	}
      else{
	newMuon.D0 = mTrack->dxy();
	newMuon.D0Inner = mTrack->dxyError();

	myhistos_["d0_"+dirtag]->Fill(mTrack->dxy()*10000);
      }
    }

  newMuon.nMatchedStations = iMuon.numberOfMatchedStations();

  if(iMuon.isEnergyValid() ){
    const reco::MuonEnergy me = iMuon.calEnergy();
    newMuon.HadEnergy = me.had + me.ho;
    newMuon.EmEnergy = me.em;
  }

  //isolation
  bool isPF = false;
  if(dirtag.Contains("PFlow"))isPF = true;
  std::vector<double> iso = defaultMuonIsolation(iMuon, isPF);
  newMuon.TrkIso = iso[0];
  newMuon.ECaloIso = iso[1];
  newMuon.HCaloIso = iso[2];
  newMuon.RelIso = iso[3];

  myhistos_["reliso_"+dirtag]->Fill(iso[3]);
  myhistos_["lowreliso_"+dirtag]->Fill(iso[3]);

  //PF isolation (also for recomuon) only above 52X
  std::vector<double> pfiso = defaultPFMuonIsolation(iMuon); 
  newMuon.ChHadIso = pfiso[0]; 
  newMuon.PhotonIso = pfiso[1]; 
  newMuon.NeuHadIso = pfiso[2]; 
  newMuon.PileupIso = pfiso[3];
  newMuon.pfRelIso = pfiso[4]; 
 
  myhistos_["relpfiso_"+dirtag]->Fill(pfiso[3]); 
  myhistos_["lowrelpfiso_"+dirtag]->Fill(pfiso[3]); 

  //User PF ISo for standard PAT muon
  double pfRelIso = -999.0;
  try{
    //std::cout<<"check muon iso "<<iMuon.userIsolation("User1Iso")<<"  "<<iMuon.userIsolation("PfNeutralHadronIso")<<std::endl;
    pfRelIso = (iMuon.userIsolation("User1Iso") + std::max(0., iMuon.userIsolation("PfNeutralHadronIso") + iMuon.userIsolation("PfGammaIso") - 0.5*iMuon.userIsolation("User2Iso")))/iMuon.pt();
  }catch(std::exception &e){}
  newMuon.UserPFRelIso = pfRelIso;

  return newMuon;
}

std::vector<double> MyEventSelection::defaultMuonIsolation (const pat::Muon& muon, bool isPF)
{
  std::vector<double> values(4,0);
  double mPt((double)muon.pt());
  reco::PFCandidateRef pfRef=muon.pfCandidateRef();
  if(!pfRef.isNull()) { mPt = pfRef->pt(); }
  double norm=std::max((double)20.0,(double)mPt);

  if(isPF)
    {
      double puOffsetCorrection = 0;
      values[0] = muon.chargedHadronIso();
      values[1] = muon.photonIso();
      values[2] = muon.neutralHadronIso();
      values[3] = (std::max(muon.photonIso()+muon.neutralHadronIso() - puOffsetCorrection, 0.0) + muon.chargedHadronIso())/norm;
    }
  else{
    values[0] = muon.trackIso();
    values[1] = muon.ecalIso();
    values[2] = muon.hcalIso();
    values[3] = (muon.trackIso()+muon.ecalIso()+muon.hcalIso())/norm;
  }
  return values;
}

std::vector<double> MyEventSelection::defaultPFMuonIsolation (const pat::Muon& muon) 
{ 
  std::vector<double> values(5,0); 
  values[0] = muon.pfIsolationR04().sumChargedHadronPt; 
  values[1] = muon.pfIsolationR04().sumPhotonEt; 
  values[2] = muon.pfIsolationR04().sumNeutralHadronEt;
  values[3] = muon.pfIsolationR04().sumPUPt;
  values[4] = (muon.pfIsolationR04().sumChargedHadronPt + std::max(0., muon.pfIsolationR04().sumNeutralHadronEt+muon.pfIsolationR04().sumPhotonEt-0.5*muon.pfIsolationR04().sumPUPt))/muon.pt();

  return values;
}
