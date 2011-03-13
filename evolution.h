#include<cstdlib>
#include<iostream>
#include<cmath>
using namespace std;

int GaussianWave(double sigma, double height, double k0, double* gridR, double* gridI, int length, double hx);

int Potential(double* V, int nx, double vh, double vw);

int GaussianCombine(double* meshXR, double* meshXI, double*, int nx);
