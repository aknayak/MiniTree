import FWCore.ParameterSet.Config as cms

resCorJet = cms.EDProducer("CorrJetProducer",
    inputJets    = cms.InputTag("patJets"),
    JECSrcFile   = cms.FileInPath("MiniTree/Utilities/data/53XPTFIXV2_DATA_L2L3Residual_AK5PFchs.txt"),
    rho          = cms.InputTag('kt6PFJets', 'rho'),                           
)
