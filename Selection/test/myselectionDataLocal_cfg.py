
import FWCore.ParameterSet.Config as cms

from MiniTree.Selection.LocalRunSkeleton_cff import *


#tau stuff
from MiniTree.Selection.TauExtra_cff import *
#PFlow
from MiniTree.Selection.pfToPatSequences_cff import *


process.maxEvents.input = cms.untracked.int32(200)
process.TFileService.fileName = cms.string('data_tau_1.root')

# config parameters ------------------------------------------------------------
procName='LOCALUSER'
process.source.fileNames = ['file:/lustre/lip.pt/data/cmslocal/samples/CMSSW_4_2_X/data/Run2011A-SingleElectron-AOD/E2599C3F-C487-E011-B415-0019B9F72BFF.root']
mutriglist = ['HLT_IsoMu24_v4']
egtriglist = ['HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3']
jettriglist = ['HLT_Jet30U_v3']
trigpath = ''
#trigpath = 'inclusive_mu'
isData=True
isFastSim=False
trigMenu = 'HLT'
applyResJEC=True
addPF2PAT=True
isAOD=True
storeOutPath = False

# start process configuration -------------------------------------------------
process.setName_(procName)
#process.GlobalTag.globaltag = cms.string( 'GR_R_42_V12::All' )
process.GlobalTag.globaltag = cms.string( 'GR_R_42_V19::All' )

# configure the extra modules -------------------------------------------------
if(addPF2PAT):
    print "**** Adding PF2PAT objects ****"
    addpf2PatSequence(process, not isData)
defineBasePreSelection(process,False,False,True,trigpath,egtriglist,mutriglist,jettriglist)

#tau stuff
configureTauProduction(process, not isData)

addJetMETExtra(process,isData,applyResJEC,isAOD)
addTriggerMatchExtra(process,egtriglist,mutriglist,jettriglist,addPF2PAT,trigMenu)


# add the analysis modules ----------------------------------------------------
process.load('MiniTree.Selection.selection_cfi')
process.myMiniTreeProducer.MCTruth.isData = cms.bool(isData)
process.myMiniTreeProducer.MCTruth.sampleCode = cms.string('DATA')
process.myMiniTreeProducer.Taus.sources = cms.VInputTag("patTausHpsPFTau", "patTausPFlow")
process.myMiniTreeProducer.minEventQualityToStore = cms.int32(2)
process.myMiniTreeProducer.Trigger.source = cms.InputTag('TriggerResults::'+trigMenu)
process.myMiniTreeProducer.Trigger.bits = cms.vstring()
process.myMiniTreeProducer.Trigger.bits = mutriglist
process.myMiniTreeProducer.Trigger.bits.extend( egtriglist )
process.myMiniTreeProducer.Trigger.bits.extend( jettriglist )

########################################################

# analysis sequence ------------------------------------------------------------
process.tau_extra = cms.Path(process.PFTau)
process.jet_extra = cms.Path(process.FastJetSequence)

process.p  = cms.Path( process.basePreSel*process.myMiniTreeProducer)

if( addPF2PAT ):
    process.pat_default = cms.Path( process.patSequence * process.patDefaultSequence )
else :
    process.pat_default = cms.Path( process.patDefaultSequence )

process.schedule = cms.Schedule(process.jet_extra, process.tau_extra, process.pat_default, process.p)
checkProcessSchedule(storeOutPath,True)

if(isAOD) :
    print "**** This is AOD run ****"
    from PhysicsTools.PatAlgos.tools.coreTools import *
    restrictInputToAOD(process)
    process.myMiniTreeProducer.Electrons.ebRecHits = cms.InputTag("reducedEcalRecHitsEB")
    process.myMiniTreeProducer.Electrons.eeRecHits = cms.InputTag("reducedEcalRecHitsEE")
