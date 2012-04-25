import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.trigTools import *

#
# wrapper for trigger matches
#
def addTriggerMatchExtra(process, eleTrigList=['HLT_Ele10_LW_L1R'], muTrigList=['HLT_Mu9'], jetTrigList=['HLT_Jet30U'], addPF2PAT=False, trigMenu='HLT') :

    jetTrig = ''
    for trig in jetTrigList:
        if(len(jetTrig) > 0):
            jetTrig = jetTrig + ' | '
        jetTrig = jetTrig + 'path("'
        jetTrig = jetTrig + trig
        jetTrig = jetTrig + '")'
    muTrig = ''
    for trig in muTrigList:
        if(len(muTrig) > 0):
            muTrig = muTrig + ' | '
        muTrig = muTrig + 'path("'
        muTrig = muTrig + trig
        muTrig = muTrig + '")'
    eleTrig = ''
    for trig in eleTrigList:
        if(len(eleTrig) > 0):
            eleTrig = eleTrig + ' | '
        eleTrig = eleTrig + 'path("'
        eleTrig = eleTrig + trig
        eleTrig = eleTrig + '")'
        
    trigMatchModules=[]
    
    # muons
    if( len(muTrigList) > 0 ):
        from PhysicsTools.PatAlgos.triggerLayer1.triggerMatcher_cfi import cleanMuonTriggerMatchHLTMu20

        process.MuonsTrigMatch = cleanMuonTriggerMatchHLTMu20.clone()
        process.MuonsTrigMatch.src = cms.InputTag("selectedPatMuons")
        process.MuonsTrigMatch.matchedCuts = cms.string( muTrig )
        #process.MuonsTrigMatch.matchedCuts = cms.string( 'path( "HLT_*" )' )
        trigMatchModules.append('MuonsTrigMatch')

        if(addPF2PAT):        
            process.MuonsPFlowTrigMatch = process.MuonsTrigMatch.clone()
            process.MuonsPFlowTrigMatch.src = cms.InputTag("selectedPatMuonsPFlow")
            trigMatchModules.append('MuonsPFlowTrigMatch')
            
    # electrons
    if( len(eleTrigList) > 0 ):
        from PhysicsTools.PatAlgos.triggerLayer1.triggerMatcher_cfi import cleanElectronTriggerMatchHLTEle27CaloIdVTCaloIsoTTrkIdTTrkIsoT

        process.ElectronsTrigMatch = cleanElectronTriggerMatchHLTEle27CaloIdVTCaloIsoTTrkIdTTrkIsoT.clone()
        process.ElectronsTrigMatch.src = cms.InputTag("selectedPatElectrons")
        process.ElectronsTrigMatch.matchedCuts = cms.string( eleTrig )
        #process.ElectronsTrigMatch.matchedCuts = cms.string( 'path( "HLT_*" )' )
        trigMatchModules.append('ElectronsTrigMatch')

        if(addPF2PAT):
            process.ElectronsPFlowTrigMatch = process.ElectronsTrigMatch.clone()
            process.ElectronsPFlowTrigMatch.src = cms.InputTag("selectedPatElectronsPFlow")
            trigMatchModules.append('ElectronsPFlowTrigMatch')

    # jets
    if( len(jetTrigList) > 0 ):
        from PhysicsTools.PatAlgos.triggerLayer1.triggerMatcher_cfi import cleanJetTriggerMatchHLTJet240

        process.JetsTrigMatch = cleanJetTriggerMatchHLTJet240.clone()
        process.JetsTrigMatch.src = cms.InputTag("selectedPatJets")
        process.JetsTrigMatch.matchedCuts = cms.string( jetTrig )
        #process.JetsTrigMatch.matchedCuts = cms.string( 'path( "HLT_*" )' )
        trigMatchModules.append('JetsTrigMatch')
        
        #process.JetsAK5PFTrigMatch = process.JetsTrigMatch.clone()
        #process.JetsAK5PFTrigMatch.src = cms.InputTag("selectedPatJetsAK5PF")
        #trigMatchModules.append('JetsAK5PFTrigMatch')
        
        if(addPF2PAT):
            process.JetsPFlowTrigMatch = process.JetsTrigMatch.clone()
            process.JetsPFlowTrigMatch.src = cms.InputTag("selectedPatJetsPFlow")
            trigMatchModules.append('JetsPFlowTrigMatch')

    #init trigger match embedding
    if(len(trigMatchModules)>0) :
        print '**** Creating trigger match sequence ****'
        print trigMatchModules
        switchOnTriggerMatching( process, trigMatchModules, hltProcess=trigMenu)
        #removeCleaningFromTriggerMatching(process)
        print '*****************************************'
        
