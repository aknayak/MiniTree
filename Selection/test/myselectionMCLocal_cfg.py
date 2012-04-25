
import FWCore.ParameterSet.Config as cms

from MiniTree.Selection.LocalRunSkeleton_cff import *


process.maxEvents.input = cms.untracked.int32(500)
process.TFileService.fileName = cms.string('mc_tau.root')

# config parameters ------------------------------------------------------------
procName='LOCALUSER'
#process.source.fileNames = ["file:/sps/cms/anayak/LocalData/relval-CMSSW523/relvalTTBar-CMSSW523-AODSIM-START52_V5-v1_numEvent1000.root"]
process.source.fileNames = ["file:/sps/cms/anayak/LocalData/WToTauNu_TuneZ2star_8TeV_pythia6_tauola-U_S7_START50_V15-v1_numEvent10000.root"]
trigMenu = 'HLT'
isData=False
isAOD=True
isFastsim = False
#mutriglist = [ 'HLT_Mu15_v2' ]
mutriglist = [ 'HLT_IsoMu24_eta2p1_v8' ]
egtriglist = [ 'HLT_Ele27_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_v2']
jettriglist = [ 'HLT_Jet30_v1' ]
trigpath = ''
applyResJEC=False
addPF2PAT=True
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

#tau stuff
configureTauProduction(process, not isData)
configurePrePatMuon(process)
configurePatMuonUserPFIso(process)
configureDiMuonVetoFilter(process)

if(addPF2PAT):
    import PhysicsTools.PatAlgos.tools.helpers as patutils
    patutils.massSearchReplaceAnyInputTag(process.muonPFIsolationDepositsSequence, cms.InputTag('pfSelectedMuons'), cms.InputTag('muons'))
    
addJetMETExtra(process,isData,applyResJEC,isAOD)
addTriggerMatchExtra(process,egtriglist,mutriglist,jettriglist,False,trigMenu)
defineGenUtilitiesSequence(process)

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

########################################################

# analysis sequence ------------------------------------------------------------
process.tau_extra = cms.Path(process.PFTau)
process.muon_extra = cms.Path(process.produceMuonPFIsoPrePat)

process.p  = cms.Path(process.allEventsFilter*process.basePreSel*process.diMuVetoFilter*process.myMiniTreeProducer)
#process.p  = cms.Path( process.basePreSel*process.myMiniTreeProducer)

if( addPF2PAT ):
    process.pat_default = cms.Path( process.patSequence * process.patDefaultSequence )
else :
    process.pat_default = cms.Path( process.patDefaultSequence )

process.schedule = cms.Schedule(process.muon_extra, process.tau_extra, process.pat_default, process.p)
#process.schedule = cms.Schedule(process.tau_extra, process.pat_default, process.p)
checkProcessSchedule(storeOutPath,True)

if(isAOD) :
    print "**** This is AOD run ****"
    from PhysicsTools.PatAlgos.tools.coreTools import *
    restrictInputToAOD(process)
    process.myMiniTreeProducer.Electrons.ebRecHits = cms.InputTag("reducedEcalRecHitsEB")
    process.myMiniTreeProducer.Electrons.eeRecHits = cms.InputTag("reducedEcalRecHitsEE")
