#include "MiniTree/Selection/interface/MyEventSelection.h"

std::vector<MyKineFitParticle> MyEventSelection::getKineFitParticles(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
  std::vector<MyKineFitParticle> selKFParticles; 
  selKFParticles.clear();
  
  try{
    //config parameters
    std::vector<edm::InputTag> sources = configParamsKFPs_.getParameter<std::vector<edm::InputTag> >("sources");
    edm::InputTag chi2OfFit = configParamsKFPs_.getParameter<edm::InputTag>("chi2OfFit");
    edm::InputTag statusOfFit = configParamsKFPs_.getParameter<edm::InputTag>("statusOfFit");
    edm::InputTag probOfFit = configParamsKFPs_.getParameter<edm::InputTag>("probOfFit");
    edm::InputTag njetsOfFit = configParamsKFPs_.getParameter<edm::InputTag>("njetsUsed");
    edm::InputTag chi2OfFitUp = configParamsKFPs_.getParameter<edm::InputTag>("chi2OfFitUp"); 
    edm::InputTag statusOfFitUp = configParamsKFPs_.getParameter<edm::InputTag>("statusOfFitUp"); 
    edm::InputTag probOfFitUp = configParamsKFPs_.getParameter<edm::InputTag>("probOfFitUp"); 
    edm::InputTag njetsOfFitUp = configParamsKFPs_.getParameter<edm::InputTag>("njetsUsedUp"); 
    edm::InputTag chi2OfFitDown = configParamsKFPs_.getParameter<edm::InputTag>("chi2OfFitDown");  
    edm::InputTag statusOfFitDown = configParamsKFPs_.getParameter<edm::InputTag>("statusOfFitDown");  
    edm::InputTag probOfFitDown = configParamsKFPs_.getParameter<edm::InputTag>("probOfFitDown");  
    edm::InputTag njetsOfFitDown = configParamsKFPs_.getParameter<edm::InputTag>("njetsUsedDown");  
    edm::InputTag chi2OfFitJERUp = configParamsKFPs_.getParameter<edm::InputTag>("chi2OfFitJERUp");  
    edm::InputTag statusOfFitJERUp = configParamsKFPs_.getParameter<edm::InputTag>("statusOfFitJERUp");  
    edm::InputTag probOfFitJERUp = configParamsKFPs_.getParameter<edm::InputTag>("probOfFitJERUp");  
    edm::InputTag njetsOfFitJERUp = configParamsKFPs_.getParameter<edm::InputTag>("njetsUsedJERUp");  
    edm::InputTag chi2OfFitJERDown = configParamsKFPs_.getParameter<edm::InputTag>("chi2OfFitJERDown");   
    edm::InputTag statusOfFitJERDown = configParamsKFPs_.getParameter<edm::InputTag>("statusOfFitJERDown");   
    edm::InputTag probOfFitJERDown = configParamsKFPs_.getParameter<edm::InputTag>("probOfFitJERDown");   
    edm::InputTag njetsOfFitJERDown = configParamsKFPs_.getParameter<edm::InputTag>("njetsUsedJERDown");

    edm::Handle<vector<double> >chi2_;
    edm::Handle<vector<int> >status_;
    edm::Handle<vector<double> >prob_;
    edm::Handle<int> njets_;
    edm::Handle<vector<double> >chi2Up_; 
    edm::Handle<vector<int> >statusUp_; 
    edm::Handle<vector<double> >probUp_; 
    edm::Handle<int> njetsUp_; 
    edm::Handle<vector<double> >chi2Down_;  
    edm::Handle<vector<int> >statusDown_;  
    edm::Handle<vector<double> >probDown_;  
    edm::Handle<int> njetsDown_;  
    edm::Handle<vector<double> >chi2JerUp_;  
    edm::Handle<vector<int> >statusJerUp_;  
    edm::Handle<vector<double> >probJerUp_;  
    edm::Handle<int> njetsJerUp_;  
    edm::Handle<vector<double> >chi2JerDown_;   
    edm::Handle<vector<int> >statusJerDown_;   
    edm::Handle<vector<double> >probJerDown_;   
    edm::Handle<int> njetsJerDown_;
    
    try{
      iEvent.getByLabel( chi2OfFit, chi2_);
      iEvent.getByLabel(statusOfFit, status_);
      iEvent.getByLabel(probOfFit, prob_);
      iEvent.getByLabel(njetsOfFit, njets_);
    }catch(std::exception &e){
      std::cout<<" KineFitter product is not available"<<std::endl;
    }
    try{ 
      iEvent.getByLabel( chi2OfFitUp, chi2Up_); 
      iEvent.getByLabel(statusOfFitUp, statusUp_); 
      iEvent.getByLabel(probOfFitUp, probUp_); 
      iEvent.getByLabel(njetsOfFitUp, njetsUp_); 
    }catch(std::exception &e){ 
      std::cout<<" KineFitter product for JES Up is not available"<<std::endl; 
    } 
    try{  
      iEvent.getByLabel( chi2OfFitDown, chi2Down_);  
      iEvent.getByLabel(statusOfFitDown, statusDown_);  
      iEvent.getByLabel(probOfFitDown, probDown_);  
      iEvent.getByLabel(njetsOfFitDown, njetsDown_);  
    }catch(std::exception &e){  
      std::cout<<" KineFitter product for JES Down is not available"<<std::endl;  
    }  
    try{  
      iEvent.getByLabel( chi2OfFitJERUp, chi2JerUp_);  
      iEvent.getByLabel(statusOfFitJERUp, statusJerUp_);  
      iEvent.getByLabel(probOfFitJERUp, probJerUp_);  
      iEvent.getByLabel(njetsOfFitJERUp, njetsJerUp_);  
    }catch(std::exception &e){  
      std::cout<<" KineFitter product for JER Up is not available"<<std::endl;  
    }  
    try{   
      iEvent.getByLabel( chi2OfFitJERDown, chi2JerDown_);   
      iEvent.getByLabel(statusOfFitJERDown, statusJerDown_);   
      iEvent.getByLabel(probOfFitJERDown, probJerDown_);   
      iEvent.getByLabel(njetsOfFitJERDown, njetsJerDown_);   
    }catch(std::exception &e){   
      std::cout<<" KineFitter product for JER Down is not available"<<std::endl;   
    }

    for(std::vector<edm::InputTag>::iterator sit = sources.begin();
	sit != sources.end();
	sit++)
      {
	TString rawtag=sit->instance();
	std::string tag(rawtag);
	TString label = sit->label();
	std::string moduleLabel(label);
	
	edm::Handle<pat::ParticleCollection>ikfps;
	try{
	  iEvent.getByLabel( *sit, ikfps);
	}catch(std::exception &e){
	  continue;
	}
	//cout<<" size "<<moduleLabel<<":"<<tag<<" "<<ikfps->size()<<endl;
	if(!ikfps.isValid()) continue;
	if(ikfps->size() == 0)continue;
	for(size_t iKfp = 0; iKfp < ikfps->size(); iKfp++)
	  {
	    const pat::Particle jKfp = ((*ikfps)[iKfp]);
	    MyKineFitParticle newKfp = MyKineFitPartConverter(jKfp, rawtag);
	    newKfp.partName = tag;
	    newKfp.labelName = moduleLabel;
	    if(moduleLabel.find("JESUp")!=std::string::npos){
	      newKfp.chi2OfFit = chi2Up_->size()>0 ? (*chi2Up_)[0] : 999.; 
              newKfp.statusOfFit = statusUp_->size()>0 ? (*statusUp_)[0] : 0; 
              newKfp.probOfFit = probUp_->size() > 0 ? (*probUp_)[0] : 0; 
              newKfp.njetsOfFit = *njetsUp_; 
	      //std::cout<<" JES UP chi2 "<<newKfp.chi2OfFit<<std::endl;
	    }
	    else if(moduleLabel.find("JESDown")!=std::string::npos){ 
	      newKfp.chi2OfFit = chi2Down_->size()>0 ? (*chi2Down_)[0] : 999.;  
              newKfp.statusOfFit = statusDown_->size()>0 ? (*statusDown_)[0] : 0;  
              newKfp.probOfFit = probDown_->size() > 0 ? (*probDown_)[0] : 0;  
              newKfp.njetsOfFit = *njetsDown_;  
	      //std::cout<<" JES Down chi2 "<<newKfp.chi2OfFit<<std::endl;
            }
	    else if(moduleLabel.find("JERUp")!=std::string::npos){ 
              newKfp.chi2OfFit = chi2JerUp_->size()>0 ? (*chi2JerUp_)[0] : 999.;  
              newKfp.statusOfFit = statusJerUp_->size()>0 ? (*statusJerUp_)[0] : 0;  
              newKfp.probOfFit = probJerUp_->size() > 0 ? (*probJerUp_)[0] : 0;  
              newKfp.njetsOfFit = *njetsJerUp_;  
	      //std::cout<<" JER UP chi2 "<<newKfp.chi2OfFit<<std::endl; 
            } 
            else if(moduleLabel.find("JERDown")!=std::string::npos){  
              newKfp.chi2OfFit = chi2JerDown_->size()>0 ? (*chi2JerDown_)[0] : 999.;   
              newKfp.statusOfFit = statusJerDown_->size()>0 ? (*statusJerDown_)[0] : 0;   
              newKfp.probOfFit = probJerDown_->size() > 0 ? (*probJerDown_)[0] : 0;   
              newKfp.njetsOfFit = *njetsJerDown_;   
	      //std::cout<<" JER Down chi2 "<<newKfp.chi2OfFit<<std::endl; 
            }
	    else{
	      newKfp.chi2OfFit = chi2_->size()>0 ? (*chi2_)[0] : 999.;
	      newKfp.statusOfFit = status_->size()>0 ? (*status_)[0] : 0;
	      newKfp.probOfFit = prob_->size() > 0 ? (*prob_)[0] : 0;
	      newKfp.njetsOfFit = *njets_;
	    }
	    selKFParticles.push_back(newKfp);
	  }
	fs_->cd();
      }
  }catch(std::exception &e){
    std::cout << "[KineFitParticle Collection] : check selection " << e.what() << std::endl;
  }
  
  return selKFParticles;
}
  
    
MyKineFitParticle MyEventSelection::MyKineFitPartConverter(const pat::Particle& ikfp, TString& dirtag)
{
  MyKineFitParticle newKFP;
  newKFP.Reset();
  
  newKFP.p4.SetCoordinates(ikfp.px(), ikfp.py(), ikfp.pz(), ikfp.energy());
  newKFP.vertex.SetCoordinates(ikfp.vx(), ikfp.vy(), ikfp.vz());
  
  //newKFP.part_id = ikfp.pid();
  //newKFP.part_mother_id = ikfp.motherID();
  newKFP.charge = ikfp.charge();


  return newKFP;
}

