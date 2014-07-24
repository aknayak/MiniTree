#ifndef LLRAnalysis_TauTauStudies_ElectronsUserEmbedded_h
#define LLRAnalysis_TauTauStudies_ElectronsUserEmbedded_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "DataFormats/RecoCandidate/interface/IsoDepositVetos.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"

#include "EgammaAnalysis/ElectronTools/interface/EGammaMvaEleEstimator.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/TransientTrack/plugins/TransientTrackBuilderESProducer.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "RecoEgamma/EgammaTools/interface/ConversionFinder.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/GeometryVector/interface/VectorUtil.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "EgammaAnalysis/ElectronTools/interface/ElectronEffectiveArea.h"

using namespace edm;
using namespace std;
using namespace reco;

class ElectronsUserEmbedded : public edm::EDProducer{


public: 

  explicit ElectronsUserEmbedded(const edm::ParameterSet&);
  virtual ~ElectronsUserEmbedded() {};

private:

  virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);


  edm::InputTag electronTag_;
  edm::InputTag vertexTag_;
  edm::InputTag rhoProducer_;
  //bool isMC_;
};

#endif

ElectronsUserEmbedded::ElectronsUserEmbedded(const edm::ParameterSet & iConfig){

  electronTag_ = iConfig.getParameter<edm::InputTag>("electronTag");
  vertexTag_   = iConfig.getParameter<edm::InputTag>("vertexTag");
  rhoProducer_ = iConfig.getParameter<edm::InputTag>("rho");
  //isMC_        = iConfig.getParameter<bool>("isMC");
  
  produces<pat::ElectronCollection>("");
}

void ElectronsUserEmbedded::produce(edm::Event & iEvent, const edm::EventSetup & iSetup){

  edm::Handle<pat::ElectronCollection> electronsHandle;
  iEvent.getByLabel(electronTag_,electronsHandle);
  const pat::ElectronCollection* electrons = electronsHandle.product();

  //edm::Handle<reco::GsfElectronCollection> gsfElectronsHandle;
  //iEvent.getByLabel("gsfElectrons",gsfElectronsHandle);
  //const reco::GsfElectronCollection* gsfElectrons = gsfElectronsHandle.product();

  edm::Handle<reco::VertexCollection> vertexHandle;
  iEvent.getByLabel(vertexTag_,vertexHandle);
  const reco::VertexCollection* vertexes = vertexHandle.product();

  edm::Handle<reco::BeamSpot> bsHandle;
  iEvent.getByLabel("offlineBeamSpot", bsHandle);
  const reco::BeamSpot &thebs = *bsHandle.product();

  //pileup
  edm::Handle<double> hRho;
  iEvent.getByLabel(rhoProducer_, hRho);
  double rho_ = *hRho;

  edm::Handle<reco::ConversionCollection> hConversions;
  iEvent.getByLabel("allConversions", hConversions);

  std::auto_ptr< pat::ElectronCollection > electronsUserEmbeddedColl( new pat::ElectronCollection() ) ;

  for(unsigned int i = 0; i < electrons->size(); i++){

    pat::Electron aElectron( (*electrons)[i] );
    //const reco::GsfElectron* aGsf = static_cast<reco::GsfElectron*>(&aElectron);

    const reco::Track *el_track = (const reco::Track*)((aElectron).gsfTrack().get());  
    const reco::HitPattern& p_inner = el_track->trackerExpectedHitsInner(); 
    float nHits = p_inner.numberOfHits();

    float dPhi  = aElectron.deltaPhiSuperClusterTrackAtVtx();
    float dEta  = aElectron.deltaEtaSuperClusterTrackAtVtx();
    float sihih = aElectron.sigmaIetaIeta();
    float HoE   = aElectron.hadronicOverEm();

    aElectron.addUserFloat("nHits",nHits);
    aElectron.addUserFloat("dPhi",fabs(dPhi));
    aElectron.addUserFloat("dEta",fabs(dEta));
    aElectron.addUserFloat("sihih",sihih);
    aElectron.addUserFloat("HoE",HoE);
    
    int passconversionveto = 
      int(!ConversionTools::hasMatchedConversion(dynamic_cast<reco::GsfElectron const&>(*(aElectron.originalObjectRef())),hConversions,thebs.position(),true,2.0,1e-06,0));
    aElectron.addUserInt("antiConv",passconversionveto);
    
    double dxyWrtPV =  -99.;
    double dzWrtPV  =  -99.;

    if(vertexes->size()!=0 && (aElectron.gsfTrack()).isNonnull() ){
      dxyWrtPV = (aElectron.gsfTrack())->dxy( (*vertexes)[0].position() ) ;
      dzWrtPV  = (aElectron.gsfTrack())->dz(  (*vertexes)[0].position() ) ;
    }
    else if (vertexes->size()!=0 && (aElectron.track()).isNonnull() ){
      dxyWrtPV = (aElectron.track())->dxy( (*vertexes)[0].position() ) ;
      dzWrtPV  = (aElectron.track())->dz(  (*vertexes)[0].position() ) ;
    }

    aElectron.addUserFloat("dxyWrtPV",dxyWrtPV);
    aElectron.addUserFloat("dzWrtPV",dzWrtPV);
    
    aElectron.addUserFloat("mvaIdTrig"   ,aElectron.electronID("mvaTrigV0"));
    aElectron.addUserFloat("mvaIdNonTrig",aElectron.electronID("mvaNonTrigV0"));
    aElectron.addUserFloat("mvaIdTrigNoIP",aElectron.electronID("mvaTrigNoIPV0"));

    //isolation with custom configuration
    double EffArea_ = ElectronEffectiveArea::GetElectronEffectiveArea(ElectronEffectiveArea::kEleGammaAndNeutralHadronIso04, aElectron.superCluster()->eta(),ElectronEffectiveArea::kEleEAData2012);
    double pfRelIso04 = (aElectron.userIsolation(pat::PfChargedHadronIso) + std::max(0., ((aElectron.userIsolation(pat::PfGammaIso)+aElectron.userIsolation(pat::PfNeutralHadronIso))-(rho_*EffArea_))))/aElectron.pt();
    
    aElectron.addUserFloat("PFRelIso04", pfRelIso04);
    electronsUserEmbeddedColl->push_back(aElectron);
    
  }


  iEvent.put( electronsUserEmbeddedColl );
  return;
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(ElectronsUserEmbedded);
