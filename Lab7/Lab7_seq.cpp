#include "tbb/tbb.h"
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <cmath>
#include <sys/time.h>

using namespace tbb;
using namespace std;

int main (int argc, char **argv) {

    int n; // Length of each vector
    int NV; // # of vectors
    int ntoken = 16; //upper bound
    struct timeval start, end;
    long t_us;
    double tmp;

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


    for(int i = 0; i < NV; i++)
    {
        for(int j = 0; j < n; j++)
        {
            r[i][j] = 1.0 / ((1.0 / std::exp(x[i][j])) + 1.0);
            if (i == 0) tmp = r[i][j];
            if (r[i][j] < tmp) tmp = r[i][j];
        }
        o[i] = tmp;
    }
    
    gettimeofday (&end, NULL);
    cout << "Result:\n";
    for (i = 0; i < NV; i++) {
    cout << o[i] << "\n";
    }
    printf ("start: %ld us\n", start.tv_usec); // start.tv_sec
    printf ("end: %ld us\n", end.tv_usec);    // end.tv_sec; 
    t_us = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec; // for ms: define t_ms as double and divide by 1000.0
     // gettimeofday: returns current time. So, when the secs increment, the us resets to 0.
    printf ("Elapsed time: %ld us\n", t_us);
    free(o);
    for (i = 0; i < n; i++) { free (x[i]); free(r[i]); }
    free(x); free(r); 

    return 0;
    }