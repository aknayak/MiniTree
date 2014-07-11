#include <algorithm>

#include "FWCore/Framework/interface/Event.h"
#include "MiniTree/Utilities/interface/CorrJetProducer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"

CorrJetProducer::CorrJetProducer(const edm::ParameterSet& cfg):
  inputJets_           (cfg.getParameter<edm::InputTag>("inputJets"           )),
  JECSrcFile_          (cfg.getParameter<edm::FileInPath>("JECSrcFile") ),
  rho_                 (cfg.getParameter<edm::InputTag>("rho"))
{
  // register products
  produces<std::vector<pat::Jet> >(outputJets_);
}

void
CorrJetProducer::beginJob()
{ 

}

void
CorrJetProducer::produce(edm::Event& event, const edm::EventSetup& setup)
{
  // access jets
  edm::Handle<std::vector<pat::Jet> > jets;
  event.getByLabel(inputJets_, jets);

  // get parameter rho for L1FastJet correction level if needed
  edm::Handle<double> rho;
  if(!rho_.label().empty()) event.getByLabel(rho_, rho);
  
  // create two new collections for jets and MET
  std::auto_ptr<std::vector<pat::Jet> > pJets(new std::vector<pat::Jet>);

  // Create the JetCorrectorParameter objects, the order does not matter.
  JetCorrectorParameters *ResJetPar = new JetCorrectorParameters(JECSrcFile_.fullPath());
  //  Load the JetCorrectorParameter objects into a vector, IMPORTANT: THE ORDER MATTERS HERE !!!! 
  std::vector<JetCorrectorParameters> vPar;
  vPar.push_back(*ResJetPar);

  FactorizedJetCorrector *JetCorrector = new FactorizedJetCorrector(vPar);
  
  // loop and rescale jets
  for(std::vector<pat::Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet){
    pat::Jet scaledJet = *jet;

    JetCorrector->setJetEta(scaledJet.eta());
    JetCorrector->setJetPt(scaledJet.pt());
    JetCorrector->setJetA(scaledJet.jetArea());
    JetCorrector->setRho(*rho); 

    //Get the correction
    double correction = JetCorrector->getCorrection();
    
    // scale jet momentum for the correction
    scaleJetEnergy( scaledJet, correction );
    
    pJets->push_back( scaledJet );
  }

  delete ResJetPar;
  delete JetCorrector;
  //vPar.clear();

  event.put(pJets, outputJets_);
}

void
CorrJetProducer::scaleJetEnergy(pat::Jet& jet, double factor)
{
  jet.scaleEnergy( factor );

  if(jet.isPFJet()){
    pat::PFSpecific specificPF = jet.pfSpecific();
    specificPF.mChargedHadronEnergy = factor * specificPF.mChargedHadronEnergy;
    specificPF.mNeutralHadronEnergy = factor * specificPF.mNeutralHadronEnergy;
    specificPF.mPhotonEnergy        = factor * specificPF.mPhotonEnergy       ;
    specificPF.mElectronEnergy      = factor * specificPF.mElectronEnergy     ;
    specificPF.mMuonEnergy          = factor * specificPF.mMuonEnergy         ;
    specificPF.mHFHadronEnergy      = factor * specificPF.mHFHadronEnergy     ;
    specificPF.mHFEMEnergy          = factor * specificPF.mHFEMEnergy         ;
    specificPF.mChargedEmEnergy     = factor * specificPF.mChargedEmEnergy    ;
    specificPF.mChargedMuEnergy     = factor * specificPF.mChargedMuEnergy    ;
    specificPF.mNeutralEmEnergy     = factor * specificPF.mNeutralEmEnergy    ;
    jet.setPFSpecific(specificPF);
  }
  else if(jet.isCaloJet() || jet.isJPTJet()){
    pat::CaloSpecific specificCalo = jet.caloSpecific();
    specificCalo.mMaxEInEmTowers         = factor * specificCalo.mMaxEInEmTowers        ;
    specificCalo.mMaxEInHadTowers        = factor * specificCalo.mMaxEInHadTowers       ;
    specificCalo.mHadEnergyInHO          = factor * specificCalo.mHadEnergyInHO         ;
    specificCalo.mHadEnergyInHB          = factor * specificCalo.mHadEnergyInHB         ;
    specificCalo.mHadEnergyInHF          = factor * specificCalo.mHadEnergyInHF         ;
    specificCalo.mHadEnergyInHE          = factor * specificCalo.mHadEnergyInHE         ;
    specificCalo.mEmEnergyInEB           = factor * specificCalo.mEmEnergyInEB          ;
    specificCalo.mEmEnergyInEE           = factor * specificCalo.mEmEnergyInEE          ;
    specificCalo.mEmEnergyInHF           = factor * specificCalo.mEmEnergyInHF          ;
    specificCalo.mEnergyFractionHadronic = factor * specificCalo.mEnergyFractionHadronic;
    specificCalo.mEnergyFractionEm       = factor * specificCalo.mEnergyFractionEm      ;
    jet.setCaloSpecific(specificCalo);

    if(jet.isJPTJet()){
      pat::JPTSpecific specificJPT = jet.jptSpecific();
      specificJPT.mChargedHadronEnergy          = factor * specificJPT.mChargedHadronEnergy         ;
      specificJPT.mNeutralHadronEnergy          = factor * specificJPT.mNeutralHadronEnergy         ;
      specificJPT.mChargedEmEnergy              = factor * specificJPT.mChargedEmEnergy             ;
      specificJPT.mNeutralEmEnergy              = factor * specificJPT.mNeutralEmEnergy             ;
      specificJPT.mSumPtOfChargedWithEff        = factor * specificJPT.mSumPtOfChargedWithEff       ;
      specificJPT.mSumPtOfChargedWithoutEff     = factor * specificJPT.mSumPtOfChargedWithoutEff    ;
      specificJPT.mSumEnergyOfChargedWithEff    = factor * specificJPT.mSumEnergyOfChargedWithEff   ;
      specificJPT.mSumEnergyOfChargedWithoutEff = factor * specificJPT.mSumEnergyOfChargedWithoutEff;
      jet.setJPTSpecific(specificJPT);
    }
  }
}

//define this as a plug-in
DEFINE_FWK_MODULE( CorrJetProducer );
