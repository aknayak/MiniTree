
import FWCore.ParameterSet.Config as cms

from MiniTree.Selection.LocalRunSkeleton_cff import *


#tau stuff
from MiniTree.Selection.TauExtra_cff import *
#PFlow
from MiniTree.Selection.pfToPatSequences_cff import *


process.maxEvents.input = cms.untracked.int32(200)
process.TFileService.fileName = cms.string('mc_tau.root')

# config parameters ------------------------------------------------------------
procName='LOCALUSER'
process.source.fileNames = ["file:/lustre/lip.pt/data/cmslocal/samples/CMSSW_4_2_X/mc/TT_TuneZ2_7TeV-pythia6-tauola-AODSIM/FEEE3638-F297-E011-AAF8-00304867BEC0.root"]
trigMenu = 'HLT'
isData=False
isAOD=True
isFastsim = False
#mutriglist = [ 'HLT_Mu15_v2' ]
mutriglist = [ 'HLT_IsoMu17_v5' ]
egtriglist = [ 'HLT_Ele27_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_v2']
jettriglist = [ 'HLT_Jet30_v1' ]
trigpath = ''
applyResJEC=False
addPF2PAT=True
storeOutPath=False

# start process configuration -------------------------------------------------
process.setName_(procName)
producePDFweights=False
process.GlobalTag.globaltag = cms.string( 'START42_V13::All' )


# configure the extra modules -------------------------------------------------
if(addPF2PAT):
    print "**** Adding PF2PAT objects ****"
    addpf2PatSequence(process, not isData)
defineBasePreSelection(process,False,False,not isFastsim and not isAOD)

#tau stuff
configureTauProduction(process, not isData)

addJetMETExtra(process,isData,applyResJEC,isAOD)
addTriggerMatchExtra(process,egtriglist,mutriglist,jettriglist,False,trigMenu)
defineGenUtilitiesSequence(process)

# add the analysis modules ----------------------------------------------------
process.load('MiniTree.Selection.selection_cfi')
process.myMiniTreeProducer.MCTruth.isData = cms.bool(isData)
process.myMiniTreeProducer.MCTruth.sampleCode = cms.string('TTBAR')
process.myMiniTreeProducer.MCTruth.producePDFweights = cms.bool(producePDFweights)
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
