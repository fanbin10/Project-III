#include <fstream>
#include "evolution.h"


int main(int argc, char *argv[]){
  int flag = 0;
  int c; //the character used to process commandline argument
  double hx, ht; // increment for time and space
  int nx, nt; // length for space and time boundary
  double vh, vw; // potential height and width
  double sigma, height, k0; // gaussian wave packet width and height
  double energy=0.501;
  nx = 200;
  hx = 1;
  ht = 0.02;
  vw = 5;
  vh = 1;

 
  nx *= 2; // we make the meshgrid center on 0;
 
  double* meshXI = (double *)malloc(nx*sizeof(double)); //image part of meshgrid
  double* meshXR = (double *)malloc(nx*sizeof(double)); //real part of meshgrid
  double* meshX = (double *)malloc(nx*sizeof(double)); 
  double* V = (double *)malloc(nx*sizeof(double)); //Potential grid

  while ((c = getopt(argc, argv, "n:h:e:s:w:por")) != -1)
    switch(c)
      {
      case 'n':
	nt = atof(optarg);
	break;
      case 'h':
	vh = atof(optarg);
	break;
      case 'e':
	energy = atof(optarg);
	break;
      case 's':
	sigma = atof(optarg);
	break;
      case 'p':				      
	flag = 1;
	break;
      case 'w':
	vw = atof(optarg);
	break;
      case 'o':
	break;
      case 'r':
	break;
      } // process commandline arguments
  
  sigma = 5;
  height = 1;
  k0 = sqrt(2*energy-1.0/(2*sigma*sigma));
  nt = (int)30*nx*1.0/k0;
 
  
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
  
  cout<<"Simulation running..."<<endl;
  FILE *pipe = popen("gnuplot -persist","w");
  fflush(pipe);
  for (int i=0; i<nt-1; i++){
    for (int j=1; j<nx; j++){
       meshXI[j] += 1.0*ht/(2*hx*hx) * (meshXR[j+1]-2*meshXR[j]+meshXR[j-1])-ht*V[j]*meshXR[j];
      meshXR[j] += -1.0*ht/(2*hx*hx) * (meshXI[j+1]-2*meshXI[j]+meshXI[j-1])+ht*V[j]*meshXI[j];
      GaussianCombine(meshXR, meshXI, meshX, nx);
    }

    if ( (flag==1) && (i%50==0) ){
      fp.open("plot.dat");
      for (int m=0; m<nx; m++){
	fp<<m<<"   "<<meshX[m]<<endl;
      }
      fp.close();
    }
    if ( (flag==1) && (i%50==0) ){
      
      fprintf(pipe,"set multiplot\n");
      fflush(pipe);
      fprintf(pipe,"set xrange [0:%d]\n",nx);
      fflush(pipe);
      fprintf(pipe,"set title 't=%d' \n", i);
      fflush(pipe);
      fprintf(pipe,"plot 'V.dat' with lines\n");
      fflush(pipe);
      fprintf(pipe,"plot 'plot.dat' with lines\n");
      fflush(pipe);
      fprintf(pipe,"set nomultiplot\n");
      fflush(pipe);
      //sleep(0.5);
    }
  }
  double reflect=0;
  double transmit=0;
  for (int i=0; i<nx/2+50; i++){
    reflect += meshX[i];
  }
  for (int i=nx/2+50+vw; i<nx; i++){
    transmit += meshX[i];
  }

  free(meshXR);
  free(meshXI);
  free(meshX); // clean our workspace

  cout<<"------------------Summary----------------------"<<endl;
  cout<<"Particle Energy: " << energy<<"   (-e)"<<endl;
  cout<<"Wavepacket sigma: "<< sigma<<"   (-s)"<<endl;
  cout<<"Step potential Height: "<<vh<<"   (-h)"<<endl;
  cout<<"Step potential Width: "<<vw<<"   (-w)"<<endl;
  cout<<"Simulation steps: "<<nt<<"   (-n)"<<endl;
  cout<<"Bounced back fraction: "<<reflect/(reflect+transmit)<<endl;
  cout<<"Transmitted fraction: "<<transmit/(transmit+reflect)<<endl;
  cout<<"-----------------------------------------------"<<endl;
  return 0;
}
