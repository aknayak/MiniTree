import FWCore.ParameterSet.Config as cms

from TopQuarkAnalysis.TopObjectResolutions.stringResolutions_etEtaPhi_Fall11_cff import *
from MiniTree.Utilities.JetEnergyScale_cfi import *

def addSemiLepKinFitElectron(process, isData=False) :

    ## std sequence to produce the kinematic fit for semi-leptonic events
    process.load("TopQuarkAnalysis.TopKinFitter.TtSemiLepKinFitProducer_Electrons_cfi")
    #apply selections on electron
    simpleCutsVeto = "(" + \
                     " (isEB && userFloat('sihih')<0.010 && userFloat('dPhi')<0.80 && "+ \
                     "          userFloat('dEta') <0.007 && userFloat('HoE') <0.15)"   + \
                     " || "  + \
                     " (isEE && userFloat('sihih')<0.030 && userFloat('dPhi')<0.70 && "+ \
                     "          userFloat('dEta') <0.010)"   + \
                     ")"
    eleIdCut = "(" + \
               " (abs(superCluster.eta) < 0.8 && userFloat('mvaIdTrig')>0.94)" + \
               " || "  + \
               " (abs(superCluster.eta) > 0.8 && abs(superCluster.eta) < 1.479 && userFloat('mvaIdTrig')>0.85)" + \
               " || "  + \
               " (abs(superCluster.eta) > 1.479 && userFloat('mvaIdTrig')>0.92)" + \
               ")"
    process.cleanPatElectronsUser = process.cleanPatElectrons.clone()
    process.cleanPatElectronsUser.src = cms.InputTag("selectedPatElectronsUserEmbedded")
    process.cleanPatElectronsUser.preselection = cms.string("pt>30 && abs(eta)<2.5 && "+
                                                            simpleCutsVeto +
                                                            " && " +
                                                            eleIdCut +
                                                            " && abs(userFloat('dxyWrtPV'))<0.045 && abs(userFloat('dzWrtPV'))<0.2"+
                                                            " && userFloat('nHits')==0 && userInt('antiConv')>0.5" + 
                                                            " && userFloat('PFRelIso04')<0.15"
                                                            )
    #clean jets from electrons
    process.cleanPatJetsUser = process.cleanPatJets.clone()
    process.cleanPatJetsUser.checkOverlaps.electrons.requireNoOverlaps  = cms.bool(True)
    process.cleanPatJetsUser.checkOverlaps.electrons.src  = cms.InputTag("cleanPatElectronsUser")
    process.cleanPatJetsUser.preselection = cms.string("pt>20 && abs(eta)<2.5")

    #only used for data
    process.cleanPatJetsResCor = process.cleanPatJetsUser.clone()
    process.cleanPatJetsResCor.src = cms.InputTag("selectedPatJetsResCor")
    process.cleanPatJetsResCor.preselection = cms.string("pt>24 && abs(eta)<2.5")
    
    #smear the JetEnergy for JER in case of MC, don't use this scaled collection for Data
    process.scaledJetEnergyNominal = scaledJetEnergy.clone()
    process.scaledJetEnergyNominal.inputJets = "cleanPatJetsUser"
    process.scaledJetEnergyNominal.inputMETs = "patPfMetT0pcT1Txy"
    process.scaledJetEnergyNominal.scaleType = "jer"
    process.scaledJetEnergyNominal.resolutionEtaRanges = cms.vdouble(
        0.0, 0.5, 0.5, 1.1, 1.1, 1.7, 1.7, 2.3, 2.3, -1.0 )
    process.scaledJetEnergyNominal.resolutionFactors = cms.vdouble(
        1.052, 1.057, 1.096, 1.134, 1.288 )

    #change constraints on kineFit
    process.kinFitTtSemiLepEvent.mTop = cms.double(172.5)
    process.kinFitTtSemiLepEvent.constraints = cms.vuint32(3, 4)
    process.kinFitTtSemiLepEvent.maxNJets = cms.int32(-1)
    process.kinFitTtSemiLepEvent.jets = cms.InputTag("cleanPatJetsUser")
    if isData:
        process.kinFitTtSemiLepEvent.jets = cms.InputTag("cleanPatJetsResCor")
    process.kinFitTtSemiLepEvent.leps = cms.InputTag("cleanPatElectrons")
    #process.kinFitTtSemiLepEvent.mets = cms.InputTag("pfType1CorrectedMet")
    process.kinFitTtSemiLepEvent.mets = cms.InputTag("patPfMetT0pcT1Txy")
    process.kinFitTtSemiLepEvent.udscResolutions = udscResolutionPF.functions
    process.kinFitTtSemiLepEvent.bResolutions = bjetResolutionPF.functions
    process.kinFitTtSemiLepEvent.lepResolutions = elecResolution.functions
    process.kinFitTtSemiLepEvent.metResolutions = metResolutionPF.functions
    process.kinFitTtSemiLepEvent.metResolutions[0].eta = "9999"
    if not isData :
        process.kinFitTtSemiLepEvent.jetEnergyResolutionScaleFactors = cms.vdouble (
            1.052, 1.057, 1.096, 1.134, 1.288  )
        process.kinFitTtSemiLepEvent.jetEnergyResolutionEtaBinning = cms.vdouble(
            0.0, 0.5, 1.1, 1.7, 2.3, -1. )
        process.cleanPatJetsNominal = process.cleanPatJetsUser.clone()
        process.cleanPatJetsNominal.src = cms.InputTag("scaledJetEnergyNominal:cleanPatJetsUser")
        process.cleanPatJetsNominal.preselection = cms.string("pt>24 && abs(eta)<2.5")
        process.kinFitTtSemiLepEvent.jets = cms.InputTag("cleanPatJetsNominal")
        process.kinFitTtSemiLepEvent.mets = cms.InputTag("scaledJetEnergyNominal:patPfMetT0pcT1Txy")
    #set b-tagging in KineFit
    process.kinFitTtSemiLepEvent.bTagAlgo          = cms.string("combinedSecondaryVertexBJetTags")
    process.kinFitTtSemiLepEvent.minBDiscBJets     = cms.double(0.679)
    process.kinFitTtSemiLepEvent.maxBDiscLightJets = cms.double(3.0)
    process.kinFitTtSemiLepEvent.useBTagging       = cms.bool(True)
    # Add JES Up and Down and Rerun the KineFitter
    process.scaledJetEnergyUp = process.scaledJetEnergyNominal.clone()
    process.scaledJetEnergyUp.inputJets = "cleanPatJetsUser"
    process.scaledJetEnergyUp.inputMETs = "patPfMetT0pcT1Txy"
    process.scaledJetEnergyUp.scaleType = "jes:up"
    process.cleanPatJetsJESUp = process.cleanPatJetsUser.clone()
    process.cleanPatJetsJESUp.src = cms.InputTag("scaledJetEnergyUp:cleanPatJetsUser")
    process.cleanPatJetsJESUp.preselection = cms.string("pt>24 && abs(eta)<2.5")
    process.kinFitTtSemiLepEventJESUp = process.kinFitTtSemiLepEvent.clone()
    process.kinFitTtSemiLepEventJESUp.jets = cms.InputTag("cleanPatJetsJESUp") 
    process.kinFitTtSemiLepEventJESUp.mets = cms.InputTag("scaledJetEnergyUp:patPfMetT0pcT1Txy")
    process.scaledJetEnergyDown = process.scaledJetEnergyNominal.clone()
    process.scaledJetEnergyDown.inputJets = "cleanPatJetsUser"
    process.scaledJetEnergyDown.inputMETs = "patPfMetT0pcT1Txy"
    process.scaledJetEnergyDown.scaleType = "jes:down"
    process.cleanPatJetsJESDown = process.cleanPatJetsUser.clone()
    process.cleanPatJetsJESDown.src = cms.InputTag("scaledJetEnergyDown:cleanPatJetsUser")
    process.cleanPatJetsJESDown.preselection = cms.string("pt>24 && abs(eta)<2.5")
    process.kinFitTtSemiLepEventJESDown = process.kinFitTtSemiLepEvent.clone()
    process.kinFitTtSemiLepEventJESDown.jets = cms.InputTag("cleanPatJetsJESDown")
    process.kinFitTtSemiLepEventJESDown.mets = cms.InputTag("scaledJetEnergyDown:patPfMetT0pcT1Txy")
    # Add JER Up and Down and Rerun the KineFitter
    process.scaledJetEnergyResnUp = process.scaledJetEnergyNominal.clone()
    process.scaledJetEnergyResnUp.inputJets = "cleanPatJetsUser"
    process.scaledJetEnergyResnUp.inputMETs = "patPfMetT0pcT1Txy"
    process.scaledJetEnergyResnUp.scaleType = "jer"
    process.scaledJetEnergyResnUp.resolutionFactors = cms.vdouble(
        1.115, 1.114, 1.161, 1.228, 1.488 )
    process.cleanPatJetsResnUp = process.cleanPatJetsUser.clone()
    process.cleanPatJetsResnUp.src = cms.InputTag("scaledJetEnergyResnUp:cleanPatJetsUser")
    process.cleanPatJetsResnUp.preselection = cms.string("pt>24 && abs(eta)<2.5")
    process.kinFitTtSemiLepEventJERUp = process.kinFitTtSemiLepEvent.clone()
    process.kinFitTtSemiLepEventJERUp.jets = cms.InputTag("cleanPatJetsResnUp")
    process.kinFitTtSemiLepEventJERUp.mets = cms.InputTag("scaledJetEnergyResnUp:patPfMetT0pcT1Txy")
    process.scaledJetEnergyResnDown = process.scaledJetEnergyNominal.clone()
    process.scaledJetEnergyResnDown.inputJets = "cleanPatJetsUser"
    process.scaledJetEnergyResnDown.inputMETs = "patPfMetT0pcT1Txy"
    process.scaledJetEnergyResnDown.scaleType = "jer"
    process.scaledJetEnergyResnDown.resolutionFactors = cms.vdouble(
        0.990, 1.001, 1.032, 1.042, 1.089 )
    process.cleanPatJetsResnDown = process.cleanPatJetsUser.clone()
    process.cleanPatJetsResnDown.src = cms.InputTag("scaledJetEnergyResnDown:cleanPatJetsUser")
    process.cleanPatJetsResnDown.preselection = cms.string("pt>24 && abs(eta)<2.5")
    process.kinFitTtSemiLepEventJERDown = process.kinFitTtSemiLepEvent.clone()
    process.kinFitTtSemiLepEventJERDown.jets = cms.InputTag("cleanPatJetsResnDown")
    process.kinFitTtSemiLepEventJERDown.mets = cms.InputTag("scaledJetEnergyResnDown:patPfMetT0pcT1Txy")
    process.kinFitSequence = cms.Sequence(process.cleanPatElectronsUser* process.cleanPatJetsUser* process.cleanPatJetsResCor* process.kinFitTtSemiLepEvent)
    if not isData :
        process.kinFitSequence.remove(process.cleanPatJetsResCor)
        process.kinFitSequence.replace(process.kinFitTtSemiLepEvent, process.scaledJetEnergyNominal * process.cleanPatJetsNominal * process.kinFitTtSemiLepEvent * process.scaledJetEnergyUp * process.cleanPatJetsJESUp * process.kinFitTtSemiLepEventJESUp * process.scaledJetEnergyDown * process.cleanPatJetsJESDown * process.kinFitTtSemiLepEventJESDown * process.scaledJetEnergyResnUp * process.cleanPatJetsResnUp * process.kinFitTtSemiLepEventJERUp * process.scaledJetEnergyResnDown * process.cleanPatJetsResnDown * process.kinFitTtSemiLepEventJERDown) 
    print "jets used in Kinematic fit", process.kinFitTtSemiLepEvent.jets    
    print "jet input to cleanPatJetsResCor", process.cleanPatJetsResCor.src
