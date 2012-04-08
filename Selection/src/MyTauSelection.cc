#include "MiniTree/Selection/interface/MyEventSelection.h"

std::vector<MyTau> MyEventSelection::getTaus(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  
  std::vector<MyTau> selTaus; 
  selTaus.clear();
  
  try{
    //config parameters
    std::vector<edm::InputTag> sources = configParamsTaus_.getParameter<std::vector<edm::InputTag> >("sources");
    double minPt = configParamsTaus_.getParameter<double>("minPt");
    double maxEta = configParamsTaus_.getParameter<double>("maxEta");

    //collect Taus
    for(std::vector<edm::InputTag>::iterator sit = sources.begin();
        sit != sources.end();
          sit++)
        {
	  TString rawtag=sit->label();
          rawtag.ReplaceAll("pat","");
          rawtag.ReplaceAll("cleanPat","");
          rawtag.ReplaceAll("selectedPat","");
          std::string tag(rawtag);
	  
          edm::Handle<pat::TauCollection>itaus;
          try{
             iEvent.getByLabel( *sit, itaus);
          }catch(std::exception &e){
            continue;
          }
          if(!itaus.isValid()) continue;
          if(itaus->size() == 0)continue;

          for(size_t iTau = 0; iTau < itaus->size(); iTau++)
            {
              const pat::Tau tIt = ((*itaus)[iTau]);
              MyTau newTau = MyTauConverter(tIt, rawtag);
	      newTau.tauAlgo = tag;
	      
	      //make selection
	      if(newTau.decayModeFinding > 0.5 && tIt.pt() >= minPt && 
		 fabs(tIt.eta()) <= maxEta){
		selTaus.push_back(newTau);
	      }
            }
        }
  }catch(std::exception &e){
    std::cout << "[Tau Selection] : check selection " << e.what() << std::endl;
  }
  
  return selTaus;
}
  
    
MyTau MyEventSelection::MyTauConverter(const pat::Tau& iTau, TString& dirtag)
{
  MyTau newTau;
  
  newTau.p4.SetCoordinates(iTau.px(), iTau.py(), iTau.pz(), iTau.p());
  newTau.vertex.SetCoordinates(iTau.vx(), iTau.vy(), iTau.vz());
  
  myhistos_["pt_"+dirtag]->Fill(iTau.pt());
  myhistos_["lowpt_"+dirtag]->Fill(iTau.pt());
  myhistos_["eta_"+dirtag]->Fill(iTau.eta());
  myhistos_["phi_"+dirtag]->Fill(iTau.phi());
  
  newTau.decayModeFinding = iTau.tauID("decayModeFinding");
  newTau.LooseIsolation = iTau.tauID("byLooseIsolation");
  newTau.LooseIsolationDeltaBetaCorr = iTau.tauID("byLooseIsolationDeltaBetaCorr");
  newTau.LooseCombinedIsolationDeltaBetaCorr = iTau.tauID("byLooseCombinedIsolationDeltaBetaCorr");
  newTau.MediumIsolation = iTau.tauID("byMediumIsolation");
  newTau.MediumIsolationDeltaBetaCorr = iTau.tauID("byMediumIsolationDeltaBetaCorr");
  newTau.MediumCombinedIsolationDeltaBetaCorr = iTau.tauID("byMediumCombinedIsolationDeltaBetaCorr");
  newTau.TightIsolation = iTau.tauID("byTightIsolation");
  newTau.TightIsolationDeltaBetaCorr = iTau.tauID("byTightIsolationDeltaBetaCorr");
  newTau.TightCombinedIsolationDeltaBetaCorr = iTau.tauID("byTightCombinedIsolationDeltaBetaCorr");
  newTau.againstElectronLoose = iTau.tauID("againstElectronLoose");
  newTau.againstElectronMedium = iTau.tauID("againstElectronMedium");
  newTau.againstElectronTight = iTau.tauID("againstElectronTight");
  newTau.againstElectronMVA = iTau.tauID("againstElectronMVA");
  newTau.againstMuonLoose = iTau.tauID("againstMuonLoose");
  newTau.againstMuonMedium = iTau.tauID("againstMuonMedium");
  newTau.againstMuonTight = iTau.tauID("againstMuonTight");
  newTau.decayMode = iTau.decayMode();

  newTau.gen_id = 0;
  newTau.gen_mother_id = 0;
  newTau.nSignalChargeHadron = iTau.signalPFChargedHadrCands().size();
  if(iTau.signalPFChargedHadrCands().size() > 0){
    newTau.leadChargedHadronPt = iTau.leadPFChargedHadrCand()->pt();
    newTau.leadChargedHadronP = iTau.leadPFChargedHadrCand()->p();
  }
  if(iTau.signalPFCands().size() > 0){
    newTau.leadingParticlePt = iTau.leadPFCand()->pt();
    newTau.leadingParticleP = iTau.leadPFCand()->p();
  }
  newTau.tauEMF = iTau.emFraction();

  //get tau charge and track d0
  double tauCharge(0);
  double d0_max(0), d0_max_err(0);
  for(reco::PFCandidateRefVector::const_iterator pfCand = iTau.signalPFChargedHadrCands().begin(); pfCand != iTau.signalPFChargedHadrCands().end(); pfCand++){
    double charge(0);
    if( (*pfCand)->charge() > 0){ charge = 1; } // is tau charge given as int? to be checked... 
    else  { charge =-1; }
    tauCharge += charge;
    reco::TrackRef PFChargedHadrCand_rectk = (**pfCand).trackRef();
    if(!PFChargedHadrCand_rectk)continue;
    double d0 =  fabs( ( PFChargedHadrCand_rectk->dxy( refVertex_.position() ) ) * 10000. );
    if(fabs(d0)>fabs(d0_max)){d0_max=d0; d0_max_err=PFChargedHadrCand_rectk->dxyError()*10000.;}
  }

  newTau.charge = tauCharge;
  newTau.d0_max = d0_max;
  newTau.d0_max_err = d0_max_err;
  
  newTau.matchJetPt = iTau.pfJetRef()->pt();
  newTau.matchJetEta = iTau.pfJetRef()->eta();
  newTau.matchJetPhi = iTau.pfJetRef()->phi();
  newTau.matchJetEMF = (iTau.pfJetRef()->chargedEmEnergyFraction() + iTau.pfJetRef()->neutralEmEnergyFraction());

  return newTau;
}
