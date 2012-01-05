#include "MiniTree/Selection/interface/MyMET.h"

MyMET::MyMET()
{
}

MyMET::~MyMET()
{
}
void MyMET::Reset()
{
  p4.SetCoordinates(0.0,0.0,0.0,0.0);
  metName = "";
  sumEt = -999.;
  metSignificance = -999.;
  emEtFraction = -999.;
  hadEtFraction = -999.;
  muonEtFraction = -999.;
  isPFMET = false;
  isCaloMET = false;
}
