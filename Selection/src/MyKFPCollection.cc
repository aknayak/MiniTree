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
    
    edm::Handle<vector<double> >chi2_;
    edm::Handle<vector<int> >status_;
    edm::Handle<vector<double> >prob_;
    edm::Handle<int> njets_;
      
    
    try{
      iEvent.getByLabel( chi2OfFit, chi2_);
      iEvent.getByLabel(statusOfFit, status_);
      iEvent.getByLabel(probOfFit, prob_);
      iEvent.getByLabel(njetsOfFit, njets_);
    }catch(std::exception &e){
      std::cout<<" KineFitter product is not available"<<std::endl;
    }
    
    for(std::vector<edm::InputTag>::iterator sit = sources.begin();
	sit != sources.end();
	sit++)
      {
	TString rawtag=sit->instance();
	std::string tag(rawtag);
	
	edm::Handle<pat::ParticleCollection>ikfps;
	try{
	  iEvent.getByLabel( *sit, ikfps);
	}catch(std::exception &e){
	  continue;
	}
	//cout<<" size "<<tag<<" "<<ikfps->size()<<endl;
	if(!ikfps.isValid()) continue;
	if(ikfps->size() == 0)continue;
	for(size_t iKfp = 0; iKfp < ikfps->size(); iKfp++)
	  {
	    const pat::Particle jKfp = ((*ikfps)[iKfp]);
	    MyKineFitParticle newKfp = MyKineFitPartConverter(jKfp, rawtag);
	    newKfp.partName = tag;
	    newKfp.chi2OfFit = chi2_->size()>0 ? (*chi2_)[0] : 999.;
	    newKfp.statusOfFit = status_->size()>0 ? (*status_)[0] : 0;
	    newKfp.probOfFit = prob_->size() > 0 ? (*prob_)[0] : 0;
	    newKfp.njetsOfFit = *njets_;
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

