import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:/data_CMS/cms/anayak/HTauTauSynchronization/8TeV/53X/VBF_HToTauTau_M-125_8TeV-powheg-pythia6-Summer12_DR53X-PU_S10_START53_V7A-v1-1200542B-D9ED-E111-B708-00237DA1A548.root'
    )
)

process.demo = cms.EDAnalyzer('HLTFilter',
                              hltmenu = cms.InputTag('TriggerResults::HLT')
)


process.p = cms.Path(process.demo)
