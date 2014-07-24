import FWCore.ParameterSet.Config as cms


#my base values for trigger bit selection -------------------
BaseTriggerSet = cms.PSet( source = cms.InputTag("TriggerResults::HLT"),
                           bits = cms.vstring('HLT_Mu9','HLT_Ele10_LW_L1R_v2','HLT_Jet30'),
                           )

#my base values for vertex selection ---------------------------------
BaseVertexSet = cms.PSet( vertexSource = cms.InputTag("offlinePrimaryVertices"),
                          maxZ = cms.double(24),
                          maxRho = cms.double(2.0),
                          minNDOF = cms.int32(4),
                          beamSpotSource = cms.InputTag("offlineBeamSpot"),
                          useBeamSpot = cms.bool(True)
                          )

#my base values for tracks selection ----------------------------------
BaseTrackSet = cms.PSet( source = cms.InputTag("generalTracks"),
                         dedxSource = cms.InputTag("dedxHarmonic2"),
                         minPt = cms.double(1),
                         maxEta = cms.double(2.5),
                         minQuality = cms.int32(2),
                         maxD0 = cms.double(400),
                         maxTrackChi2 = cms.double(10),
                         minTrackValidHits = cms.int32(5),
                         minPixelHits = cms.int32(1),
                         minPrimaryVertexWeight = cms.double(0),
                         minDeltaRtoLepton = cms.double(0.1),
                         trackIsoCone = cms.double(0.3),
                         maxPtSumInTrackIsoCone = cms.double(10)
                         )



#my base values for muon selection ---------------------------------------
BaseMuonsSet =  cms.PSet( sources = cms.VInputTag("selectedPatMuons","selectedPatMuonsPFlow"),
                          dedxSource = cms.InputTag("dedxHarmonic2"),
                          triggerEvent = cms.InputTag("patTriggerEvent"),
                          triggerMatch = cms.string("TrigMatch"),
                          minPt = cms.double(10),
                          maxEta = cms.double(2.4),
                          onlyGlobal = cms.bool(True),
                          id = cms.string('GlobalMuonPromptTight'),
                          maxD0 = cms.double(200),
                          maxRelIso = cms.double(0.3),
                          useDefaultIso = cms.bool(True),
                          maxTrackChi2 = cms.double(10),
                          minMuonHits = cms.int32(0),
                          minPixelHits = cms.int32(0),
                          minMatchStations = cms.int32(1),
                          minTrackerLayers = cms.int32(5)
                          )



# base values for electron selection ----------------------------------------------
BaseElectronsSet =  cms.PSet(sources = cms.VInputTag("selectedPatElectrons", "selectedPatElectronsPFlow"),
                             ebRecHits = cms.InputTag("ecalRecHit:EcalRecHitsEB"),
                             eeRecHits = cms.InputTag("ecalRecHit:EcalRecHitsEE"),
                             dedxSource = cms.InputTag("dedxHarmonic2"),
                             triggerEvent = cms.InputTag("patTriggerEvent"),
                             triggerMatch = cms.string("TrigMatch"),
                             minEt = cms.double(10),
                             minSCEt = cms.double(10),
                             maxEta = cms.double(2.5),
                             ecalOnly = cms.bool(True),
                             id = cms.string('mvaTrigV0'),
                             maxD0 = cms.double(400),
                             maxRelIso = cms.double(0.25),
                             useDefaultIso = cms.bool(True),
                             maxTrackLostHits = cms.int32(1),
                             minDeltaRtoMuons = cms.double(0.1),
                             minSigmaIetaIeta = cms.double(0.002),
                             minS4S1=cms.double(0.05),
                             rho = cms.InputTag("kt6PFJets", "rho")
                             )

#my base values for jet selection -----------------------------------------------
BaseJetsSet = cms.PSet(sources = cms.VInputTag("selectedPatJets","selectedPatJetsPFlow","selectedPatJetsResCor"),
                       CaloJetId = cms.PSet( version = cms.string("PURE09"), quality = cms.string("LOOSE") ),
                       PFJetId = cms.PSet( version = cms.string("FIRSTDATA"), quality = cms.string("LOOSE") ),
                       dedxSource = cms.InputTag("dedxHarmonic2"),
                       triggerEvent = cms.InputTag("patTriggerEvent"),
                       triggerMatch = cms.string("TrigMatch"),
                       useRawJets = cms.bool(False),
                       minPt = cms.double(17),
                       maxEta = cms.double(2.5),
                       minDeltaRtoLepton = cms.double(0.4),
                       puMVADiscriminant = cms.InputTag("puJetMva:fullDiscriminant"),
                       puMVAID = cms.InputTag("puJetMva:fullId"),
                       puMVADiscriminantResCor = cms.InputTag("puJetMvaResCor:fullDiscriminant"),
                       puMVAIDResCor = cms.InputTag("puJetMvaResCor:fullId")
                       )

#my base values for tau selection -----------------------------------------------
BaseTausSet = cms.PSet(sources = cms.VInputTag("selectedPatTaus", "selectedPatTausPFlow"),
                       minPt                    = cms.double(10.0),
                       maxEta                   = cms.double(2.4),    
                       minDeltaRtoLeptons       = cms.double(0.3), 
                       maxDeltaRtoOriginJet     = cms.double(0.3)
                       )


#my base values for met selection ------------------------------------------------
BaseMetsSet = cms.PSet(sources = cms.VInputTag("patMETs","patMETsPFlow","patPfMetT0pcT1Txy"),
                       minMET = cms.double(10)
                       )

#my MC truth matching sets -------------------------------------------------------
BaseMCTruthSet = cms.PSet( isData = cms.bool(False),
                           producePDFweights = cms.bool(False),
                           sampleCode = cms.string("SEECODES"),
                           jpMatchSources = cms.VInputTag("selectedPatJetsByRef", "selectedPatJetsAK5JPTByRef", "selectedPatJetsAK5PFByRef", "selectedPatJetsPFlowByRef")
                           )
#values for kine fit object collection ------------------------------------------------
BaseKFPSet = cms.PSet(sources = cms.VInputTag("kinFitTtSemiLepEvent:Leptons","kinFitTtSemiLepEvent:Neutrinos","kinFitTtSemiLepEvent:PartonsHadB","kinFitTtSemiLepEvent:PartonsHadP","kinFitTtSemiLepEvent:PartonsHadQ","kinFitTtSemiLepEvent:PartonsLepB","kinFitTtSemiLepEventJESUp:Leptons","kinFitTtSemiLepEventJESUp:Neutrinos","kinFitTtSemiLepEventJESUp:PartonsHadB","kinFitTtSemiLepEventJESUp:PartonsHadP","kinFitTtSemiLepEventJESUp:PartonsHadQ","kinFitTtSemiLepEventJESUp:PartonsLepB","kinFitTtSemiLepEventJESDown:Leptons","kinFitTtSemiLepEventJESDown:Neutrinos","kinFitTtSemiLepEventJESDown:PartonsHadB","kinFitTtSemiLepEventJESDown:PartonsHadP","kinFitTtSemiLepEventJESDown:PartonsHadQ","kinFitTtSemiLepEventJESDown:PartonsLepB","kinFitTtSemiLepEventJERUp:Leptons","kinFitTtSemiLepEventJERUp:Neutrinos","kinFitTtSemiLepEventJERUp:PartonsHadB","kinFitTtSemiLepEventJERUp:PartonsHadP","kinFitTtSemiLepEventJERUp:PartonsHadQ","kinFitTtSemiLepEventJERUp:PartonsLepB","kinFitTtSemiLepEventJERDown:Leptons","kinFitTtSemiLepEventJERDown:Neutrinos","kinFitTtSemiLepEventJERDown:PartonsHadB","kinFitTtSemiLepEventJERDown:PartonsHadP","kinFitTtSemiLepEventJERDown:PartonsHadQ","kinFitTtSemiLepEventJERDown:PartonsLepB"),
                      njetsUsed = cms.InputTag("kinFitTtSemiLepEvent:NumberOfConsideredJets"),
                      chi2OfFit = cms.InputTag("kinFitTtSemiLepEvent:Chi2"),
                      probOfFit = cms.InputTag("kinFitTtSemiLepEvent:Prob"),
                      statusOfFit = cms.InputTag("kinFitTtSemiLepEvent:Status"),
                      njetsUsedUp = cms.InputTag("kinFitTtSemiLepEventJESUp:NumberOfConsideredJets"),
                      chi2OfFitUp = cms.InputTag("kinFitTtSemiLepEventJESUp:Chi2"),
                      probOfFitUp = cms.InputTag("kinFitTtSemiLepEventJESUp:Prob"),
                      statusOfFitUp = cms.InputTag("kinFitTtSemiLepEventJESUp:Status"),
                      njetsUsedDown = cms.InputTag("kinFitTtSemiLepEventJESDown:NumberOfConsideredJets"),
                      chi2OfFitDown = cms.InputTag("kinFitTtSemiLepEventJESDown:Chi2"),
                      probOfFitDown = cms.InputTag("kinFitTtSemiLepEventJESDown:Prob"),
                      statusOfFitDown = cms.InputTag("kinFitTtSemiLepEventJESDown:Status"),
                      njetsUsedJERUp = cms.InputTag("kinFitTtSemiLepEventJERUp:NumberOfConsideredJets"),
                      chi2OfFitJERUp = cms.InputTag("kinFitTtSemiLepEventJERUp:Chi2"),
                      probOfFitJERUp = cms.InputTag("kinFitTtSemiLepEventJERUp:Prob"),
                      statusOfFitJERUp = cms.InputTag("kinFitTtSemiLepEventJERUp:Status"),
                      njetsUsedJERDown = cms.InputTag("kinFitTtSemiLepEventJERDown:NumberOfConsideredJets"),
                      chi2OfFitJERDown = cms.InputTag("kinFitTtSemiLepEventJERDown:Chi2"),
                      probOfFitJERDown = cms.InputTag("kinFitTtSemiLepEventJERDown:Prob"),
                      statusOfFitJERDown = cms.InputTag("kinFitTtSemiLepEventJERDown:Status"),
                      runKineFitter = cms.bool(True)
                      )
