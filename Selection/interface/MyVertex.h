#ifndef __MYVERTEX_H__
#define __MYVERTEX_H__

#include "TROOT.h"
#include <map>
#include <vector>
#include <string>

#include "MomentumVec.h"

class MyVertex
{
 public:
  MyVertex();
  ~MyVertex();
  
  void Reset();
  
  Point3D        XYZ;      //  position
  Point3D        ErrXYZ;   //  error position
  
  double chi2;
  int isFake;
  int isValid;
  double ndof;
  double rho;  // for maxd0 selection (<2)
  
  double normalizedChi2;
  size_t NumberOfTracks;
  double fracHighPurity;
  double sumpt;

 private :

};
#endif
