#include "tbb/tbb.h"
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <cmath>
#include <sys/time.h>

// it seems that 'const' must be used by every filter in the pipeline (according to online version of the Intel TBB, even though
//                        the intel tbb pdf does not specify that. This is an ERROR)

// g++ lab7.cpp -ltbb -o lab7 -g
// ./lab7
using namespace tbb;
using namespace std;

class MyPair {
public:
  double *x;
  double *r;
  int n;
};

class my_in {
  double **x;
  double **r;
  int n;
  int NV;
  mutable int i;

public: 
  my_in (double **xp,  double **rp, int np, int NVp): x(xp), r(rp), n(np), NV(NVp), i(0) {} // a=ap,  r=rp, n = np, NV = NVp

  MyPair operator () (flow_control& fc) const {
    MyPair t;

    if (i < NV) {
      t.x = *(x+i); t.r = *(r+i); t.n = n;
      i++;
      return t; }
    else
      fc.stop();
    
  }
};

class my_transf {
public:
  //my_transf (double *tv): result(tv) {} // result = tv
  
  double* operator() (MyPair input) const { // 'input': when called by runpipeline, it includes its data
 
     size_t i;
     double *result = input.r;
     for (i = 0; i < input.n; i++)
       result[i] =  1.0 / ((1.0 / std::exp(input.x[i])) + 1.0);
     
     return result;
  }  
};

class my_out {
public: 
  mutable int j; // so it can be modified by a 'const' operator()
  double *oi;
  int n;
  my_out (double *op, int np): oi(op), n(np), j(0) {} 
    
  void operator () (double *rt) const { // 'const' required
    size_t k;
    double tmp = rt[0] ;
    
    for (k = 0; k < n; k++)
    {
        if (rt[k] < tmp) tmp = rt[k];
    }
      
      
    *(oi+j) = tmp;
    j++;
        
  }
};



void RunPipeline (int ntoken, int n, int NV, double **x, double **r, double *o) {
  parallel_pipeline(
          ntoken,
	  make_filter<  void,  MyPair> (filter::serial_in_order, my_in(x,r,n,NV) )
      &   make_filter<MyPair,  double*>(filter::parallel,        my_transf() ) 
      &   make_filter<double*,    void>(filter::serial_in_order, my_out(o,n) ) );
}

int main (int argc, char **argv) {

    int n; // Length of each vector
    int NV; // # of vectors
    int ntoken = 16; //upper bound
    struct timeval start, end;
    long t_us;

    if (argc != 3) 
    {
    printf ("Warning: Usage: %s t\n", argv[0]);
    printf ("Using n = 5, NV = 20 as default \n");
    n = 5;
    NV = 20;
    }
    else 
    {
    n = atoi(argv[1]); 
    NV = atoi(argv[2]);
    }

    int i; int j;
    double **x,**r;
    double *o;
    x = (double **) calloc (NV, sizeof(double *));
    r = (double **) calloc (NV, sizeof(double *));
    o = (double *) calloc (NV, sizeof(double)); 

    for (i=0; i < NV; i++) 
    { 
        x[i] = (double *) calloc (n, sizeof(double));
        r[i] = (double *) calloc (n, sizeof(double));
    }

    for(i = 0; i < NV; i++)
    {
    for (j = 0; j < n; j++)
    {
        x[i][j] = (i*0.4) - (j*0.5);
    }
    }

    gettimeofday (&start, NULL);

    RunPipeline (ntoken, n, NV, x, r, o);

    gettimeofday (&end, NULL);
    cout << "Result:\n";
    for (i = 0; i < 20; i++) {
    cout << o[i] << "\n";
    }
    printf ("start: %ld us\n", start.tv_usec); // start.tv_sec
    printf ("end: %ld us\n", end.tv_usec);    // end.tv_sec; 
    t_us = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec; // for ms: define t_ms as double and divide by 1000.0
     // gettimeofday: returns current time. So, when the secs increment, the us resets to 0.
    printf ("Elapsed time: %ld us\n", t_us);
    free(o);
    for (i = 0; i < NV; i++) { free (x[i]); free(r[i]); }
    free(x); free(r); 

    return 0;
    }