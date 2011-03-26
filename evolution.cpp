#include "evolution.h"
#include <iomanip>


int GaussianWave(double sigma, double height, double k0, double* gridR, double* gridI, int length, double hx){
  double x;
  height = 1/(sqrt(2*3.1415)*sigma);
  for (int i=0; i<length; i++){
    x = (i-length/2)*hx;
    gridR[i] = height*exp(-1.0*(x*x)/(2*sigma*sigma))*cos(k0*x);
    gridI[i] = height*exp(-1.0*(x*x)/(2*sigma*sigma))*sin(k0*x);
  }
  return 0;
};

int Potential(double* V, int nx, double vh, double vw){
  for (int i=0; i<nx; i++)
    V[i] = 0;
  // initialize potential grid

  for (int i=nx/2+50; i<nx/2+50+vw; i++ )
    V[i] = vh;
  return 0;
}

int GaussianCombine(double* meshXR, double* meshXI, double* meshX, int nx){
  for (int i=0; i<nx; i++)
    meshX[i]=sqrt(pow(meshXR[i],2)+pow(meshXI[i],2) );
  return 0;
}
