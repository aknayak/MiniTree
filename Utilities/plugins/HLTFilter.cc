// -*- C++ -*-
//
// Package:    HLTFilter
// Class:      HLTFilter
// 
/**\class HLTFilter HLTFilter.cc HLTMenuCheck/HLTFilter/src/HLTFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Christian veelken
//         Created:  Mon Oct  1 19:48:16 CEST 2012
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
//
// class declaration
//
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

class HLTFilter : public edm::EDAnalyzer {
   public:
      explicit HLTFilter(const edm::ParameterSet&);
      ~HLTFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      // ----------member data ---------------------------
  edm::InputTag hlt_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HLTFilter::HLTFilter(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
  hlt_ = iConfig.getParameter<edm::InputTag>("hltmenu");
}


HLTFilter::~HLTFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
HLTFilter::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;

   edm::Handle<edm::TriggerResults> hltresults;
   iEvent.getByLabel(hlt_,hltresults);
   const edm::TriggerNames& TrigNames_ = iEvent.triggerNames(*hltresults);
   const int ntrigs = hltresults->size();

   for(int itrig=0; itrig<ntrigs; itrig++)
     {
       std::string trigName = TrigNames_.triggerName(itrig);
       cout<<trigName.c_str()<<endl; 
     }
   

}


// ------------ method called once each job just before starting event loop  ------------
void 
HLTFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HLTFilter::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
HLTFilter::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
HLTFilter::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
HLTFilter::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
HLTFilter::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HLTFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HLTFilter);
