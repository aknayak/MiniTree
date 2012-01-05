import FWCore.ParameterSet.Config as cms

procName='USERLOCAL'
mutrig = 'HLT_Mu9'
egtriglist = ['HLT_Ele15_SW_L1R']
jettrig = 'HLT_Jet30U'
trigpath = ''
runon35x = False
isData=True

# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *
process.setName_(procName)
process.GlobalTag.globaltag = cms.string( 'GR_R_36X_V11A::All' )

# add simple cut based electron identification to PAT
process.load('RecoEgamma.ElectronIdentification.simpleEleIdSequence_cff')
process.makePatElectrons = cms.Sequence( process.eIdSequence * process.simpleEleIdSequence * process.electronMatch * process.patElectrons )
process.patElectrons.electronIDSources.simpleEleId95relIso =cms.InputTag("simpleEleId95relIso")
process.patElectrons.electronIDSources.simpleEleId90relIso =cms.InputTag("simpleEleId90relIso")
process.patElectrons.electronIDSources.simpleEleId85relIso =cms.InputTag("simpleEleId85relIso")
process.patElectrons.electronIDSources.simpleEleId80relIso =cms.InputTag("simpleEleId80relIso")
process.patElectrons.electronIDSources.simpleEleId70relIso =cms.InputTag("simpleEleId70relIso")
process.patElectrons.electronIDSources.simpleEleId60relIso =cms.InputTag("simpleEleId60relIso")
process.patElectrons.electronIDSources.simpleEleId95cIso =cms.InputTag("simpleEleId95cIso")
process.patElectrons.electronIDSources.simpleEleId90cIso =cms.InputTag("simpleEleId90cIso")
process.patElectrons.electronIDSources.simpleEleId85cIso =cms.InputTag("simpleEleId85cIso")
process.patElectrons.electronIDSources.simpleEleId80cIso =cms.InputTag("simpleEleId80cIso")
process.patElectrons.electronIDSources.simpleEleId70cIso =cms.InputTag("simpleEleId70cIso")
process.patElectrons.electronIDSources.simpleEleId60cIso =cms.InputTag("simpleEleId60cIso")


#-- Meta data to be logged in DBS ---------------------------------------------
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.16 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/UserCode/psilva/TopDileptons/test/LocalDataRun_cfg.py,v $'),
    annotation = cms.untracked.string('top dileptons analysis')
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

# data preselection -----------------------------------------------------------
from MiniTree.Selection.DataPreselSequences_cff import *

# pat tuple modules -----------------------------------------------------------
from MiniTree.Selection.JetMETExtra_cff import *

#trigger match
from MiniTree.Selection.PATtriggerMatchExtra_cff import *

#generator level utils
from MiniTree.Selection.GeneratorLevelUtilities_cff import *

# input/output -----------------------------------------------------------------
from MiniTree.Selection.LocalSources_cff import *
process.source.fileNames = dilcands
process.maxEvents.input = cms.untracked.int32(-1)

process.out.fileName = 'data.root'
process.out.splitLevel = cms.untracked.int32(99)
process.out.overrideInputFileSplitLevels = cms.untracked.bool(True)
process.out.outputCommands = cms.untracked.vstring('keep *')

# tfileservice ------------------------------------------------------------------
process.load("PhysicsTools.UtilAlgos.TFileService_cfi")
process.TFileService.fileName = cms.string('Monitor.root')

# adds/removes the outpath
def checkProcessSchedule(storeOutPath=True, debug=True) :
    
    if( storeOutPath ) :
        process.schedule.append( process.outpath )
    else :
        del process.outpath
    if(debug): print process.schedule




