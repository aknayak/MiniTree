import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.metTools import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
from MiniTree.Utilities.JECFactorsProducer_cfi import *

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
            corrections = ['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']
        else :
            corrections = ['L1FastJet','L2Relative','L3Absolute']
        runOnData(process, ['All'])
    else :
        corrections = ['L1FastJet','L2Relative','L3Absolute']
    if( isAOD ) : process.patJets.addTagInfos   = cms.bool(False)
    print "Jet corrections used ", corrections
    
    #from PhysicsTools.PatAlgos.recoLayer0.jetCorrFactors_cfi import *
    #process.patJetCorrFactors.levels = ['L1Offset', 'L2Relative', 'L3Absolute']
    #process.patJetCorrFactors.useRho = cms.bool(True)
    
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
    #if( isAOD ) : process.patJets.addTagInfos = cms.bool(False)
    process.patJets.addTagInfos = cms.bool(True)
    
    print "*** Adding PF MET ***"
    ##process.load("JetMETCorrections.Type1MET.pfMETCorrectionType0_cfi")
    ##process.pfType1CorrectedMet.applyType0Corrections = cms.bool(False)
    ##process.pfType1CorrectedMet.srcType1Corrections = cms.VInputTag(
    ##    cms.InputTag('pfMETcorrType0'),
    ##    cms.InputTag('pfJetMETcorr', 'type1')
    ##    )
    ##addPfMET(process, 'PF')

    process.load("JetMETCorrections.Type1MET.correctionTermsPfMetType1Type2_cff")
    if(isData) :
        process.corrPfMetType1.jetCorrLabel = cms.string("ak5PFL1FastL2L3Residual")
    else :
        process.corrPfMetType1.jetCorrLabel = cms.string("ak5PFL1FastL2L3")
    process.load("JetMETCorrections.Type1MET.correctionTermsPfMetType0PFCandidate_cff")
    #process.load("JetMETCorrections.Type1MET.correctionTermsPfMetType0RecoTrack_cff")
    process.load("JetMETCorrections.Type1MET.correctionTermsPfMetShiftXY_cff")
    if(isData) :
        process.corrPfMetShiftXY.parameter = process.pfMEtSysShiftCorrParameters_2012runABCDvsNvtx_data
    else :
        process.corrPfMetShiftXY.parameter = process.pfMEtSysShiftCorrParameters_2012runABCDvsNvtx_mc
    process.load("JetMETCorrections.Type1MET.correctedMet_cff")
    process.load("JetMETCorrections.Type1MET.pfMETCorrections_cff")
    process.RecoMetSequence = cms.Sequence(process.pfCandsNotInJet * process.pfJetMETcorr * process.pfCandMETcorr * process.pfchsMETcorr *
                                           process.corrPfMetType1 * process.correctionTermsPfMetType1Type2 * process.correctionTermsPfMetType0PFCandidate *
                                           process.correctionTermsPfMetShiftXY * process.pfMetT0pcT1Txy)
    from PhysicsTools.PatAlgos.producersLayer1.metProducer_cfi import patMETs
    process.patPfMetT0pcT1Txy = patMETs.clone(
        metSource = cms.InputTag('pfMetT0pcT1Txy'),
        addMuonCorrections = cms.bool(False),
        addGenMET    = cms.bool(False)
        )
    print "*** Adding PileupJetID ***"
    process.load("CMGTools.External.pujetidsequence_cff")

    # Add user residual correction for Data from Mikko
    if(isData and not applyResJEC ) :
        process.selectedPatJetsResCor = resCorJet.clone()
        process.selectedPatJetsResCor.inputJets    = cms.InputTag("selectedPatJets")
        process.selectedPatJetsResCor.JECSrcFile   = cms.FileInPath("MiniTree/Utilities/data/53XPTFIXV2_DATA_L2L3Residual_AK5PFchs.txt")
        process.selectedPatJetsResCor.rho          = cms.InputTag('kt6PFJets', 'rho')
        process.puJetIdResCor = process.puJetId.clone()
        process.puJetIdResCor.jets = cms.InputTag("selectedPatJetsResCor")
        process.puJetMvaResCor = process.puJetMva.clone()
        process.puJetMvaResCor.jets = cms.InputTag("selectedPatJetsResCor")
        process.puJetMvaResCor.jetids = cms.InputTag("puJetIdResCor")
        process.ResJetCorSequence = cms.Sequence(process.selectedPatJetsResCor*process.puJetIdResCor*process.puJetMvaResCor)
