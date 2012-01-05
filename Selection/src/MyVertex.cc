#include "MiniTree/Selection/interface/MyVertex.h"

MyVertex::MyVertex():
  chi2(-999),
  isFake(-1),
  isValid(-1),
  ndof(0),
  rho(-999),
  normalizedChi2(-999),
  NumberOfTracks(0),
  fracHighPurity(0),
  sumpt(0)
{
}

MyVertex::~MyVertex()
{
}

void MyVertex::Reset()
{
  chi2 = -999;
  isFake = -1;
  isValid = -1;
  ndof = 0;
  rho = -999;
  
  normalizedChi2 = -999.;
  NumberOfTracks = 0;
  fracHighPurity = 0;
  sumpt = 0;

  XYZ.SetCoordinates(0.0,0.0,0.0);
  ErrXYZ.SetCoordinates(0.0,0.0,0.0);
  

}
