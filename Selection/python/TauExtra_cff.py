import FWCore.ParameterSet.Config as cms
import copy

from PhysicsTools.PatAlgos.tools.tauTools import *
from PhysicsTools.PatAlgos.tools.jetTools import *


def configureTauProduction(process, isMC=False) :
    #Re-process PFTau to give ak5pfjets as input to pftau, only for 36x MC data
    process.load("RecoTauTag.Configuration.RecoPFTauTag_cff")
    #Re-process TCTau to get the DiscriminationAgainstMuon
    process.load("RecoTauTag.Configuration.RecoTCTauTag_cff")
    #process.load("RecoTauTag.RecoTau.CaloRecoTauDiscriminationAgainstMuon_cfi")
    if(not isMC):
        removeMCMatching(process,['Taus'])
    switchToPFTauHPS(process)
    #switchToPFTauShrinkingCone(process)
    #addTauCollection(process,
    #                 tauCollection = cms.InputTag('caloRecoTauProducer'),
    #                 algoLabel = "caloReco",
    #                 typeLabel = "Tau"
    #                 )
#    addTauCollection(process,
#                     tauCollection = cms.InputTag('hpsPFTauProducer'),
#                     algoLabel = "hps",
#                     typeLabel = "PFTau"
#                     )
#    addTauCollection(process,
#                     tauCollection = cms.InputTag('hpsTancTaus'),
#                     algoLabel = "hpsTanc",
#                     typeLabel = "PFTau"
#                     )
