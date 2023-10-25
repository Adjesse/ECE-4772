#include <iostream>
#include <cmath>
#include <cstdio>
#include <sys/time.h>
#include "tbb/tbb.h"
using namespace std;
using namespace tbb;

class Network_Layer {
public:
    double *my_a;      // Pointer to input values
    double *my_W;      // Pointer to one row of weights
    double my_sum;
    //double *b;    // Bias
    //size_t af;   // Activation function (1: ReLU, 2: tanh)
    
    //double *z;    //membrane potential

    //size_t NI;   // Number of inputs
    //size_t NO;   // Number of outpus
    //double *a_o;  // Output after activation

    void operator()(const blocked_range<size_t> &r)
    {
        double *w = my_W;
        double *a = my_a;
        double sum = my_sum;


        for(size_t i = r.begin(); i != r.end(); ++i)
        {
            sum += w[i]*a[i];
        }
        my_sum = sum;

    }
    // Constructors
    Network_Layer (Network_Layer &x,split): my_a(x.my_a), my_W(x.my_W), my_sum(0) {}
    void join (const Network_Layer &y) { my_sum += y.my_sum;}
    Network_Layer (double *a ,double* W ): my_a(a), my_W(W), my_sum(0) {} // my_max = my_a[0]    

};

double Dotpro(double* W, double* a, size_t L)
{
    Network_Layer NL(a,W);
    tbb::parallel_reduce(blocked_range<size_t> (0,L), NL);
    return NL.my_sum;
}

int main()
{
    size_t NI = 5;
    size_t NO = 4;
    int af = 1;
    double *a,*b,*z,*a_o;
    double **w;
    struct timeval start, end;
    long t_us;
    a = (double *) calloc (NI, sizeof(double));
    w = (double **) calloc (NO, sizeof(double*));
    for(int i = 0; i < NO; ++i)
    {
        w[i] = (double*)calloc(NI,sizeof(double));
    }
    b = (double *) calloc (NI, sizeof(double));
    z = (double *) calloc (NO, sizeof(double));
    a_o = (double *) calloc (NO, sizeof(double));
    
    w[0][0] = 0.25; w[0][1] =0.5; w[0][2] = -3.20; w[0][3] = -4.50; w[0][4] = -2.00;
    w[1][0] = 2.00; w[1][1] =3.25; w[1][2] = 5.75; w[1][3] = 6.25;  w[1][4] = 7.15;
    w[2][0] = 0.25; w[2][1] =-3.50;w[2][2] = 0.25;w[2][3] = 0.25; w[2][4] = 0.25; 
    w[3][0] = 2.00; w[3][1] =3.25; w[3][2] = 0.75; w[3][3] = -6.50; w[3][4] = 1.50;


    a[0] = 2.5; a[1] = 3.0; a[2] = 2.5; a[3] = 1.5; a[4] = 1.0;
 
    b[0] = 2.0; b[1] = 1.5; b[2] = 2.5; b[3] = 3.5;

    gettimeofday (&start, NULL);

    tbb::parallel_for(int(0),int(NO),[&] (int i)
    {
       z[i] = Dotpro(w[i],a,NI);
       z[i] += b[i];
        //apply non-linear operation
        if (af == 1)
        { 
            for(int i = 0; i < NO; i++)
            {
                if(z[i]>=0) {a_o[i]=z[i];}
                else {a_o[i] =0;}
            }
        }
        else if(af == 2)
            for(int i = 0; i < NO; i++)
            {
                a_o[i] = tanh(z[i]);
            }
        else 
            {cout << "error";}
    });

    gettimeofday (&end, NULL);

    int i;
    for (i = 0; i < NO; i++)
    {
    cout << "Membrane potential (z): " << z[i] << endl;
    }
    for (i = 0; i < NO; i++)
    {
    cout << "Action potential (a_o): " << a_o[i] << endl; 
    }

    printf ("start: %ld us\n", start.tv_usec); // start.tv_sec
    printf ("end: %ld us\n", end.tv_usec);    // end.tv_sec; 
    t_us = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec; // for ms: define t_ms as double and divide by 1000.0
    // gettimeofday: returns current time. So, when the secs increment, the us resets to 0.
    printf ("Elapsed time (only convolution computation): %ld us\n", t_us);

    for(int i = 0; i < NO; ++i)
    {
        free(w[i]);
    }
    free(w); free(a); free(b); free(z);

    return 0;
}