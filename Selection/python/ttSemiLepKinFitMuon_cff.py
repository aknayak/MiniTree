import FWCore.ParameterSet.Config as cms

from TopQuarkAnalysis.TopObjectResolutions.stringResolutions_etEtaPhi_Fall11_cff import *
from MiniTree.Utilities.JetEnergyScale_cfi import *

def addSemiLepKinFitMuon(process, isData=False) :

    ## std sequence to produce the kinematic fit for semi-leptonic events
    process.load("TopQuarkAnalysis.TopKinFitter.TtSemiLepKinFitProducer_Muons_cfi")
    #apply selections on muon
    process.cleanPatMuons.preselection = cms.string("pt>20 && abs(eta)<2.4"+
                                                    " && isGlobalMuon && isPFMuon && isTrackerMuon" +
                                                    " && globalTrack.isNonnull "+
                                                    " && globalTrack.normalizedChi2<10"+
                                                    " && globalTrack.hitPattern.numberOfValidMuonHits>0"+
                                                    " && numberOfMatchedStations>1"+
                                                    " && innerTrack.hitPattern.numberOfValidPixelHits>0"+
                                                    " && track.hitPattern.trackerLayersWithMeasurement > 5"+
                                                    " && dB() < 0.2"+
                                                    " && (pfIsolationR04.sumChargedHadronPt+ max(0.,pfIsolationR04.sumNeutralHadronEt+pfIsolationR04.sumPhotonEt-0.5*pfIsolationR04.sumPUPt))/pt < 0.15"
                                                    )
    #clean jets from muons
    process.cleanPatJets.checkOverlaps.muons.requireNoOverlaps  = cms.bool(True)
    process.cleanPatJets.preselection = cms.string("pt>15 && abs(eta)<2.5")
    
    #change constraints on kineFit
    process.kinFitTtSemiLepEvent.constraints = cms.vuint32(3, 4)
    process.kinFitTtSemiLepEvent.maxNJets = cms.int32(5)
    process.kinFitTtSemiLepEvent.jets = cms.InputTag("cleanPatJets")
    process.kinFitTtSemiLepEvent.leps = cms.InputTag("cleanPatMuons")
    #process.kinFitTtSemiLepEvent.mets = cms.InputTag("pfType1CorrectedMet")
    process.kinFitTtSemiLepEvent.mets = cms.InputTag("patMETsPF")
    process.kinFitTtSemiLepEvent.udscResolutions = udscResolutionPF.functions
    process.kinFitTtSemiLepEvent.bResolutions = bjetResolutionPF.functions
    process.kinFitTtSemiLepEvent.lepResolutions = muonResolution.functions
    process.kinFitTtSemiLepEvent.metResolutions = metResolutionPF.functions
    process.kinFitTtSemiLepEvent.metResolutions[0].eta = "9999"
    if not isData :
        process.kinFitTtSemiLepEvent.jetEnergyResolutionScaleFactors = cms.vdouble (
            1.052, 1.057, 1.096, 1.134, 1.288  )
        process.kinFitTtSemiLepEvent.jetEnergyResolutionEtaBinning = cms.vdouble(
            0.0, 0.5, 1.1, 1.7, 2.3, -1. ) 
    # Add JES Up and Down and Rerun the KineFitter
    process.scaledJetEnergyUp = scaledJetEnergy.clone()
    process.scaledJetEnergyUp.inputJets = "cleanPatJets"
    process.scaledJetEnergyUp.inputMETs = "patMETsPF"
    process.scaledJetEnergyUp.scaleType = "jes:up"
    process.kinFitTtSemiLepEventJESUp = process.kinFitTtSemiLepEvent.clone()
    process.kinFitTtSemiLepEventJESUp.jets = cms.InputTag("scaledJetEnergyUp:cleanPatJets") 
    process.kinFitTtSemiLepEventJESUp.mets = cms.InputTag("scaledJetEnergyUp:patMETsPF")
    process.scaledJetEnergyDown = scaledJetEnergy.clone()
    process.scaledJetEnergyDown.inputJets = "cleanPatJets"
    process.scaledJetEnergyDown.inputMETs = "patMETsPF"
    process.scaledJetEnergyDown.scaleType = "jes:down"
    process.kinFitTtSemiLepEventJESDown = process.kinFitTtSemiLepEvent.clone()
    process.kinFitTtSemiLepEventJESDown.jets = cms.InputTag("scaledJetEnergyDown:cleanPatJets")
    process.kinFitTtSemiLepEventJESDown.mets = cms.InputTag("scaledJetEnergyDown:patMETsPF")
    process.kinFitSequence = cms.Sequence(process.kinFitTtSemiLepEvent * process.scaledJetEnergyUp * process.kinFitTtSemiLepEventJESUp * process.scaledJetEnergyDown * process.kinFitTtSemiLepEventJESDown) 
