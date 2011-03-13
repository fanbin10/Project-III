#include <fstream>
#include "shoot.h"


int main(int argc, char *argv[])
{
  double energy = -0.08;
  double bound = 5;
  int c;
  int steps;
  int flag = 0;
  double h = 0.0001;
  double bic;
  double m = 0;
  double deep = 140;
  double (*algorithm)(double*, double*, double, double, int);
  algorithm = &run_eu;
  while ((c = getopt(argc, argv, "n:h:e:b:por")) != -1)
    switch(c)
      {
      case 'h':
	h = atof(optarg);
	break;
      case 'e':
	energy = atof(optarg);
	break;
      case 'p':
	flag =1;
	break;
      case 'b':
	bound = atof(optarg);
	break;
      case 'o':
	m = 1;
	break;
      case 'r':
	algorithm = &run_rk;
      }
  
  steps= bound/h;
  energy *= deep;

 
  // construct grid for positions
  double* meshP = (double *)malloc(steps*sizeof(double));
  meshP[0] = 1-m;
  // construct grid for velocities
  double* meshV = (double *)malloc(steps*sizeof(double));
  meshV[0] = m;

  
  if (flag == 1){
    bic=(*algorithm)(meshP, meshV,  energy, h, steps);
    cout<<bic<<endl;

      ofstream fp;
      fp.open("plot.dat");
      for (int j=0; j<steps; j++){
	fp<<h*j<<"   "<<meshP[j]<<endl;
      }
      fp.close();
      FILE *pipe = popen("gnuplot -persist","w");
      fprintf(pipe,"set title \"energy: %f \" \n", energy/deep);
      fflush(pipe);
      fprintf(pipe,"set xrange [0:%d]\n", (int)floor(h*steps));
      fflush(pipe);
      fprintf(pipe,"set yrange [-4:4]\n");
      fflush(pipe);
      fprintf(pipe,"plot 'plot.dat' with lines\n");
      fflush(pipe);
      //sleep(2);
    }						
     else
       search( h, steps, 0.0001);
  return 0;
}
