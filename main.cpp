#include <fstream>
#include "evolution.h"


int main(int argc, char *argv[]){
  int flag = 0;
  int c; //the character used to process commandline argument
  double hx, ht; // increment for time and space
  int nx, nt; // length for space and time boundary
  double vh, vw; // potential height and width
  double sigma, height, k0; // gaussian wave packet width and height
  nx = 50;
  hx = 1;
  ht = 0.1;
  vw = 5;
  vh = 0;
  nt = 400;
 
  nx *= 2; // we make the meshgrid center on 0;
 
  double* meshXI = (double *)malloc(nx*sizeof(double)); //image part of meshgrid
  double* meshXR = (double *)malloc(nx*sizeof(double)); //real part of meshgrid
  double* meshX = (double *)malloc(nx*sizeof(double)); 

  double* V = (double *)malloc(nx*sizeof(double)); //Potential grid
  while ((c = getopt(argc, argv, "n:h:e:b:por")) != -1)
    switch(c)
      {
      case 'h':
	break;
      case 'e':
	break;
      case 'p':				      
	flag = 1;
	break;
      case 'b':
	break;
      case 'o':
	break;
      case 'r':
	break;
      } // process commandline arguments
  
  sigma = 5;
  height = 1;
  k0 = 1;
  
  GaussianWave(sigma, height, k0, meshXR, meshXI, nx, hx);
  GaussianCombine(meshXR, meshXI, meshX,nx);
 // construct a gaussian wavepacket
  Potential(V, nx, vh, vw);
  // construct a potential step
  ofstream fp;
  fp.open("V.dat");
  for (int m=0; m<nx; m++){
    fp<<m<<"   "<<V[m]<<endl;
  }
  fp.close();
  
  
  FILE *pipe = popen("gnuplot -persist","w");
  fflush(pipe);
  for (int i=1; i<nt-1; i++){
    for (int j=0; j<nx; j++){
      meshXI[j] += ht/(2*hx*hx) * (meshXR[j+1]-2*meshXR[j]+meshXR[j-1])-ht*V[j]*meshXR[j];
      meshXR[j] += -1*ht/(2*hx*hx) * (meshXI[j+1]-2*meshXI[j]+meshXI[j-1])+ht*V[j]*meshXI[j];
      GaussianCombine(meshXR, meshXI, meshX, nx);
    }

    fp.open("plot.dat");
    for (int m=0; m<nx; m++){
      fp<<m<<"   "<<meshX[m]<<endl;
    }
    fp.close();
    if (flag==1){
      fprintf(pipe,"set xrange [0:%d]\n",nx);
      fflush(pipe);
      fprintf(pipe,"set title 't=%d' \n", i);
      fflush(pipe);
      fprintf(pipe,"plot 'plot.dat' with lines\n");
      fflush(pipe);
      sleep(1);
    }
  }

  free(meshXR);
  free(meshXI);
  free(meshX); // clean our workspace

  return 0;
}
