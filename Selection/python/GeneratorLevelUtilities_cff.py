import FWCore.ParameterSet.Config as cms


def defineGenUtilitiesSequence(process) :

    # Produce PDF weights (maximum is 3)
    process.pdfWeights = cms.EDProducer("PdfWeightProducer",
                                        # Fix POWHEG if buggy (this PDF set will also appear on output,
                                        # so only two more PDF sets can be added in PdfSetNames if not "")
                                        #FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),
                                        #GenTag = cms.untracked.InputTag("genParticles"),
                                        PdfInfoTag = cms.untracked.InputTag("generator"),
                                        PdfSetNames = cms.untracked.vstring("cteq66.LHgrid" ) )
    
    process.genUtilsSequence = cms.Path( process.pdfWeights )
