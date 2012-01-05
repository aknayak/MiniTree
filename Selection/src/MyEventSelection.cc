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
  float TrueDist_f[25] = {
    0.00592951, 0.0255428, 0.0591468, 0.097016, 0.126287, 0.138848, 0.134117, 0.11691, 0.0937398, 0.0700927, 0.0493627, 0.0329741, 0.0209976, 0.0127917, 0.00747402, 0.00419649, 0.00226774, 0.00118102, 0.000593481, 0.000288109, 0.000135272, 6.14976e-05, 2.71017e-05, 1.15906e-05, 4.81577e-06};

  //MC
  float mc_truegen_f[25] = {
    0.0698146584, 0.0698146584, 0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584 /* <-- 10*/, 0.0630151648,0.0526654164,0.0402754482,0.0292988928,0.0194384503,0.0122016783,0.007207042,0.004003637,0.0020278322,0.0010739954,0.0004595759,0.0002229748,0.0001028162,4.58337152809607E-05 /* <-- 24 */};

  float mc_ttbar_f[25] = {
    0.117198, 0.0638211, 0.0698478, 0.0698528, 0.0688964, 0.0685752, 0.066122, 0.0646321, 0.0621532, 0.0586756, 0.052647, 0.0466854, 0.040664, 0.033903, 0.0281996, 0.0226153, 0.0177697, 0.013507, 0.0102728, 0.00745686, 0.0053866, 0.00371641, 0.0026116, 0.00172162, 0.00112084}; //summer11
  float mc_wjets_f[25] = {
    0.0766293, 0.0735828, 0.069928, 0.0777541, 0.0702916, 0.0716951, 0.0724031, 0.0713587, 0.073293, 0.0740002, 0.0681019, 0.0587143, 0.0492786, 0.0343848, 0.0224241, 0.0155293, 0.00978994, 0.00634288, 0.0030619, 0.000802916, 0.000230819, 0.000131179, 5.75906e-05, 0.000111525, 0.000102383};
  float mc_zjets_f[25] = {
    0.07755, 0.0732363, 0.069868, 0.0769772, 0.0708263, 0.0722797, 0.0725818, 0.0702651, 0.0727378, 0.0748525, 0.0682161, 0.0582509, 0.0493266, 0.0342148, 0.0222552, 0.0157219, 0.0100292, 0.00647099, 0.00303309, 0.000813923, 0.000159433, 4.77009e-05, 6.57498e-05, 0.000110013, 0.000109583};
    float mc_qcd_f[25] = {
    0.0726378, 0.0747552, 0.0689556, 0.0760957, 0.0716543, 0.0756597, 0.0751914, 0.0728996, 0.0707219, 0.0729673, 0.0700738, 0.0594349, 0.0484091, 0.0333147, 0.0229574, 0.0145763, 0.00902782, 0.00617539, 0.00287905, 0.000970002, 0.000302527, 0.000162392, 3.91681e-05, 7.22435e-05, 6.68967e-05};
  float mc_sTop_f[25] = {
    0.0802195, 0.0776887, 0.0678381, 0.0752282, 0.0728507, 0.0726313, 0.0728869, 0.0673992, 0.0713914, 0.0736879, 0.0660656, 0.0575805, 0.0494001, 0.0354294, 0.0220041, 0.0164205, 0.010347, 0.00645701, 0.00301441, 0.000933082, 4.26065e-06, 4.68671e-05, 9.1604e-05, 0.000191729, 0.000191729};
  float mc_ww_f[25] = {
    0.0712905, 0.0748102, 0.0688297, 0.0756034, 0.072427, 0.0758105, 0.0746822, 0.0737018, 0.0716917, 0.0731857, 0.0699342, 0.0592673, 0.0483946, 0.0332349, 0.0226174, 0.014567, 0.00926215, 0.00616733, 0.00287556, 0.000978895, 0.000347318, 0.000154741, 3.88065e-05, 6.88816e-05, 5.82098e-05};
    float mc_wz_f[25] = {
    0.0757356, 0.0764022, 0.0682677, 0.0766057, 0.0728946, 0.0744608, 0.071986, 0.0718338, 0.0711525, 0.0732555, 0.0692824, 0.0597457, 0.0490703, 0.0326763, 0.0224811, 0.0146913, 0.00903123, 0.0058693, 0.00301548, 0.000973304, 0.000271498, 9.69975e-05, 4.2793e-05, 8.08313e-05, 7.70275e-05};
  float mc_zz_f[25] = {
    0.0748179, 0.0752953, 0.0688111, 0.0758676, 0.0713962, 0.074829, 0.0742284, 0.0724563, 0.0717228, 0.072567, 0.0694933, 0.059943, 0.0490344, 0.0328642, 0.0225583, 0.0144542, 0.00888811, 0.00606218, 0.00300999, 0.000999177, 0.000332828, 0.000175064, 4.63607e-05, 8.64939e-05, 6.08917e-05};
  
  std::vector< float > MCPUDist, MCPUTrueGen;
  std::vector< float > DataPUDist;
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

  LumiWeights_ = edm::LumiReWeighting(MCPUDist, DataPUDist);
  LumiWeightsDefault_ = edm::LumiReWeighting(MCPUTrueGen, DataPUDist);

    
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
    if(algo.find("PFlow") == std::string::npos) continue;
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
