#include <fstream>
#include "evolution.h"


int main(int argc, char *argv[]){
  int c; //the character used to process commandline argument
  double hx, ht; // increment for time and space
  int nx, nt; // length for space and time boundary
  double vh, vw; // potential height and width
  double sigma, height, k0; // gaussian wave packet width and height
  nx = 50;
  hx = 1;
  ht = 1;
  vw = 3;
  vh = 1;
  nt = 9;
 
  nx *= 2; // we make the meshgrid center on 0;
 
  double* meshXI = (double *)malloc(nx*sizeof(double)); //image part of meshgrid
  double* meshXR = (double *)malloc(nx*sizeof(double)); //real part of meshgrid
  double* V = (double *)malloc(nx*sizeof(double)); //Potential grid
  while ((c = getopt(argc, argv, "n:h:e:b:por")) != -1)
    switch(c)
      {
      case 'h':
	break;
      case 'e':
	break;
      case 'p':
	break;
      case 'b':
	break;
      case 'o':
	break;
      case 'r':
	break;
      } // process commandline arguments
  
  sigma = 10;
  height = 1;
  k0 = 1;
  
  GaussianWave(sigma, height, k0, meshXR, meshXI, nx, hx);
 // construct a gaussian wavepacket
  Potential(V, nx, vh, vw);
  // construct a potential step

  
 
  for (int i=1; i<nt-1; i++){
    for (int j=0; j<nx; j++){
      meshXI[j] += ht/(2*hx*hx) * (meshXR[j+1]-2*meshXR[j]+meshXR[j-1])-ht*V[j]*meshXR[j];
      meshXR[j] += -1*ht/(2*hx*hx) * (meshXI[j+1]-2*meshXR[j]+meshXR[j-1])+ht*V[j]*meshXI[j];
    }
  }

  ofstream fp;
  fp.open("plot.dat");
  for (int j=0; j<nx; j++){
    fp<<j<<"   "<<meshXR[j]<<endl;
  }
  fp.close();
  FILE *pipe = popen("gnuplot -persist","w");
  fflush(pipe);
  fprintf(pipe,"set xrange [0:%d]\n",nx);
  fflush(pipe);
  fprintf(pipe,"set yrange [-6:6]\n");
  fflush(pipe);
  fprintf(pipe,"plot 'plot.dat' with lines\n");
  fflush(pipe);

  return 0;
}
