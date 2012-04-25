import FWCore.ParameterSet.Config as cms

from MiniTree.Selection.LocalRunSkeleton_cff import *

procName = 'USER'
mutrigstring = '@mutrig'
mutriglist = mutrigstring.split('+')
egtrigstring = '@egtrig'
egtriglist = egtrigstring.split('+')
jettrigstring = '@jettrig'
jettriglist = jettrigstring.split('+')
trigpath = '@trigpath'
isData=True
isFastSim=False
trigMenu = 'HLT'
applyResJEC=True
addPF2PAT=True
isaod=@isaod
storeOutPath=@storeoutput


process.setName_(procName)
process.GlobalTag.globaltag = cms.string( '@globaltag' )


# data preselection -----------------------------------------------------------
if(addPF2PAT):
    print "**** Adding PF2PAT objects ****"
    addpf2PatSequence(process, not isData)
defineBasePreSelection(process,False,True,trigpath,egtriglist,mutriglist,jettriglist)
addJetMETExtra(process,isData, applyResJEC,isaod)
addTriggerMatchExtra(process,egtriglist,mutriglist,jettriglist,addPF2PAT,trigMenu)


#tau stuff
configureTauProduction(process, not isData)
#muon user iso
configurePrePatMuon(process)
configurePatMuonUserPFIso(process)
if(addPF2PAT):
    import PhysicsTools.PatAlgos.tools.helpers as patutils
    patutils.massSearchReplaceAnyInputTag(process.muonPFIsolationDepositsSequence, cms.InputTag('pfSelectedMuons'), cms.InputTag('muons'))
    

# analysis modules -------------------------------------------------------------
process.load("MiniTree.Selection.@myplugin_cfi")
process.myMiniTreeProducer.MCTruth.sampleCode = cms.string('@cs')

# e+jet vertex selection requirements
process.myMiniTreeProducer.Muons.id = cms.string('AllGlobalMuons')

# Default Taus
process.myMiniTreeProducer.Taus.sources = cms.VInputTag("patTaus", "patTausPFlow");

process.myMiniTreeProducer.minEventQualityToStore = cms.int32(2)
process.myMiniTreeProducer.MCTruth.isData = cms.bool(isData)
process.myMiniTreeProducer.Trigger.source = cms.InputTag("TriggerResults::"+trigMenu)
process.myMiniTreeProducer.Trigger.bits = mutriglist
process.myMiniTreeProducer.Trigger.bits.extend( egtriglist )
process.myMiniTreeProducer.Trigger.bits.extend( jettriglist )

# analysis sequence --------------------------------------------------------------
process.muon_extra = cms.Path(process.produceMuonPFIsoPrePat)

process.p  = cms.Path( process.basePreSel*process.myMiniTreeProducer)

if( addPF2PAT ):
    process.pat_default = cms.Path( process.patSequence * process.patDefaultSequence )
else :
    process.pat_default = cms.Path( process.patDefaultSequence )   

process.schedule = cms.Schedule(process.muon_extra, process.pat_default, process.p)
checkProcessSchedule(storeOutPath,True)

if(isaod) :
    print "**** This is AOD run ****"
    from PhysicsTools.PatAlgos.tools.coreTools import *
    restrictInputToAOD(process)
    process.myMiniTreeProducer.Electrons.ebRecHits = cms.InputTag("reducedEcalRecHitsEB")
    process.myMiniTreeProducer.Electrons.eeRecHits = cms.InputTag("reducedEcalRecHitsEE")

