import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.pfTools import *
from PhysicsTools.PatAlgos.tools.trigTools import *
from RecoJets.Configuration.RecoPFJets_cff import kt6PFJets
from CommonTools.ParticleFlow.Tools.enablePileUpCorrection import enablePileUpCorrection
    
##
## adds pat sequence
##
def addpf2PatSequence(process, runOnMC) :
    
    postfix = "PFlow"

    #jet energy corrections
    jetAlgo='AK5'
    jecSetPF = jetAlgo+'PFchs'
    jecLevels=['L1FastJet','L2Relative','L3Absolute']
    if(not runOnMC) : jecLevels.append( 'L2L3Residual' )

    #start PF2PAT
    usePF2PAT(process,
              runPF2PAT=True,
              runOnMC=runOnMC,
              jetAlgo=jetAlgo,
              postfix=postfix,
              jetCorrections=(jecSetPF, jecLevels)
              )
    #enablePileUpCorrection( process, postfix=postfix, doRho=True)  #old
    enablePileUpCorrection( process, postfix=postfix)
                                                        
    #switch to hpsPFTau
    adaptPFTaus(process,"hpsPFTau",postfix=postfix)
    #remove tau selection
    getattr(process,"pfTaus"+postfix).discriminators = [cms.PSet(
                discriminator=cms.InputTag("pfTausBaseDiscriminationByDecayModeFinding"+postfix),
                selectionCut=cms.double(0.5) )]
    
    #disable mc matching for photons
    removeMCMatching(process,names=['Photons'],postfix=postfix)
    process.patElectronsPFlow.embedTrack=True
    process.patMuonsPFlow.embedTrack=True
#    process.patMETsPFlow.userFloats
           
    #configure top projections
    getattr(process,"pfNoPileUp"+postfix).enable = True
    getattr(process,"pfNoMuon"+postfix).enable = True
    getattr(process,"pfNoElectron"+postfix).enable = True
    getattr(process,"pfNoTau"+postfix).enable = False  #jets not cleaned for taus
    getattr(process,"pfNoJet"+postfix).enable = True
    getattr(process,"pfNoMuon"+postfix).verbose = False

    #fix isolation to use a cone of 0.3 for both electrons and muons
    applyPostfix(process,"isoValMuonWithNeutral",postfix).deposits[0].deltaR = cms.double(0.3)
    applyPostfix(process,"isoValMuonWithCharged",postfix).deposits[0].deltaR = cms.double(0.3)
    applyPostfix(process,"isoValMuonWithPhotons",postfix).deposits[0].deltaR = cms.double(0.3)
    applyPostfix(process,"isoValElectronWithNeutral",postfix).deposits[0].deltaR = cms.double(0.3)
    applyPostfix(process,"isoValElectronWithCharged",postfix).deposits[0].deltaR = cms.double(0.3)
    applyPostfix(process,"isoValElectronWithPhotons",postfix).deposits[0].deltaR = cms.double(0.3)

    applyPostfix(process,"pfIsolatedMuons",postfix).combinedIsolationCut = cms.double(9999.)
    applyPostfix(process,"pfIsolatedElectrons",postfix).combinedIsolationCut = cms.double(9999.)
    
    #CiC electron ID
    process.load( "RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_cfi" )
    process.eidCiCSequence = cms.Sequence(
        process.eidVeryLooseMC
        + process.eidLooseMC
        + process.eidMediumMC
        + process.eidTightMC
        + process.eidSuperTightMC
        + process.eidHyperTight1MC
        )
    applyPostfix( process, 'patElectrons', postfix ).electronIDSources = cms.PSet(
        eidVeryLooseMC = cms.InputTag("eidVeryLooseMC"),
        eidLooseMC = cms.InputTag("eidLooseMC"),
        eidMediumMC = cms.InputTag("eidMediumMC"),
        eidTightMC = cms.InputTag("eidTightMC"),
        eidSuperTightMC = cms.InputTag("eidSuperTightMC"),
        eidHyperTight1MC = cms.InputTag("eidHyperTight1MC")
        )
    #test for simpleEId
    applyPostfix( process, 'patElectrons', postfix ).electronIDSources.simpleEleId95relIso =cms.InputTag("simpleEleId95relIsoPFlow")
    applyPostfix( process, 'patElectrons', postfix ).electronIDSources.simpleEleId90relIso =cms.InputTag("simpleEleId90relIsoPFlow")
    applyPostfix( process, 'patElectrons', postfix ).electronIDSources.simpleEleId85relIso =cms.InputTag("simpleEleId85relIsoPFlow")
    applyPostfix( process, 'patElectrons', postfix ).electronIDSources.simpleEleId80relIso =cms.InputTag("simpleEleId80relIsoPFlow")
    applyPostfix( process, 'patElectrons', postfix ).electronIDSources.simpleEleId70relIso =cms.InputTag("simpleEleId70relIsoPFlow")
    applyPostfix( process, 'patElectrons', postfix ).electronIDSources.simpleEleId60relIso =cms.InputTag("simpleEleId60relIsoPFlow")
    applyPostfix( process, 'patElectrons', postfix ).electronIDSources.simpleEleId95cIso =cms.InputTag("simpleEleId95cIsoPFlow")
    applyPostfix( process, 'patElectrons', postfix ).electronIDSources.simpleEleId90cIso =cms.InputTag("simpleEleId90cIsoPFlow")
    applyPostfix( process, 'patElectrons', postfix ).electronIDSources.simpleEleId85cIso =cms.InputTag("simpleEleId85cIsoPFlow")
    applyPostfix( process, 'patElectrons', postfix ).electronIDSources.simpleEleId80cIso =cms.InputTag("simpleEleId80cIsoPFlow")
    applyPostfix( process, 'patElectrons', postfix ).electronIDSources.simpleEleId70cIso =cms.InputTag("simpleEleId70cIsoPFlow")
    applyPostfix( process, 'patElectrons', postfix ).electronIDSources.simpleEleId60cIso =cms.InputTag("simpleEleId60cIsoPFlow")
    ################
    
    # match the trigger information for selected pat leptons
    ##process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")
    ##process.triggerProducerPF = process.patTrigger.clone()
    ##setattr( process, 'patTrigger' + postfix, process.triggerProducerPF )
    ##process.muTriggerMatchPF = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR"
    ##                                           , src     = cms.InputTag( "patMuons" )
    ##                                           , matched = cms.InputTag( "patTrigger"+postfix )
    ##                                           , matchedCuts = cms.string( 'path( "HLT_*" )' )
    ##                                           , maxDPtRel = cms.double( 0.5 )
    ##                                           , maxDeltaR = cms.double( 0.5 )
    ##                                           , resolveAmbiguities    = cms.bool( True )
    ##                                           , resolveByMatchQuality = cms.bool( True )
    ##                                           )
    ##setattr( process, 'muTriggerMatch' + postfix, process.muTriggerMatchPF )
    ##process.eleTriggerMatchPF = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR"
    ##                                    , src     = cms.InputTag( "patElectrons" )
    ##                                    , matched = cms.InputTag( "patTrigger"+postfix )
    ##                                    , matchedCuts = cms.string( 'path( "HLT_*" )' )
    ##                                    , maxDPtRel = cms.double( 0.5 )
    ##                                    , maxDeltaR = cms.double( 0.5 )
    ##                                    , resolveAmbiguities    = cms.bool( True )
    ##                                    , resolveByMatchQuality = cms.bool( True )
    ##                                    )
    ##setattr( process, 'eleTriggerMatch' + postfix, process.eleTriggerMatchPF )
    ##switchOnTriggerMatching( process,
    ##                         triggerProducer = 'patTrigger' + postfix,
    ##                         triggerMatchers = [ 'muTriggerMatch'+postfix, 'eleTriggerMatch' + postfix ],
    ##                         sequence        = 'patPF2PATSequence' + postfix,
    ##                         postfix         = postfix )
    ##removeCleaningFromTriggerMatching( process, sequence = 'patPF2PATSequence' + postfix )
    
    
    #create the path
    process.patSequence = cms.Sequence(
        process.eidCiCSequence*
        getattr(process,"patPF2PATSequence"+postfix)
        )
    print " *** PAT path has been defined"
    


