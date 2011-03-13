#include<cstdlib>
#include<iostream>
#include<cmath>
using namespace std;

//int run(int, particle, double, double *);
double run_rk(double*, double*, double, double, int);
double run_eu(double*, double*, double, double, int);
double potential(double x, double deep);
int search( double h, int steps, double resolv);
double bisec(double low, double lowP, double high, double highP,  double* meshP, double* meshV, double E, double h, int steps, double (&range)[3]);
