
import FWCore.ParameterSet.Config as cms

from MiniTree.Selection.LocalRunSkeleton_cff import *


process.maxEvents.input = cms.untracked.int32(-1)
process.TFileService.fileName = cms.string('data_tree.root')

# config parameters ------------------------------------------------------------
procName='LOCALUSER'
process.source.fileNames = ["file:/tmp/gkole/4A898E77-AEE3-E211-AB0A-0025907277A0.root"]

trigMenu = 'HLT'
isData=True
isAOD=True
isFastsim = False
#mutriglist = [ 'HLT_Mu15_v2' ]
mutriglist = [ 'HLT_IsoMu24_eta2p1_v1' ]
egtriglist = [ 'HLT_Ele27_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_v2']
jettriglist = [ 'HLT_Jet30_v1' ]
trigpath = ''
applyResJEC=False
addPF2PAT=False
storeOutPath=True

# start process configuration -------------------------------------------------
process.setName_(procName)
producePDFweights=False
process.GlobalTag.globaltag = cms.string( 'FT_53_V21_AN4::All' )


# configure the extra modules -------------------------------------------------
if(addPF2PAT):
    print "**** Adding PF2PAT objects ****"
    addpf2PatSequence(process, not isData)
defineBasePreSelection(process,False,not isFastsim and not isAOD)

configureTauProduction(process, not isData)
addJetMETExtra(process,isData,applyResJEC,isAOD)
addTriggerMatchExtra(process,egtriglist,mutriglist,jettriglist,False,trigMenu)
defineGenUtilitiesSequence(process)
addSemiLepKinFitMuon(process, isData)

# add the analysis modules ----------------------------------------------------
process.load('MiniTree.Selection.selection_cfi')
process.myMiniTreeProducer.MCTruth.isData = cms.bool(isData)
process.myMiniTreeProducer.MCTruth.sampleCode = cms.string('DATA')
process.myMiniTreeProducer.MCTruth.producePDFweights = cms.bool(producePDFweights)
process.myMiniTreeProducer.Taus.sources = cms.VInputTag("patTaus", "patTausPFlow")
process.myMiniTreeProducer.minEventQualityToStore = cms.int32(2)
process.myMiniTreeProducer.Trigger.source = cms.InputTag('TriggerResults::'+trigMenu)
process.myMiniTreeProducer.Trigger.bits = cms.vstring()
process.myMiniTreeProducer.Trigger.bits = mutriglist
process.myMiniTreeProducer.Trigger.bits.extend( egtriglist )
process.myMiniTreeProducer.Trigger.bits.extend( jettriglist )
#process.myMiniTreeProducer.KineFit.runKineFitter = cms.bool(False)
########################################################

# analysis sequence ------------------------------------------------------------
#process.tau_extra = cms.Path(process.PFTau)
process.met_extra = cms.Path(process.type0PFMEtCorrection * process.producePFMETCorrections)
if(isData and not applyResJEC ):
    process.jet_extra = cms.Path(process.ResJetCorSequence)
process.kineFit = cms.Path(process.kinFitSequence) #cms.Path(process.kinFitTtSemiLepEvent)

process.p  = cms.Path(process.allEventsFilter*process.basePreSel*process.myMiniTreeProducer)
#process.p  = cms.Path( process.basePreSel*process.myMiniTreeProducer)

if( addPF2PAT ):
    process.pat_default = cms.Path( process.patSequence * process.patDefaultSequence * process.puJetIdSqeuence)
else :
    process.pat_default = cms.Path( process.patDefaultSequence * process.puJetIdSqeuence)
        
process.schedule = cms.Schedule(process.met_extra, process.pat_default, process.kineFit, process.p)
if(isData and not applyResJEC ):
    process.schedule = cms.Schedule(process.met_extra, process.pat_default, process.jet_extra, process.kineFit, process.p)

checkProcessSchedule(storeOutPath,True)

if(isAOD) :
    print "**** This is AOD run ****"
    from PhysicsTools.PatAlgos.tools.coreTools import *
    restrictInputToAOD(process)
    process.myMiniTreeProducer.Electrons.ebRecHits = cms.InputTag("reducedEcalRecHitsEB")
    process.myMiniTreeProducer.Electrons.eeRecHits = cms.InputTag("reducedEcalRecHitsEE")
