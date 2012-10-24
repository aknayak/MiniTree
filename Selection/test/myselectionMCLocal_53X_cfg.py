
import FWCore.ParameterSet.Config as cms

from MiniTree.Selection.LocalRunSkeleton_cff import *


process.maxEvents.input = cms.untracked.int32(100)
process.TFileService.fileName = cms.string('mc_tau.root')

# config parameters ------------------------------------------------------------
procName='LOCALUSER'
#process.source.fileNames = ["file:/sps/cms/anayak/LocalData/relval-CMSSW523/relvalTTBar-CMSSW523-AODSIM-START52_V5-v1_numEvent1000.root"]
#process.source.fileNames = ["file:/sps/cms/anayak/LocalData/WToTauNu_TuneZ2star_8TeV_pythia6_tauola-U_S7_START50_V15-v1_numEvent10000.root"]
#process.source.fileNames = ["/store/cmst3/user/pharris/HTauTauSynchronization/VBF_HToTauTau_M-120_8TeV-powheg-pythia6-tauola_FED5F7FE-0597-E111-BE71-485B39800BB5.root"]
process.source.fileNames = ["/store/relval/CMSSW_5_3_4_cand1-START53_V10/RelValWjet_Pt_3000_3500/GEN-SIM-RECO/v1/0000/EA116D50-68F5-E111-AFF5-0018F3D0966C.root"]

trigMenu = 'HLT'
isData=False
isAOD=False
isFastsim = False
#mutriglist = [ 'HLT_Mu15_v2' ]
mutriglist = [ 'HLT_IsoMu24_eta2p1_v8' ]
egtriglist = [ 'HLT_Ele27_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_v2']
jettriglist = [ 'HLT_Jet30_v1' ]
trigpath = ''
applyResJEC=False
addPF2PAT=False
storeOutPath=False

# start process configuration -------------------------------------------------
process.setName_(procName)
producePDFweights=False
process.GlobalTag.globaltag = cms.string( 'START52_V5::All' )


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
process.myMiniTreeProducer.MCTruth.sampleCode = cms.string('TTBAR')
process.myMiniTreeProducer.MCTruth.producePDFweights = cms.bool(producePDFweights)
process.myMiniTreeProducer.Taus.sources = cms.VInputTag("patTaus", "patTausPFlow")
process.myMiniTreeProducer.minEventQualityToStore = cms.int32(0)
process.myMiniTreeProducer.Trigger.source = cms.InputTag('TriggerResults::'+trigMenu)
process.myMiniTreeProducer.Trigger.bits = cms.vstring()
process.myMiniTreeProducer.Trigger.bits = mutriglist
process.myMiniTreeProducer.Trigger.bits.extend( egtriglist )
process.myMiniTreeProducer.Trigger.bits.extend( jettriglist )
#process.myMiniTreeProducer.KineFit.runKineFitter = cms.bool(False)
########################################################

# analysis sequence ------------------------------------------------------------
process.tau_extra = cms.Path(process.PFTau)
process.met_extra = cms.Path(process.type0PFMEtCorrection * process.producePFMETCorrections)
process.kineFit = cms.Path(process.kinFitTtSemiLepEvent)

process.p  = cms.Path(process.allEventsFilter*process.basePreSel*process.myMiniTreeProducer)
#process.p  = cms.Path( process.basePreSel*process.myMiniTreeProducer)

if( addPF2PAT ):
    process.pat_default = cms.Path( process.patSequence * process.patDefaultSequence * process.puJetIdSqeuence)
else :
    process.pat_default = cms.Path( process.patDefaultSequence * process.puJetIdSqeuence)

process.schedule = cms.Schedule(process.tau_extra, process.met_extra, process.pat_default, process.kineFit, process.p)

checkProcessSchedule(storeOutPath,True)

if(isAOD) :
    print "**** This is AOD run ****"
    from PhysicsTools.PatAlgos.tools.coreTools import *
    restrictInputToAOD(process)
    process.myMiniTreeProducer.Electrons.ebRecHits = cms.InputTag("reducedEcalRecHitsEB")
    process.myMiniTreeProducer.Electrons.eeRecHits = cms.InputTag("reducedEcalRecHitsEE")
