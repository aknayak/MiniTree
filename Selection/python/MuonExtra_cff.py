import FWCore.ParameterSet.Config as cms
import copy
from CommonTools.ParticleFlow.pfParticleSelection_cff import *


def configurePrePatMuon(process) :
    process.selectedPrimaryVertexQuality = cms.EDFilter("VertexSelector",
                                                        src = cms.InputTag('offlinePrimaryVertices'),
                                                        cut = cms.string("isValid & ndof >= 4 & chi2 > 0 & tracksSize > 0"), # CV: cut >= 4 if using 'offlinePrimaryVertices', # >= 7 if using 'offlinePrimaryVerticesWithBS' as input
                                                        filter = cms.bool(False)
                                                        )
    process.selectedPrimaryVertexPosition = cms.EDFilter("VertexSelector",
                                                         src = cms.InputTag('selectedPrimaryVertexQuality'),
                                                         cut = cms.string("abs(z) < 24 & abs(position.Rho) < 2."),
                                                         filter = cms.bool(False)
                                                         )
    process.load("CommonTools.ParticleFlow.pfNoPileUp_cff")
    process.pfPileUp.Enable = cms.bool(True)
    process.pfPileUp.checkClosestZVertex = cms.bool(True)
    process.pfPileUp.Vertices = cms.InputTag('selectedPrimaryVertexPosition')
    process.load("CommonTools.ParticleFlow.pfParticleSelection_cff")

    process.pfPileUp.PFCandidates = 'particleFlow'
    process.pfNoPileUp.bottomCollection = 'particleFlow'
    process.pfPileUpIso.PFCandidates = 'particleFlow'
    process.pfNoPileUpIso.bottomCollection='particleFlow'
    
    process.load("RecoMuon.MuonIsolation.muonPFIsolation_cff")
    import PhysicsTools.PatAlgos.tools.helpers as patutils
    patutils.massSearchReplaceAnyInputTag(process.muonPFIsolationDepositsSequence, cms.InputTag('muons1stStep'), cms.InputTag('muons'))
    process.produceMuonPFIsoPrePat = cms.Sequence(
        process.selectedPrimaryVertexQuality
        * process.selectedPrimaryVertexPosition
        * process.pfNoPileUpSequence
        * process.pfParticleSelectionSequence
        * process.muonPFIsolationDepositsSequence)
    

def configurePatMuonUserPFIso(process):
    print 'adding PF isolation to PAT Muons'
    process.patMuons.isoDeposits = cms.PSet(
        # CV: strings for IsoDeposits defined in PhysicsTools/PatAlgos/plugins/PATMuonProducer.cc
        pfChargedHadrons = cms.InputTag("muPFIsoDepositCharged"),
        pfNeutralHadrons = cms.InputTag("muPFIsoDepositNeutral"),
        pfPhotons = cms.InputTag("muPFIsoDepositGamma"),
        user = cms.VInputTag(
            cms.InputTag("muPFIsoDepositChargedAll"),
            cms.InputTag("muPFIsoDepositPU")
            )
        )
    
    process.patMuons.userIsolation = cms.PSet(
        # CV: strings for Isolation values defined in PhysicsTools/PatAlgos/src/MultiIsolator.cc
        pfChargedHadron = cms.PSet(
            deltaR = cms.double(0.4),
            src = process.patMuons.isoDeposits.pfChargedHadrons,
            vetos = process.muPFIsoValueCharged04.deposits[0].vetos,
            skipDefaultVeto = process.muPFIsoValueCharged04.deposits[0].skipDefaultVeto
            ),
        pfNeutralHadron = cms.PSet(
            deltaR = cms.double(0.4),
            src = process.patMuons.isoDeposits.pfNeutralHadrons,
            vetos = process.muPFIsoValueNeutral04.deposits[0].vetos,
            skipDefaultVeto = process.muPFIsoValueNeutral04.deposits[0].skipDefaultVeto
            ),
        pfGamma = cms.PSet(
            deltaR = cms.double(0.4),
            src = process.patMuons.isoDeposits.pfPhotons,
            vetos = process.muPFIsoValueGamma04.deposits[0].vetos,
            skipDefaultVeto = process.muPFIsoValueGamma04.deposits[0].skipDefaultVeto
            ),
        user = cms.VPSet(
            cms.PSet(
                deltaR = cms.double(0.4),
                src = process.patMuons.isoDeposits.user[0],
                vetos = process.muPFIsoValueChargedAll04.deposits[0].vetos,
                skipDefaultVeto = process.muPFIsoValueChargedAll04.deposits[0].skipDefaultVeto
            ),
            cms.PSet(
                deltaR = cms.double(0.4),
                src = process.patMuons.isoDeposits.user[1],
                vetos = process.muPFIsoValuePU04.deposits[0].vetos,
                skipDefaultVeto = process.muPFIsoValuePU04.deposits[0].skipDefaultVeto
                )
            )
        )
    
def configureDiMuonVetoFilter(process) :
    process.globalMuons = cms.EDFilter("PATMuonSelector",
                               src = cms.InputTag('patMuons'),
                               cut = cms.string("isGlobalMuon"),
                               filter = cms.bool(False)
                               )
    
    process.diMuonVeto = cms.EDFilter("PATCandViewCountFilter",
                              src = cms.InputTag('globalMuons'),
                              minNumber = cms.uint32(1),
                              maxNumber = cms.uint32(1)
                              )
    process.diMuVetoFilter = cms.Sequence(process.globalMuons*process.diMuonVeto)
    
