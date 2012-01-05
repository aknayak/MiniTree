#include "MiniTree/Selection/interface/MyEventSelection.h"

std::vector<MyMET> MyEventSelection::getMETs(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  
  std::vector<MyMET> selMETs; 
  selMETs.clear();

  try{
    //config parameters
    std::vector<edm::InputTag> sources = configParamsMETs_.getParameter<std::vector<edm::InputTag> >("sources");
    
    //collect METs
    for(std::vector<edm::InputTag>::iterator sit = sources.begin();
        sit != sources.end();
	sit++)
      {
	
	TString rawtag=sit->label();
	rawtag.ReplaceAll("pat","");
	rawtag.ReplaceAll("cleanPat","");
	rawtag.ReplaceAll("selectedPat","");
	std::string tag(rawtag);
	
	edm::Handle<pat::METCollection>imets;
	try{
	  iEvent.getByLabel( *sit, imets);
	}catch(std::exception &e){
	  continue;
	}
	if(!imets.isValid()) continue;
	if(imets->size() == 0)continue;
	
	const pat::MET metIt = ((*imets)[0]);
	MyMET newMET = MyMETConverter(metIt, rawtag);
	newMET.metName = tag;
	selMETs.push_back(newMET);
      }
  }catch(std::exception &e){
    std::cout << "[MET Selection] : check selection " << e.what() << std::endl;
  }
  
  return selMETs;
}

  
MyMET MyEventSelection::MyMETConverter(const pat::MET& iMET, TString& dirtag)
{
  MyMET newMET;
  newMET.Reset();
  
  newMET.p4.SetCoordinates(iMET.px(), iMET.py(), 0, iMET.et());
  newMET.sumEt = iMET.sumEt();
  newMET.metSignificance = iMET.significance();
  
  myhistos_["pt_"+dirtag]->Fill(iMET.pt());
  myhistos_["sumet_"+dirtag]->Fill(iMET.sumEt());
  myhistos_["phi_"+dirtag]->Fill(iMET.phi());

  //pfMET
  if(iMET.isPFMET()){
    newMET.emEtFraction = iMET.NeutralEMFraction()+ iMET.ChargedEMEtFraction();
    newMET.hadEtFraction = iMET.NeutralHadEtFraction()+ iMET.ChargedHadEtFraction();
    newMET.muonEtFraction = iMET.MuonEtFraction();
    newMET.isPFMET = true;
  }
  else if(iMET.isCaloMET()){
    newMET.emEtFraction = iMET.emEtFraction();
    newMET.hadEtFraction = iMET.etFractionHadronic();
    newMET.metSignificance = iMET.metSignificance();
    newMET.isCaloMET = true;
  }
  return newMET;
}
