#ifndef CorrJetProducer_h
#define CorrJetProducer_h

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

/**
   \class   CorrJetProducer CorrJetProducer.h 

   \brief   Plugin to apply the jet energy scale on fly from a .txt file.
            It is aimed to be used for test purposes of L2L3Residual 
            correction for data. This test correction is derived by Mikko
            to solve the problem of data/mc difference in W mass. 
            The plugin takes input collection of PAT jets which are already 
            corrected for L1,L2,L3 correction and applies only Residual correction 
            on it. The output is a collection of PAT jets. 

*/

class CorrJetProducer : public edm::EDProducer {

 public:
  /// default constructor
  explicit CorrJetProducer(const edm::ParameterSet&);
  /// default destructor
  ~CorrJetProducer(){};
  
 private:
  /// check settings
  virtual void beginJob();
  /// rescale jet energy
  virtual void produce(edm::Event&, const edm::EventSetup&);
  /// scale all energies of the jet
  void scaleJetEnergy(pat::Jet&, double);

 private:
  /// jet input collection 
  edm::InputTag inputJets_;
  /// met input collection
  std::string outputJets_;
  /// JECSource File
  edm::FileInPath JECSrcFile_;
  //rho for pileup density
  edm::InputTag rho_;
  
  //FactorizedJetCorrector *JetCorrector;
};

#endif
