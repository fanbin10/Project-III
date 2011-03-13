#include "shoot.h"
#include <iomanip>

double run_rk(double* meshP, double* meshV, double E, double h, int steps){
  double k1[2], k2[2], k3[2], k4[2];
  double U, W, V;
  double deep = 140;
  for (int i=0; i<steps-1; i++){
      U = meshP[i];
      W = meshV[i];
      
      k1[0] = W;
      k1[1] = U*(potential(i*h, deep)-E);

      k2[0] = k1[1]*h/2+W;
      V = potential( i*h+0.5*h, deep);
      k2[1] = (V-E) *(k1[0]*h/2+U);

      k3[0] = k2[1]*h/2+W;
      V = potential( i*h+0.5*h, deep);
      k3[1] = (V-E)*(k2[0]*h/2+U);
			  
      k4[0] = k3[1]*h+W;
      V = potential( i*h+h, deep);
      k4[1] = (V-E)*(k3[0]*h+U);
      
      meshP[i+1] = U + 1.0/6*h*(k1[0]+2*k2[0]+2*k3[0]+k4[0]);
      meshV[i+1] = W + 1.0/6*h*(k1[1]+2*k2[1]+2*k3[1]+k4[1]);
    }

  return meshP[steps-1];
};

double run_eu(double* meshP, double* meshV, double E, double h, int steps){
  double U, W, V;
  double deep = 140;
  for (int i=0; i<steps-1; i++){
    U = meshP[i];
    W = meshV[i];
    
    V = potential(i*h, deep);
    U += W*h;
    W += (V-E)*h*meshP[i];

    meshP[i+1] = U;
    meshV[i+1] = W;
  }
  return meshP[steps-1];
};



double potential(double x, double deep){
  if (x<1)
    return -1*deep;
  else
    return 0;
};


int search( double h, int steps, double resolv){
  int keyN = floor(1/resolv);
  int N=0;
  double  E;
  double deep = 140;
  double* keys = (double*)malloc(sizeof(double)*(keyN+1));
  double range[]={0,0,0};
  
  steps = floor(3/h);
  double* meshP = (double *)malloc(steps*sizeof(double));
  double* meshV = (double *)malloc(steps*sizeof(double));

  for (int m=0; m<2; m++){
    meshP[0]=m;
    meshV[0]=1-m;
    keys[0]=run_eu(meshP, meshV, 0, h, steps);
    for (int i=1; i<=keyN; i++ ){
      E = -1.0*i*resolv*deep;
      keys[i]=run_eu(meshP, meshV, E, h, steps);
      if ( (keys[i]*keys[i-1]) < 0 ){
	bisec( (-1*i*resolv-resolv)*deep, keys[i-1],  -1*i*resolv*deep, keys[i],meshP, meshV,  E, h, steps, range);
	N++;
	if (m==0)
	  cout<<setiosflags(ios::fixed)<<setprecision(6)<<"Energy state found:  "<<range[0]/deep<<" (odd) "<<"Range ["<<range[1]/deep<<" "<<range[2]/deep<<"]"<<endl;
	else
	  cout<<setiosflags(ios::fixed)<<setprecision(6)<<"Energy state found:  "<<range[0]/deep<<" (even) "<<"Range ["<<range[1]/deep<<" "<<range[2]/deep<<"]"<<endl;
      }
    }
  }
  free(meshP);
  free(meshV);
  return N;
};

double bisec(double low, double lowP,  double high, double highP, double* meshP, double* meshV, double E, double h, int steps, double (&range)[3]){
  if (high-low < 0.001){
    range[0] = 0.5*(low+high);
    range[1] = low;
    range[2] = high;
    return range[0];
  }
  double middle = 0.5*(low+high);
  double middleP = run_rk(meshP, meshV, middle, h, steps);

  if (middleP * lowP < 0)
    bisec(low, lowP,  middle, middleP,meshP, meshV,  E, h, steps, range);
  else
    bisec(middle, middleP, high, highP, meshP, meshV,  E, h, steps, range);
  return range[0];
};


