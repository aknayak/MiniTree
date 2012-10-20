import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.metTools import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
        
def addJetMETExtra(process, isData=False, applyResJEC=True, isAOD=False) :

    #process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
    ##-------------------- Import the Jet RECO modules -----------------------
    #process.load('RecoJets.Configuration.RecoPFJets_cff')
    ##-------------------- Turn-on the FastJet density calculation -----------------------
    #process.kt6PFJets.doRhoFastjet = True
    ##-------------------- Turn-on the FastJet jet area calculation for your favorite algorithm -----------------------
    #process.ak5PFJets.doAreaFastjet = True
    #process.FastJetSequence = cms.Sequence(process.kt6PFJets * process.ak5PFJets)
    if(isData) :
        if(applyResJEC) :
            corrections = ['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual', 'L5Flavor', 'L7Parton']
        else :
            corrections = ['L1FastJet','L2Relative','L3Absolute','L5Flavor','L7Parton']
        runOnData(process, ['All'])
    else :
        corrections = ['L1FastJet','L2Relative','L3Absolute','L5Flavor','L7Parton']
    if( isAOD ) : process.patJets.addTagInfos   = cms.bool(False)
    
    from PhysicsTools.PatAlgos.recoLayer0.jetCorrFactors_cfi import *
    process.patJetCorrFactors.levels = ['L1Offset', 'L2Relative', 'L3Absolute']
    process.patJetCorrFactors.useRho = cms.bool(True)
    
    print "*** Switching to PF ak5 jets ***"
    switchJetCollection(process,cms.InputTag('ak5PFJets'),
                     doJTA        = True,
                     doBTagging   = True,
                     jetCorrLabel = ('AK5PF',corrections),
                     doType1MET   = False,
                     genJetCollection = cms.InputTag("ak5GenJets"),
                     doJetID      = True,
                     jetIdLabel   = "ak5"
                     )
    if( isAOD ) : process.patJets.addTagInfos = cms.bool(False)
    
    print "*** Adding PF MET ***"
    addPfMET(process, 'PF')
            
    print "*** Adding PileupJetID ***"
    process.load("CMGTools.External.pujetidsequence_cff")
