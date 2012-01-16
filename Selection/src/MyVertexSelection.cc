#include "MiniTree/Selection/interface/MyEventSelection.h"

std::vector<MyVertex> MyEventSelection::getVertices(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  
  std::vector<MyVertex> selVertices; 
  selVertices.clear();

  try{
    bestPrimVertex_ = 0;
    refPoint_ = math::XYZPoint(0,0,0);

    //config parameters
    edm::InputTag vtxSource = configParamsVertex_.getParameter<edm::InputTag>("vertexSource");
    edm::InputTag bsSource = configParamsVertex_.getParameter<edm::InputTag>("beamSpotSource");
    double maxZ = configParamsVertex_.getParameter<double>("maxZ");
    double maxRho = configParamsVertex_.getParameter<double>("maxRho");
    int minNDOF = configParamsVertex_.getParameter<int>("minNDOF");

    edm::Handle<reco::VertexCollection>vtx_;
    iEvent.getByLabel(vtxSource, vtx_);
    std::vector<const reco::Vertex *> selVtx; selVtx.clear();
    for(size_t ivtx = 0; ivtx < vtx_->size(); ivtx++)
      {
	const reco::Vertex *vIt = &((*vtx_)[ivtx]);
	
	//base quantities
	bool isReal = !(vIt->isFake());
	double z = fabs(vIt->z());
	double rho = vIt->position().Rho();
	int ndof = vIt->ndof();
	//double chi2 = vIt->chi2()/ndof;
	if( isReal && z < maxZ && rho < maxRho && ndof >= minNDOF )
	  selVtx.push_back(vIt);
      }
    std::sort(selVtx.begin(), selVtx.end(), &sumPtOrder);
    //select the best primary vertex (highest sum pT of tracks)
    if(selVtx.size())bestPrimVertex_ = selVtx[0];

    //assign the reference point
    iEvent.getByLabel( bsSource, beamSpot_);
    bool useBeamSpot = configParamsVertex_.getParameter<bool>("useBeamSpot");
    if(bestPrimVertex_ && !useBeamSpot) 
      {
	refPoint_ = bestPrimVertex_->position();
	refVertex_ = *bestPrimVertex_;
      }
    else
      {
	refPoint_ = beamSpot_->position();
	const reco::BeamSpot &bs = *(beamSpot_.product());
	reco::Vertex bsVtx( bs.position(), bs.covariance3D() );
	refVertex_ = bsVtx;
      }

    for(size_t ivtx = 0; ivtx < selVtx.size(); ivtx++)
      {
	const reco::Vertex *vIt = selVtx[ivtx];
	MyVertex newVertex = MyVertexConverter(*vIt);
	selVertices.push_back(newVertex);
      }
  }catch(std::exception &e){
    std::cout << "[Vertex Selection] : check selection " << e.what() << std::endl;
  }
  
  return selVertices;
}


MyVertex MyEventSelection::MyVertexConverter(const reco::Vertex& iVertex)
{
  MyVertex newVertex;
  newVertex.Reset();
  
  newVertex.XYZ.SetCoordinates(iVertex.x(), iVertex.y(), iVertex.z());
  newVertex.ErrXYZ.SetCoordinates(iVertex.xError(), iVertex.yError(), iVertex.zError());

  newVertex.chi2 = iVertex.chi2();
  newVertex.isValid = !(iVertex.isFake());
  newVertex.ndof = iVertex.ndof();
  newVertex.rho = iVertex.position().Rho();
  
  newVertex.normalizedChi2 = iVertex.chi2()/iVertex.ndof();
  double sumpt(0),ntracks(0),fracHighPurity(0);
  for(reco::Vertex::trackRef_iterator iTrack= iVertex.tracks_begin(); 
      iTrack != iVertex.tracks_end(); iTrack++)
    {
      ntracks++;
      sumpt += (*iTrack)->pt();
      fracHighPurity += (*iTrack)->quality(reco::TrackBase::highPurity);
    }
  if(ntracks) fracHighPurity /= ntracks;
  newVertex.NumberOfTracks =  ntracks;
  newVertex.fracHighPurity = fracHighPurity;
  newVertex.sumpt = sumpt;

  return newVertex;
}

bool MyEventSelection::sumPtOrder(const reco::Vertex *a, const reco::Vertex *b)
{
  double sumpta(0);
  for(reco::Vertex::trackRef_iterator iTrack=a->tracks_begin();
      iTrack != a->tracks_end();
      iTrack++)
    sumpta += (*iTrack)->pt();
  
  double sumptb(0);
  for(reco::Vertex::trackRef_iterator iTrack=b->tracks_begin();
      iTrack != b->tracks_end();
      iTrack++)
    sumptb += (*iTrack)->pt();
  
  return sumpta>sumptb;
}
