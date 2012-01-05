import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.metTools import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
        
def addJetMETExtra(process, jetcorr='Spring10', isData=False, isFastsim=False, applyResJEC=False, addPF2PAT=False,isAOD=False) :

    process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
    ##-------------------- Disable the CondDB for the L1FastJet (until they are included in a new global tag) -------
    ##process.ak5PFL1Fastjet.useCondDB = False  //only for old globaltag (38x)
    ##-------------------- Import the Jet RECO modules -----------------------
    process.load('RecoJets.Configuration.RecoPFJets_cff')
    ##-------------------- Turn-on the FastJet density calculation -----------------------
    process.kt6PFJets.doRhoFastjet = True
    ##-------------------- Turn-on the FastJet jet area calculation for your favorite algorithm -----------------------
    process.ak5PFJets.doAreaFastjet = True
    process.FastJetSequence = cms.Sequence(process.kt6PFJets * process.ak5PFJets)
    # Re run the kt6PFJet clustering to calculate rho, for lepton isolation
    # this produces a double in the event like double_kt6PFJetsForIso_rho__PROCESS
    process.kt6PFJetsForIso = process.kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
    process.kt6PFJetsForIso.Rho_EtaMax = cms.double(2.5)
    process.kt6PFJetsForIso.Ghost_EtaMax = cms.double(2.5)
    process.FastJetSequence = cms.Sequence(process.FastJetSequence + process.kt6PFJetsForIso )
    
    if(isData) :
        corrections = ['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual', 'L5Flavor', 'L7Parton']
        #corrections = ['L1FastJet','L2Relative','L3Absolute','L2L3Residual']
        runOnData(process, ['All'])
    else :
        corrections = ['L1FastJet','L2Relative','L3Absolute','L5Flavor','L7Parton']
    if( isAOD ) : process.patJets.addTagInfos   = cms.bool(False)

    #from PhysicsTools.PatAlgos.recoLayer0.jetCorrFactors_cfi import *
    process.patJetCorrFactors.levels = ['L1Offset', 'L2Relative', 'L3Absolute']
    #patJetCorrFactors.levels = corrections
    #process.patJetCorrFactors.useRho = cms.bool(True)
    
    print "*** Adding PF ak5 jets ***"
    addJetCollection(process,cms.InputTag('ak5PFJets'),
                     'AK5', 'PF',
                     doJTA        = True,
                     doBTagging   = True,
                     jetCorrLabel = ('AK5PF',corrections),
                     doType1MET   = False,
                     doL1Cleaning = False,
                     doL1Counters = True,
                     genJetCollection = cms.InputTag("ak5GenJets"),
                     doJetID      = True,
                     jetIdLabel   = "ak5"
                     )
    process.patJetCorrFactorsAK5PF.useRho = True
    if( isAOD ) : process.patJetsAK5PF.addTagInfos
    
    ##print "*** Adding JPT ak5 jets ***"
    ##addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
    ##                 'AK5', 'JPT',
    ##                 doJTA        = True,
    ##                 doBTagging   = True,
    ##                 jetCorrLabel = ('AK5JPT', corrections),
    ##                 doType1MET   = False,
    ##                 doL1Cleaning = False,
    ##                 doL1Counters = True,
    ##                 genJetCollection = cms.InputTag("ak5GenJets"),
    ##                 doJetID      = True,
    ##                 jetIdLabel   = "ak5"
    ##                 )
    ##process.load('RecoJets.JetPlusTracks.JetPlusTrackCorrections_cff')
    ##process.JetPlusTrackZSPCorJetAntiKt5.ResponseMap = cms.string('CondFormats/JetMETObjects/data/CMSSW_362_resptowers.txt')
    ##if( isAOD ) : process.patJetsAK5JPT.addTagInfos
    ##
    ##print "*** Adding TC MET ***"
    ##addTcMET(process, 'TC')

    print "*** Adding PF MET ***"
    addPfMET(process, 'PF')
            
            
