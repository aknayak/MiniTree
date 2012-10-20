#include "MiniTree/Selection/interface/MyEventSelection.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "CMGTools/External/interface/PileupJetIdentifier.h"

std::vector<MyJet> MyEventSelection::getJets(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  
  std::vector<MyJet> selJets; 
  selJets.clear();
  
  jetIDFunctor_ = JetIDSelectionFunctor(configParamsJets_.getParameter<edm::ParameterSet>("CaloJetId") );
  pfjetIDFunctor_ = PFJetIDSelectionFunctor(configParamsJets_.getParameter<edm::ParameterSet>("PFJetId") );

  //edm::Handle<edm::View< reco::CaloJet > > hJets; 
  //iEvent.getByLabel("ak5CaloJets", hJets );
  iEvent.getByLabel( "ak5JetID", hJetIDMap );
  
  try{
    //config parameters
    std::vector<edm::InputTag> sources = configParamsJets_.getParameter<std::vector<edm::InputTag> >("sources");
    bool useRawJets = configParamsJets_.getParameter<bool>("useRawJets");
    double minPt = configParamsJets_.getParameter<double>("minPt");
    double maxEta = configParamsJets_.getParameter<double>("maxEta");
    double minDeltaRtoLepton = configParamsJets_.getParameter<double>("minDeltaRtoLepton");
    std::string triggerMatch = configParamsJets_.getParameter<std::string>("triggerMatch");
    
    edm::Handle< pat::TriggerEvent > triggerEvent;
    iEvent.getByLabel( configParamsJets_.getParameter<edm::InputTag>("triggerEvent"), triggerEvent );
    //const pat::TriggerObjectRefVector triggerJets( triggerEvent->objects( trigger::TriggerJet ) );
    
    edm::InputTag puMVADiscriminant = configParamsJets_.getParameter<edm::InputTag>("puMVADiscriminant");
    edm::InputTag puMVAID = configParamsJets_.getParameter<edm::InputTag>("puMVAID");

    //PUJetID MVA
    Handle<ValueMap<float> > puJetIdMVA;
    iEvent.getByLabel(puMVADiscriminant, puJetIdMVA);

    Handle<ValueMap<int> > puJetIdFlag;
    iEvent.getByLabel(puMVAID, puJetIdFlag);

    //collect Jets
    for(std::vector<edm::InputTag>::iterator sit = sources.begin();
	sit != sources.end();
          sit++)
        {
	  TString rawtag=sit->label();
	  rawtag.ReplaceAll("pat","");
	  rawtag.ReplaceAll("cleanPat","");
	  rawtag.ReplaceAll("selectedPat","");
	  std::string tag(rawtag);
	  
	  edm::Handle<pat::JetCollection>ijets;
	  try{
	     iEvent.getByLabel( *sit, ijets);
	  }catch(std::exception &e){
	    continue;
	  }
	  if(!ijets.isValid()) continue;
	  if(ijets->size() == 0)continue;
	  
	  edm::Handle<edm::View<pat::Jet> > jetsHandleForMVA;
	  iEvent.getByLabel(*sit,jetsHandleForMVA);

	  //add to extract JES uncertainty from CondDB
          // handle the jet corrector parameters collection
          edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
          //get the jet corrector parameters collection from the globaltag
	  std::string uncType("PF");
          if(rawtag.Contains("JPT") ) uncType="JPT";
          else if(rawtag.Contains("Calo") ) uncType="Calo";
          else if(rawtag.Contains("TRK") ) uncType="TRK";
          iSetup.get<JetCorrectionsRecord>().get("AK5"+uncType,JetCorParColl);
          // get the uncertainty parameters from the collection
          JetCorrectorParameters const & JetCorPar = (*JetCorParColl)["Uncertainty"]; 
          // instantiate the jec uncertainty object
          JetCorrectionUncertainty *jecUnc = new JetCorrectionUncertainty(JetCorPar); 
	  

	  //get the tau collection and pass it for required information
	  TString tautag("patTaus");  
	  if(rawtag.Contains("PFlow") )tautag = TString("patTausPFlow"); 
	  
	  //std::cout<<" tau tag for Jet "<<tautag.Data()<<std::endl; 
	  const std::vector<pat::Tau> *tauColl = 0;
	  edm::Handle<pat::TauCollection>itaus; 
	  try{ 
	    iEvent.getByLabel( tautag.Data(), itaus); 
	  }catch(std::exception &e){ } 
	  if(itaus.isValid()){
	    tauColl = itaus.product();
	  }

	  for(size_t iJet = 0; iJet < ijets->size(); iJet++)
	    {
	      const pat::Jet jIt = ((*ijets)[iJet]);

	      if(jIt.pt() < 15 || fabs(jIt.eta()) > maxEta)continue;
	      
	      MyJet newJet = MyJetConverter(jIt, rawtag, tauColl);
	      newJet.jetName = tag;
	      
	      
	      //JEC uncertainty
	      jecUnc->setJetEta(jIt.eta());
              jecUnc->setJetPt(jIt.pt());  
	      float JECUncertainty = jecUnc->getUncertainty(true);
	      newJet.JECUncertainty = JECUncertainty;
	      
	      std::string labelMatcher = tag+triggerMatch;
              //std::string labelMatcher("JetsAK5PFTrigMatch");
              pat::helper::TriggerMatchHelper tmhelper;
              const pat::TriggerObjectRef objRef(tmhelper.triggerMatchObject( ijets, iJet, labelMatcher, iEvent, *triggerEvent ));
              if(objRef.isAvailable()){newJet.triggerJet_pt = objRef->pt();}
	      
	      //store PU JetID only for PAT PF Jets
	      if(rawtag.Contains("PFlow") || rawtag.Contains("JPT") || 
		 rawtag.Contains("Calo")){
		newJet.puIDMVALoose = 0;   
		newJet.puIDMVAMedium = 0;   
		newJet.puIDMVATight = 0;   
		newJet.puIDMVADiscr = 0; 
	      }
	      else{
		int    idflag = (*puJetIdFlag)[jetsHandleForMVA->refAt(iJet)];
		newJet.puIDMVALoose = PileupJetIdentifier::passJetId( idflag, PileupJetIdentifier::kLoose);    
		newJet.puIDMVAMedium = PileupJetIdentifier::passJetId( idflag, PileupJetIdentifier::kMedium);    
                newJet.puIDMVATight = PileupJetIdentifier::passJetId( idflag, PileupJetIdentifier::kTight );
                newJet.puIDMVADiscr = (*puJetIdMVA)[jetsHandleForMVA->refAt(iJet)];
	      }
	      
	      //make selections
	      bool passKin = true, passId = true, passIso = true;
	      if(jIt.pt() < minPt || fabs(jIt.eta()) > maxEta)passKin = false;
	      //id
	      if(!(newJet.jetIDLoose))passId = false;
	      
	      int quality = 0;
	      if(passKin)quality  = 1;
              //std::cout<<"jet quality "<<quality<<std::endl;
              if(passId)quality |= 1<<1;
              //std::cout<<"jet quality "<<quality<<std::endl;
              if(passIso)quality |= 1<<2;
              //std::cout<<"jet quality "<<quality<<std::endl;
              newJet.quality = quality;
	      
	      if(passKin && passId) selJets.push_back(newJet);
	    }
	  fs_->cd();
	}
  }catch(std::exception &e){
    std::cout << "[Jet Selection] : check selection " << e.what() << std::endl;
  }
  
  return selJets;
}
  
    
MyJet MyEventSelection::MyJetConverter(const pat::Jet& iJet, TString& dirtag, const std::vector<pat::Tau> *tauColl)
{
  MyJet newJet;
  newJet.Reset();
  
  pat::strbitset jetid = jetIDFunctor_.getBitTemplate();
  pat::strbitset pfjetid = pfjetIDFunctor_.getBitTemplate();

  newJet.p4.SetCoordinates(iJet.px(), iJet.py(), iJet.pz(), iJet.energy());
  newJet.vertex.SetCoordinates(iJet.vx(), iJet.vy(), iJet.vz());
  
  //myhistos_["pfJetPt"]->Fill(iJet.pt());
  myhistos_["pt_"+dirtag]->Fill(iJet.pt());
  myhistos_["lowpt_"+dirtag]->Fill(iJet.pt());
  myhistos_["eta_"+dirtag]->Fill(iJet.eta());
  myhistos_["phi_"+dirtag]->Fill(iJet.phi());
  
  //MC truth
  const reco::Candidate *genParton = iJet.genParton();
  newJet.parton_id = (genParton != 0 ? genParton->pdgId() : 0 );
  if(genParton){
    if(genParton->numberOfMothers() > 0){
     newJet.parton_mother_id = genParton->mother()->pdgId();
    }
  }
  newJet.partonFlavour = double(iJet.partonFlavour());
  newJet.jetCharge = iJet.jetCharge();
  newJet.etaetaMoment = iJet.etaetaMoment();
  newJet.phiphiMoment = iJet.phiphiMoment();
  
  //JECs
  std::map<std::string, double>jetCorrections; jetCorrections.clear();
  const std::vector<std::string> jeclevels = iJet.availableJECLevels();
  for(size_t j = 0; j < jeclevels.size(); j++){
    std::string levelName = jeclevels[j];
    if(levelName.find("L5Flavor") != std::string::npos ||
       levelName.find("L7Parton") != std::string::npos ){
      jetCorrections[levelName] = iJet.jecFactor(levelName, "bottom");
    }
    else{ jetCorrections[levelName] = iJet.jecFactor(levelName); }
  }
  newJet.JECs = jetCorrections;
  newJet.JECUncertainty = 1.0;  //default, get it later from CondDB.

  //b-tags
  std::map<std::string, double> discr; discr.clear();
  discr["trackCountingHighEffBJetTags"] = iJet.bDiscriminator("trackCountingHighEffBJetTags");
  discr["trackCountingHighPurBJetTags"] = iJet.bDiscriminator("trackCountingHighPurBJetTags");
  discr["jetProbabilityBJetTags"] = iJet.bDiscriminator("jetProbabilityBJetTags");   
  discr["jetBProbabilityBJetTags"] = iJet.bDiscriminator("jetBProbabilityBJetTags");
  discr["combinedSecondaryVertexBJetTags"] = iJet.bDiscriminator("combinedSecondaryVertexBJetTags");
  discr["simpleSecondaryVertexHighEffBJetTags"] = iJet.bDiscriminator("simpleSecondaryVertexHighEffBJetTags");
  discr["simpleSecondaryVertexHighPurBJetTags"] = iJet.bDiscriminator("simpleSecondaryVertexHighPurBJetTags");

  newJet.bDiscriminator = discr;
  
  //jet id
  if(iJet.isPFJet())
    {     
      newJet.CHEF = iJet.chargedHadronEnergyFraction();
      newJet.NHEF = iJet.neutralHadronEnergyFraction();
      newJet.CEEF = iJet.chargedEmEnergyFraction();
      newJet.NEEF = iJet.neutralEmEnergyFraction();
      newJet.emEnergyFraction = (iJet.chargedEmEnergyFraction() + iJet.neutralEmEnergyFraction());
      newJet.hadEnergyFraction = ( iJet.chargedHadronEnergy() + iJet.neutralHadronEnergy() + iJet.HFHadronEnergy() ) / iJet.energy();
      newJet.chargeMultiplicity = iJet.chargedMultiplicity();
      newJet.NumberOfDoughters = iJet.numberOfDaughters();

      pfjetid.set(false);
      newJet.jetIDLoose = pfjetIDFunctor_( iJet, pfjetid );

      myhistos_["emf_"+dirtag]->Fill(iJet.chargedEmEnergyFraction() + iJet.neutralEmEnergyFraction());
    }
  else if(iJet.isCaloJet() )
    {
      const reco::JetID &id = iJet.jetID();
      newJet.fRBX = id.fRBX;
      newJet.RestrictedEMF = id.restrictedEMF;
      newJet.nHCALTowers = id.nHCALTowers;
      newJet.nECALTowers = id.nECALTowers;
      newJet.fHPD = id.fHPD;
      newJet.n90Hits = id.n90Hits;
      newJet.emEnergyFraction = iJet.emEnergyFraction();

      jetid.set(false);
      newJet.jetIDLoose = jetIDFunctor_( iJet, jetid );

      myhistos_["emf_"+dirtag]->Fill(iJet.emEnergyFraction());
    }
  else if(iJet.isJPTJet() )
    {
      const pat::JPTSpecific &jptspec = iJet.jptSpecific();
      newJet.CHEF = jptspec.mChargedHadronEnergy/iJet.energy();
      newJet.NHEF = jptspec.mNeutralHadronEnergy/iJet.energy();
      newJet.CEEF = jptspec.mChargedEmEnergy/iJet.energy();
      newJet.NEEF = jptspec.mNeutralEmEnergy/iJet.energy();

      const reco::JPTJet &jptJet = dynamic_cast<const reco::JPTJet &>( *(iJet.originalObjectRef()));
      const edm::RefToBase<reco::Jet> &origJetRef = jptJet.getCaloJetRef();
      const reco::CaloJet &caloJet = dynamic_cast<const reco::CaloJet &>( *origJetRef );
      reco::JetID const & jetId = (*hJetIDMap)[ origJetRef ];
      jetid.set(false);
      newJet.jetIDLoose = jetIDFunctor_( caloJet, jetId, jetid );
      
      myhistos_["emf_"+dirtag]->Fill((jptspec.mChargedEmEnergy+jptspec.mNeutralEmEnergy)/iJet.energy());
    }


  //leading track
  const reco::TrackRefVector &tracks = iJet.associatedTracks();
  newJet.nTracks = tracks.size();
  for(reco::TrackRefVector::const_iterator tIt = tracks.begin();
      tIt != tracks.end();
      tIt++)
    {
      if( tIt->get()->pt() < newJet.lead_track_pt ) continue;
      newJet.lead_track_charge=tIt->get()->charge();
      newJet.lead_track_pt=tIt->get()->pt();
      reco::TransientTrack tt = trackBuilder->build(*tIt);
      if((&refVertex_))
	{
	  std::pair<bool,Measurement1D> d03dmeas = IPTools::absoluteImpactParameter3D(tt, refVertex_);
	  newJet.lead_track_d0 = d03dmeas.second.value();
	  newJet.lead_track_d0_Significance = d03dmeas.second.significance();
	}
    }

  myhistos_["ntracks_"+dirtag]->Fill(tracks.size());
  myhistos_["nconstituents_"+dirtag]->Fill(iJet.numberOfDaughters());

  //Match to a reco tau and get AgainstLepton Discriminators, needed for tau fake rate studies
  if(iJet.isPFJet()){
    pat::Tau *matchTau = getTauMatchedtoJet(iJet, tauColl); 
    if(matchTau != 0){
      newJet.tau_vertex.SetCoordinates(matchTau->vertex().x(), matchTau->vertex().y(), matchTau->vertex().z());
      newJet.tau_againstElectronLoose = matchTau->tauID("againstElectronLoose");
      newJet.tau_againstElectronMedium = matchTau->tauID("againstElectronMedium"); 
      newJet.tau_againstElectronTight = matchTau->tauID("againstElectronTight"); 
      newJet.tau_againstElectronMVA = matchTau->tauID("againstElectronMVA"); 
      newJet.tau_againstMuonLoose = matchTau->tauID("againstMuonLoose"); 
      newJet.tau_againstMuonMedium = matchTau->tauID("againstMuonMedium"); 
      newJet.tau_againstMuonTight = matchTau->tauID("againstMuonTight"); 
      //std::cout<<" against electron Medium "<<matchTau->tauID("againstElectronMedium")<<std::endl;
    }
  } 

  return newJet;
}

pat::Tau* MyEventSelection::getTauMatchedtoJet(const pat::Jet& iJet, const std::vector<pat::Tau> *tauColl)
{
  pat::Tau* matchTau = 0;

  if(tauColl->size() == 0) return 0;
  
  if(iJet.originalObjectRef().isNonnull()){
    for(size_t iTau = 0; iTau < tauColl->size(); iTau++)
      {
	const pat::Tau tIt = ((*tauColl)[iTau]);
	if(tIt.pfJetRef()->pt() == iJet.originalObjectRef()->pt() &&
	   tIt.pfJetRef()->eta() == iJet.originalObjectRef()->eta() && 
	   tIt.pfJetRef()->phi() == iJet.originalObjectRef()->phi() ){
	  matchTau = const_cast<pat::Tau*>(&((*tauColl)[iTau]));
	  //std::cout<<"Jet finds a tau ref. pt"<<iJet.pt()<<" eta "<<iJet.eta()<<std::endl;
	  //std::cout<<"matched tau vz "<<matchTau->vz()<<" pt "<<matchTau->pt()<<" eta "<<matchTau->eta()<<std::endl;
	}
      }
  }
   
  return matchTau;
}
