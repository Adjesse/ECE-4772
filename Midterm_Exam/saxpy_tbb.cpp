#include <iostream>
#include <math.h>
#include <cstdio>
#include <sys/time.h>
#include "tbb/tbb.h"
using namespace std;
using namespace tbb;

int vector_len;
float *x,*y,*y_s,*y_p;
float a = 1.618;
float sum;


void saxpy_seq()
{
    for(int i = 0; i < vector_len; i++)
    {
        y_s[i] = a*x[i] + y[i];
    }
}

int main(int argc, char *argv[])
{
    struct timeval start, end;
    struct timeval start2,end2;
    long t_us;
    void *status;
    if (argc != 2) {
        printf("Warning: Usage: %s n\n", argv[0]);
        printf("Using vector_len = 1 as default\n");
        vector_len = 1;
    }
    else 
    {
        vector_len = atoi(argv[1]);
    }

    x = (float *) calloc (vector_len, sizeof(float));
    y = (float *) calloc (vector_len, sizeof(float));
    y_s = (float *) calloc (vector_len, sizeof(float));
    y_p = (float *) calloc (vector_len, sizeof(float));


    for(int i = 0; i < vector_len; i++)
    {
        x[i] = sinh((i*3.416)/(vector_len * 1.00));
        y[i] = cosh((i*3.416)/(vector_len * 1.00));
    }

    gettimeofday (&start, NULL);
    saxpy_seq();
    gettimeofday (&end, NULL);
    
    gettimeofday (&start2, NULL);
    tbb::parallel_for (int(0), int(vector_len), [&] (int i) { 
        y_p[i] = a*x[i] + y[i];
    });
    gettimeofday (&end2, NULL);

    printf("x(input)\t\ty(input)\t\t\ty(output)\n");
    for(int i = 0; i < 20;i++)
    {
        printf("x[%d]=%f\t\ty[%d]=%f\t\t\ty[%d]=%f\n",i,x[i],i,y[i],i,y_p[i]);
    }

    for(int i = 0; i < vector_len; i++)
    {
        sum += fabs(y_p[i] - y_s[i]);
    }


    printf("Sum of absolute differences: %f\n",sum);
    printf("vector_len = %d\n\n",vector_len);

    printf("Time Measurments\n****************\n");
    printf("pthreads implementation - nthreads = %d\n");
    printf ("start: %ld us\n", start2.tv_usec); // start.tv_sec
    printf ("end: %ld us\n", end2.tv_usec);    // end.tv_sec; 
    t_us = (end2.tv_sec - start2.tv_sec)*1000000 + end2.tv_usec - start2.tv_usec; // for ms: define t_ms as double and divide by 1000.0
    printf ("Elapsed time: %ld us\n\n", t_us);

    printf("Sequential implementation\n");
    printf ("start: %ld us\n", start.tv_usec); // start.tv_sec
    printf ("end: %ld us\n", end.tv_usec);    // end.tv_sec; 
    t_us = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec; // for ms: define t_ms as double and divide by 1000.0
    // gettimeofday: returns current time. So, when the secs increment, the us resets to 0.
    printf ("Elapsed time: %ld us\n", t_us);
    free(x); free(y); free(y_s);
    return 0;
}