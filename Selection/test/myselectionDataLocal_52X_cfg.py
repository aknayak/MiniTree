
import FWCore.ParameterSet.Config as cms

from MiniTree.Selection.LocalRunSkeleton_cff import *


process.maxEvents.input = cms.untracked.int32(1000)
process.TFileService.fileName = cms.string('data_tau.root')

# config parameters ------------------------------------------------------------
procName='LOCALUSER'
process.source.fileNames = ['file:/sps/cms/anayak/LocalData/Run2012A-SingleMu-AOD-PromptReco-v1/Run2012A-SingleMu-AOD-190731-7CF056BD-B383-E111-8787-003048CF94A6.root']
mutriglist = ['HLT_IsoMu24_v4']
egtriglist = ['HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3']
jettriglist = ['HLT_Jet30U_v3']
trigpath = ''
#trigpath = 'inclusive_mu'
isData=True
isFastSim=False
trigMenu = 'HLT'
applyResJEC=True
addPF2PAT=False
isAOD=True
storeOutPath = False

# start process configuration -------------------------------------------------
process.setName_(procName)
process.GlobalTag.globaltag = cms.string( 'GR_R_52_V7::All' )

# configure the extra modules -------------------------------------------------
if(addPF2PAT):
    print "**** Adding PF2PAT objects ****"
    addpf2PatSequence(process, not isData)
defineBasePreSelection(process,False,True,trigpath,egtriglist,mutriglist,jettriglist)

configureTauProduction(process, not isData)
addJetMETExtra(process,isData,applyResJEC,isAOD)
addTriggerMatchExtra(process,egtriglist,mutriglist,jettriglist,addPF2PAT,trigMenu)


# add the analysis modules ----------------------------------------------------
process.load('MiniTree.Selection.selection_cfi')
process.myMiniTreeProducer.MCTruth.isData = cms.bool(isData)
process.myMiniTreeProducer.MCTruth.sampleCode = cms.string('DATA')
process.myMiniTreeProducer.Taus.sources = cms.VInputTag("patTaus", "patTausPFlow")
process.myMiniTreeProducer.minEventQualityToStore = cms.int32(0)
process.myMiniTreeProducer.Trigger.source = cms.InputTag('TriggerResults::'+trigMenu)
process.myMiniTreeProducer.Trigger.bits = cms.vstring()
process.myMiniTreeProducer.Trigger.bits = mutriglist
process.myMiniTreeProducer.Trigger.bits.extend( egtriglist )
process.myMiniTreeProducer.Trigger.bits.extend( jettriglist )

########################################################

# analysis sequence ------------------------------------------------------------
process.tau_extra = cms.Path(process.PFTau)

process.p  = cms.Path( process.basePreSel*process.myMiniTreeProducer)

if( addPF2PAT ):
    process.pat_default = cms.Path( process.patSequence * process.patDefaultSequence )
else :
    process.pat_default = cms.Path( process.patDefaultSequence )

process.schedule = cms.Schedule(process.tau_extra, process.pat_default, process.p)
checkProcessSchedule(storeOutPath,True)

if(isAOD) :
    print "**** This is AOD run ****"
    from PhysicsTools.PatAlgos.tools.coreTools import *
    restrictInputToAOD(process)
    process.myMiniTreeProducer.Electrons.ebRecHits = cms.InputTag("reducedEcalRecHitsEB")
    process.myMiniTreeProducer.Electrons.eeRecHits = cms.InputTag("reducedEcalRecHitsEE")
