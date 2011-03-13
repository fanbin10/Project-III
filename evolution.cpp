#include "evolution.h"
#include <iomanip>


int GaussianWave(double sigma, double height, double k0, double* gridR, double* gridI, int length, double hx){
  double x;
  for (int i=0; i<length; i++){
    x = abs(length/2-i)*hx;
    gridR[i] = height*exp(-1.0*(x*x)/(2*sigma*sigma))*cos(k0*x);
    gridI[i] = height*exp(-1.0*(x*x)/(2*sigma*sigma))*sin(k0*x);
  }
  return 0;
};

int Potential(double* V, int nx, double vh, double vw){
  for (int i=0; i<nx; i++)
    V[i] = 0;
  // initialize potential grid

  for (int i=floor(nx*0.75); i< floor(nx*0.75)+vw; i++ )
    V[i] = vh;
  return 0;
}
