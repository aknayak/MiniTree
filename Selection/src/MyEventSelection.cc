#include "MiniTree/Selection/interface/MyEventSelection.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "DataFormats/JetReco/interface/JPTJet.h"

MyEventSelection::MyEventSelection(const edm::ParameterSet& iConfig)  
{
  configParamsVertex_ = iConfig.getParameter<edm::ParameterSet>("Vertex");
  configParamsJets_ = iConfig.getParameter<edm::ParameterSet>("Jets");
  configParamsMETs_ = iConfig.getParameter<edm::ParameterSet>("Mets");
  configParamsMuons_ = iConfig.getParameter<edm::ParameterSet>("Muons");
  configParamsElectrons_ = iConfig.getParameter<edm::ParameterSet>("Electrons");
  configParamsTaus_ = iConfig.getParameter<edm::ParameterSet>("Taus");
  configParamshlt_ = iConfig.getParameter<edm::ParameterSet>("Trigger");
  configParamsMC_ = iConfig.getParameter<edm::ParameterSet>("MCTruth");

  std::string code = configParamsMC_.getParameter<std::string>("sampleCode");
  if(code!=std::string("DATA")) { isData_=false; }
  else{ isData_=true; inputDataSampleCode_ = MyEvent::DATA; }

  if(code==std::string("TTBAR")){ inputDataSampleCode_ = MyEvent::TTBAR;  }
  if(code==std::string("ZJETS")){ inputDataSampleCode_ = MyEvent::ZJETS;  }
  if(code==std::string("WJETS")){ inputDataSampleCode_ = MyEvent::WJETS;  }
  if(code==std::string("TOPS")) { inputDataSampleCode_ = MyEvent::TOPS;   }
  if(code==std::string("TOPT")) { inputDataSampleCode_ = MyEvent::TOPT;   }
  if(code==std::string("TOPW")) { inputDataSampleCode_ = MyEvent::TOPW;   }
  if(code==std::string("QCD" )) { inputDataSampleCode_ = MyEvent::QCD;    }
  if(code==std::string(""))     { inputDataSampleCode_ = MyEvent::OTHER;  }
  if(code==std::string("WW"))   { inputDataSampleCode_ = MyEvent::WW;     }
  if(code==std::string("WZ"))   { inputDataSampleCode_ = MyEvent::WZ;     }
  if(code==std::string("ZZ"))   { inputDataSampleCode_ = MyEvent::ZZ;     }

  //initialize pileup re-weighting method
  //get the pu vectors
  //data
  double PUDataDist_truth[60] = {
    3.44995e-09, 1.1359e-08, 2.61411e-07, 0.00391959, 0.0592632, 0.00226285, 0.000113361, 0.000143113, 8.95836e-05, 0.000105439, 0.00089546, 0.00809503, 0.0256425, 0.04022, 0.0502341, 0.0658163, 0.0880936, 0.102146, 0.0980098, 0.0814968, 0.0633475, 0.0503196, 0.0436695, 0.0408493, 0.0381886, 0.0337634, 0.0278815, 0.0217244, 0.0162019, 0.0116874, 0.00819999, 0.00560731, 0.00373748, 0.00242552, 0.00152978, 0.000935589, 0.000553594, 0.000316269, 0.000174158, 9.23184e-05, 4.70655e-05, 2.30644e-05, 1.08615e-05, 4.91498e-06, 2.13744e-06, 8.93537e-07, 3.59185e-07, 1.38889e-07, 5.16801e-08, 1.8511e-08, 6.38428e-09, 2.12062e-09, 6.78483e-10, 2.09101e-10, 6.20715e-11, 1.77456e-11, 4.88498e-12, 1.2945e-12, 3.30127e-13, 8.09973e-14
  };
  double PUDataDist_observed[60] = {
    0.000788272, 0.00344101, 0.00757879, 0.0112497, 0.012756, 0.0120619, 0.0105258, 0.00977743, 0.0108973, 0.0142354, 0.0195895, 0.0264344, 0.0340818, 0.0417866, 0.0488345, 0.0546237, 0.0587299, 0.060942, 0.0612614, 0.0598677, 0.0570639, 0.0532135, 0.0486841, 0.0438056, 0.0388466, 0.0340074, 0.0294246, 0.0251826, 0.0213262, 0.0178732, 0.0148227, 0.0121621, 0.00987093, 0.00792303, 0.0062886, 0.00493542, 0.00383014, 0.00293945, 0.00223124, 0.0016755, 0.00124496, 0.000915572, 0.000666594, 0.000480585, 0.000343181, 0.000242783, 0.000170194, 0.000118245, 8.14343e-05, 5.56015e-05, 3.76426e-05, 2.5272e-05, 1.68274e-05, 1.11136e-05, 7.28103e-06, 4.73229e-06, 3.05159e-06, 1.95251e-06, 1.23966e-06, 7.81077e-07
  };
  //MC
  double Summer2012_truth[60] = {
    2.344E-05, 2.344E-05, 2.344E-05, 2.344E-05, 4.687E-04, 4.687E-04, 7.032E-04, 9.414E-04, 1.234E-03, 1.603E-03, 2.464E-03, 3.250E-03, 5.021E-03, 6.644E-03, 8.502E-03, 1.121E-02, 1.518E-02, 2.033E-02, 2.608E-02, 3.171E-02, 3.667E-02, 4.060E-02, 4.338E-02, 4.520E-02, 4.641E-02, 4.735E-02, 4.816E-02, 4.881E-02, 4.917E-02, 4.909E-02, 4.842E-02, 4.707E-02, 4.501E-02, 4.228E-02, 3.896E-02, 3.521E-02, 3.118E-02, 2.702E-02, 2.287E-02, 1.885E-02, 1.508E-02, 1.166E-02, 8.673E-03, 6.190E-03, 4.222E-03, 2.746E-03, 1.698E-03, 9.971E-04, 5.549E-04, 2.924E-04, 1.457E-04, 6.864E-05, 3.054E-05, 1.282E-05, 5.081E-06, 1.898E-06, 6.688E-07, 2.221E-07, 6.947E-08, 2.047E-08 };  
  double Summer2012_observed[] = {
    0.00001, 0.00001, 0.0001, 0, 0.0008, 0.0005, 0.0012, 0.0024, 0.0033, 0.0026, 0.0052, 0.0053, 0.009, 0.0096, 0.0148, 0.0189, 0.0195, 0.0237, 0.0264, 0.0268, 0.0345, 0.0349, 0.0394, 0.0384, 0.0447, 0.043, 0.041, 0.0427, 0.0433, 0.0394, 0.04, 0.0427, 0.039, 0.0366, 0.0351, 0.0309, 0.0253, 0.0265, 0.0235, 0.0217, 0.018, 0.0133, 0.0134, 0.0129, 0.009, 0.0074, 0.0067, 0.0058, 0.0061, 0.0026, 0.0022, 0.0023, 0.002, 0.0018, 0.001, 0.0005, 0.0006, 0.0004, 0.0006, 0.0005};

  std::vector< float > MCPUDist, MCPUTrueGen;
  std::vector< float > DataPUDist, DataPUTrueDist;
  for( int i=0; i<60; ++i) {
    DataPUDist.push_back(PUDataDist_observed[i]); 
    DataPUTrueDist.push_back(PUDataDist_truth[i]);
    MCPUDist.push_back(Summer2012_observed[i]); 
    MCPUTrueGen.push_back(Summer2012_truth[i]);
  }
  /*
  for( int i=0; i<25; ++i) {
    DataPUDist.push_back(TrueDist_f[i]);
    MCPUTrueGen.push_back(mc_truegen_f[i]);
    if(inputDataSampleCode_ == MyEvent::TTBAR)MCPUDist.push_back(mc_ttbar_f[i]);
    else if(inputDataSampleCode_ == MyEvent::WJETS)MCPUDist.push_back(mc_wjets_f[i]);
    else if(inputDataSampleCode_ == MyEvent::ZJETS)MCPUDist.push_back(mc_zjets_f[i]);
    else if(inputDataSampleCode_ == MyEvent::TOPS || 
	    inputDataSampleCode_ == MyEvent::TOPT ||
            inputDataSampleCode_ == MyEvent::TOPW)MCPUDist.push_back(mc_sTop_f[i]);
    else if(inputDataSampleCode_ == MyEvent::QCD)MCPUDist.push_back(mc_qcd_f[i]);
    else if(inputDataSampleCode_ == MyEvent::WW)MCPUDist.push_back(mc_ww_f[i]);
    else if(inputDataSampleCode_ == MyEvent::WZ)MCPUDist.push_back(mc_wz_f[i]);
    else if(inputDataSampleCode_ == MyEvent::ZZ)MCPUDist.push_back(mc_zz_f[i]);
    else{MCPUDist.push_back(TrueDist_f[i]);}
  }
  */
  LumiWeights_ = edm::LumiReWeighting(MCPUDist, DataPUDist);
  LumiWeightsDefault_ = edm::LumiReWeighting(MCPUTrueGen, DataPUTrueDist);

    
}

MyEventSelection::~MyEventSelection()
{

}

void MyEventSelection::Set(const edm::Event& e, const edm::EventSetup& es)
{

  es.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilder);

  event_.runNb = e.id().run();
  event_.eventNb = e.id().event();
  event_.lumiblock = e.luminosityBlock();
  event_.isData = isData_;
  
  
  //get trigger
  event_.hlt = getHLT(e, es);
  //get vertex
  event_.PrimaryVtxs = getVertices(e, es);
  //get muon
  event_.Muons = getMuons(e, es);
  //std::cout<<" pass muon "<<event_.Muons.size()<<std::endl;
  //get electron
  event_.Electrons = getElectrons(e, es);
  //std::cout<<"pass electron "<<event_.Electrons.size()<<std::endl;
   event_.Taus = getTaus(e, es);
  //std::cout<<" pass tau "<<event_.Taus.size()<<std::endl;
  //get Jets
  event_.Jets = getJets(e, es);
  //std::cout<<"pass jet "<<event_.Jets.size()<<std::endl;
  //get mets
  event_.mets = getMETs(e, es);
  //std::cout<<"pass met "<<event_.mets.size()<<std::endl;
  if(!isData_){
    event_.mcParticles = getMCParticles(e);
    event_.mcMET = mcMET;
    event_.sampleInfo = getSampleInfo(e, es);
  }

  //make event selection
  bool passTrig = false;
  std::vector<std::string> trigs = event_.hlt;
  for(size_t itrig = 0; itrig < trigs.size(); itrig++){
    if(trigs[itrig].find("Ele") != std::string::npos)passTrig = true;
    if(trigs[itrig].find("Mu") != std::string::npos)passTrig = true;
  }
  int nIsoMuon = 0, nIsoElectron = 0;
  std::vector<MyElectron> electrons = event_.Electrons;
  for(size_t iele = 0; iele < electrons.size(); iele++){
    std::string algo = electrons[iele].name;
    if(algo.find("PFlow") == std::string::npos) continue;
    bool passKin = false, passId = false, passIso = false;
    int quality = electrons[iele].quality;
    if(quality & 0x1)passKin = true;
    if((quality >> 1) & 0x1)passId = true;
    if((quality >> 2) & 0x1)passIso = true;
    if(passKin && passId && passIso){
     myhistos_["SelElePt"]->Fill(electrons[iele].p4.Pt());
     myhistos_["SelEleEta"]->Fill(electrons[iele].p4.Eta());
      nIsoElectron++;
    }
  }
  myhistos_["SelEleMultiplicity"]->Fill(nIsoElectron);

  std::vector<MyMuon> muons = event_.Muons;
  for(size_t imu = 0; imu < muons.size(); imu++){
    std::string algo = muons[imu].name;
    //if(algo.find("PFlow") == std::string::npos) continue;
    bool passKin = false, passId = false, passIso = false;
    int quality = muons[imu].quality;
    if(quality & 0x1)passKin = true;
    if((quality >> 1) & 0x1)passId = true;
    if((quality >> 2) & 0x1)passIso = true;
    if(passKin && passId && passIso){
      myhistos_["SelMuPt"]->Fill(muons[imu].p4.Pt());
      myhistos_["SelMuEta"]->Fill(muons[imu].p4.Eta());
      nIsoMuon++;
    }
  }
  myhistos_["SelMuMultiplicity"]->Fill(nIsoMuon);

  int nIsoLepton = nIsoMuon + nIsoElectron;

  std::vector<MyJet> jets = event_.Jets;
  int nJets = 0, nHighPtJets = 0;
  for(size_t ijet = 0; ijet < jets.size(); ijet++){
    std::string algo = jets[ijet].jetName;
    if(algo.find("PFlow") == std::string::npos) continue;
    bool passKin = false, passId = false;
    int quality = jets[ijet].quality;
    if(quality & 0x1)passKin = true;
    if((quality >> 1) & 0x1)passId = true;
    if(passKin && passId){
      myhistos_["SelJetPt"]->Fill(jets[ijet].p4.Pt());
      myhistos_["SelJetEta"]->Fill(jets[ijet].p4.Eta());
      nJets++;
      if(jets[ijet].p4.Pt() > 30)nHighPtJets++;
    }
  }
  myhistos_["SelJetMultiplicity"]->Fill(nJets);

  int EventQuality = 0;
  if(passTrig){
    EventQuality++;
    if(nIsoLepton > 0){
      EventQuality++;
      if(nJets > 0){
	EventQuality++;
	if(nHighPtJets > 0){
	  EventQuality++;
	  if(nHighPtJets > 1){
	    EventQuality++;
	  }
	}
      }
    }
  }

  for(int istep = 0; istep <= EventQuality; istep++){
    myhistos_["cutflow"]->Fill(istep);
  }

  event_.eventQuality = EventQuality;
  fs_->cd();
}


//void MyEventSelection::BookHistos(edm::Service<TFileService> tfs_)
void MyEventSelection::BookHistos()
{

  //book histograms
  initTriggerHistos_ = true;
  
  //myhistos_["cutflow"] = fs_->make<TH1D>("cutflow", "cutflow", 20, 0., 20.);
  //myhistos_["pfJetPt"] = fs_->make<TH1D>("pfJetPt", "pfJetPt", 100, 0., 500.);
  
  //selection
  dirs_.push_back( fs_->mkdir("selection") );
  myhistos_["cutflow"] = dirs_[dirs_.size() - 1].make<TH1D>("cutflow", "cutflow", 10, 0., 10.);
  myhistos2_["cutflowmctruth"] = dirs_[dirs_.size() - 1].make<TH2D>("cutflowmctruth", "cutflow", 10, 0., 10., 18, 0., 18.);
  TString steps[6] = {"reco","trigger","#geq 1 leptons","#geq 1 jet","#geq 1 jet (pT > 25)", "#geq 2 jets (pT > 25)"};
  TString ttch[18] = {"unk.", "full had","e+jets","#mu+jets","#tau+jets","ee","e#mu","e#tau","#mu#mu","#mu#tau","#tau#tau", "z+jets","z#tau#tau","w+jets","top-s", "top-t", "top-w","qcd"};
  const size_t nsteps = sizeof(steps)/sizeof(TString);
  for(uint istep=0; istep<nsteps; istep++ ){
    myhistos2_["cutflowmctruth"]->GetXaxis()->SetBinLabel(istep+1, steps[istep]);
    myhistos_["cutflow"]->GetXaxis()->SetBinLabel(istep+1, steps[istep]);
  }
  for(int ich=0;   ich<18;  ich++  ){
    myhistos2_["cutflowmctruth"]->GetYaxis()->SetBinLabel(ich+1, ttch[ich]);
  }

  myhistos_["SelJetPt"]  = dirs_[dirs_.size() - 1].make<TH1D>("SelJetPt", "jet Pt;jet p_{T} [GeV/c];N_{events}",200, 0, 400.);
  myhistos_["SelJetEta"]  = dirs_[dirs_.size() - 1].make<TH1D>("SelJetEta", "jet Eta;jet #eta;N_{events}",100, -5.0, 5.0);
  myhistos_["SelJetMultiplicity"]  = dirs_[dirs_.size() - 1].make<TH1D>("SelJetMultiplicity", "jet Multiplicity;jet Multiplicit;N_{events}",20, 0, 20);
  myhistos_["SelElePt"]  = dirs_[dirs_.size() - 1].make<TH1D>("SelElePt", "electron Pt;electron p_{T} [GeV/c];N_{events}",200, 0, 400.);
  myhistos_["SelEleEta"]  = dirs_[dirs_.size() - 1].make<TH1D>("SelEleEta", "electron Eta;electron #eta;N_{events}",100, -5.0, 5.0);
  myhistos_["SelEleMultiplicity"]  = dirs_[dirs_.size() - 1].make<TH1D>("SelEleMultiplicity", "electron Multiplicity;electron Multiplicity;N_{events}",20, 0, 20);
  myhistos_["SelMuPt"]  = dirs_[dirs_.size() - 1].make<TH1D>("SelMuPt", "muon Pt;muon p_{T} [GeV/c];N_{events}",200, 0, 400.);
  myhistos_["SelMuEta"]  = dirs_[dirs_.size() - 1].make<TH1D>("SelMuEta", "muon Eta;muon #eta;N_{events}" ,100, -5.0, 5.0);
  myhistos_["SelMuMultiplicity"]  = dirs_[dirs_.size() - 1].make<TH1D>("SelMuMultiplicity", "muon Multiplicity;muon Multiplicity;N_{events}",20, 0, 20);

  //MC infor
  dirs_.push_back( fs_->mkdir("MCINFO") );
  myhistos_["intimepu"] = dirs_[dirs_.size() - 1].make<TH1D>("intimepu", "intime pileup", 101, -0.5, 100.5);
  myhistos_["outoftimepu"] = dirs_[dirs_.size() - 1].make<TH1D>("outoftimepu", "out of time pileup", 101, -0.5, 100.5);
  myhistos_["totalpu"] = dirs_[dirs_.size() - 1].make<TH1D>("totalpu", "total pileup", 101, -0.5, 100.5);

  //Jets
  std::vector<edm::InputTag> sources = configParamsJets_.getParameter<std::vector<edm::InputTag> >("sources");
  for(std::vector<edm::InputTag>::iterator sit = sources.begin();
      sit != sources.end();
      sit++)
    {
      TString rawtag=sit->label();
      rawtag.ReplaceAll("pat","");
      rawtag.ReplaceAll("cleanPat","");
      rawtag.ReplaceAll("selectedPat","");
      std::string tag(rawtag);
      
      //Make a new TDirectory
      dirs_.push_back( fs_->mkdir(tag.c_str()) );
	  
      myhistos_["pt_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("pt_"+rawtag, "Jet Pt", 200, 0., 500.);
      myhistos_["lowpt_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("lowpt_"+rawtag, "Jet Pt", 200, 0., 100.);
      myhistos_["eta_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("eta_"+rawtag, "Jet #eta", 100, -5.0, 5.0);
      myhistos_["phi_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("phi_"+rawtag, "Jet #phi", 80, -4.05, 3.95);
      myhistos_["emf_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("emf_"+rawtag, "Jet emf", 120, 0, 1.02);
      myhistos_["nconstituents_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("nconstituents_"+rawtag, "Jet nconstituents", 50, 0, 50.);
      myhistos_["ntracks_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("ntracks_"+rawtag, "Jet ntracks", 50, 0, 50.);
    }

  //Electrons
  sources = configParamsElectrons_.getParameter<std::vector<edm::InputTag> >("sources");
  for(std::vector<edm::InputTag>::iterator sit = sources.begin();
      sit != sources.end();
      sit++)
    {
      TString rawtag=sit->label();
      rawtag.ReplaceAll("pat","");
      rawtag.ReplaceAll("cleanPat","");
      rawtag.ReplaceAll("selectedPat","");
      std::string tag(rawtag);
      
      //Make a new TDirectory
      dirs_.push_back( fs_->mkdir(tag.c_str()) );
	  
      myhistos_["pt_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("pt_"+rawtag, "Electron Pt", 200, 0., 500.);
      myhistos_["lowpt_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("lowpt_"+rawtag, "Electron Pt", 200, 0., 100.);
      myhistos_["eta_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("eta_"+rawtag, "Electron #eta", 60, -3.0, 3.0);
      myhistos_["phi_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("phi_"+rawtag, "Electron #phi", 80, -4.05, 3.95);
      
      myhistos_["cic_id_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("cic_id_"+rawtag, "Electron CiC id", 25, 0., 25.);
      myhistos_["vbtf_id_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("vbtf_id_"+rawtag, "Electron VBTF id", 25, 0., 25.);
      myhistos_["reliso_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("reliso_"+rawtag, "Electron reliso", 100, 0, 5.);
      myhistos_["lowreliso_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("lowreliso_"+rawtag, "Electron lowreliso", 100, 0, 1.);
    }
  //Muons
  sources = configParamsMuons_.getParameter<std::vector<edm::InputTag> >("sources");
  for(std::vector<edm::InputTag>::iterator sit = sources.begin();
      sit != sources.end();
      sit++)
    {
      TString rawtag=sit->label();
      rawtag.ReplaceAll("pat","");
      rawtag.ReplaceAll("cleanPat","");
      rawtag.ReplaceAll("selectedPat","");
      std::string tag(rawtag);
      
      //Make a new TDirectory
      dirs_.push_back( fs_->mkdir(tag.c_str()) );
	  
      myhistos_["pt_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("pt_"+rawtag, "Muon Pt", 200, 0., 500.);
      myhistos_["lowpt_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("lowpt_"+rawtag, "Muon Pt", 200, 0., 100.);
      myhistos_["eta_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("eta_"+rawtag, "Muon #eta", 60, -3.0, 3.0);
      myhistos_["phi_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("phi_"+rawtag, "Muon #phi", 80, -4.05, 3.95);
      myhistos_["d0_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("d0_"+rawtag, "Muon d0", 100, 0, 2000.);
      myhistos_["trackChi2_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("trackChi2_"+rawtag, "Muon track Chi2/dof", 200, 0, 40.);
      myhistos_["nHits_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("nHits_"+rawtag, "Muon track Hits", 50, 0, 50.);
      myhistos_["nMuonHits_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("nMuonHits_"+rawtag, "Muon track muon Hits", 50, 0, 50.);
      myhistos_["id_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("id_"+rawtag, "Muon id", 20, 0, 20.);
      myhistos_["reliso_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("reliso_"+rawtag, "Muon reliso", 100, 0, 5.);
      myhistos_["lowreliso_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("lowreliso_"+rawtag, "Muon lowreliso", 100, 0, 1.);
      myhistos_["relpfiso_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("relpfiso_"+rawtag, "Muon pf reliso", 100, 0, 5.); 
      myhistos_["lowrelpfiso_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("lowrelpfiso_"+rawtag, "Muon pf lowreliso", 100, 0, 1.);
    }
  //Taus
  sources = configParamsTaus_.getParameter<std::vector<edm::InputTag> >("sources");
  for(std::vector<edm::InputTag>::iterator sit = sources.begin();
      sit != sources.end();
      sit++)
    {
      TString rawtag=sit->label();
      rawtag.ReplaceAll("pat","");
      rawtag.ReplaceAll("cleanPat","");
      rawtag.ReplaceAll("selectedPat","");
      std::string tag(rawtag);
      
      //Make a new TDirectory
      dirs_.push_back( fs_->mkdir(tag.c_str()) );
	  
      myhistos_["pt_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("pt_"+rawtag, "Tau Pt", 200, 0., 500.);
      myhistos_["lowpt_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("lowpt_"+rawtag, "Tau Pt", 200, 0., 100.);
      myhistos_["eta_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("eta_"+rawtag, "Tau eta", 60, -3.0, 3.0);
      myhistos_["phi_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("phi_"+rawtag, "Tau phi", 80, -4.05, 3.95);
    }
  //METs
  sources = configParamsMETs_.getParameter<std::vector<edm::InputTag> >("sources");
  for(std::vector<edm::InputTag>::iterator sit = sources.begin();
      sit != sources.end();
      sit++)
    {
      TString rawtag=sit->label();
      rawtag.ReplaceAll("pat","");
      rawtag.ReplaceAll("cleanPat","");
      rawtag.ReplaceAll("selectedPat","");
      std::string tag(rawtag);
      
      //Make a new TDirectory
      dirs_.push_back( fs_->mkdir(tag.c_str()) );
	  
      myhistos_["pt_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("met_"+rawtag, "MET", 200, 0., 500.);
      myhistos_["lowpt_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("lowmet_"+rawtag, "MET", 200, 0., 100.);
      myhistos_["sumet_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("sumet_"+rawtag, "sum Et", 200, 0., 500.);
      myhistos_["phi_"+rawtag] = dirs_[dirs_.size() - 1].make<TH1D>("phi_"+rawtag, "MET", 80, -4.05, 3.95);
    }
}
