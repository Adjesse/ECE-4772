#include <iostream>
#include <cmath>
using namespace std;

class Network_Layex {
public:
    double *a;   // Pointer to input values
    double *w;   // Pointer to weights
    double *b;    // Bias
    double *z;    //membrane potential

    size_t NI;   // Number of inputs
    size_t NO;   // Number of outpus
    size_t af;   // Activation function (1: ReLU, 2: tanh)
    double z;    // Weighted sum
    double a_o;  // Output after activation

    // Constructors
    Network_Layex () { // Default constructor
        NI = 10; cout << "Default constructor called. Setting NI=10 by default" << endl;
        NO = 10; cout << "Default constructor called. Setting NO=10 by default" << endl; }
    Network_Layex(size_t NI_i, size_t NO_o) : NI(NI_i), NO(NO_o) {
        cout << "Parameterized constructor called." << endl;
    }
    // Destructor
    // ~Neuron() {
    //     delete[] a; // Free allocated memory for input values
    //     delete[] w; // Free allocated memory for weights
    // }

    void operator() (double *a_i, double *w_i,double *b_i,size_t af_i)
    {
        a = a_i; w = w_i; b = b_i; af = af_i;
        z = 0; 
        for(int i = 0; i < NI; i++)
        {
            z = z + a[i]*w[i];
            z = z+b[i];

            //apply non-linear operation
            if (af == 1)
            { if(z>=0) a_o=z; else a_o =0;}
            else if(af == 2)
                {a_o = tanh(z);}
            else 
                {cout << "error"; a_o = z;}
            }

        }


    };

int main()
{   
    size_t NI = 5;
    size_t NO = 4;
    double *a,*w,*b;
    int af = 1;

    Network_Layex NL(NI,NO);

    a = (double *) calloc (NI, sizeof(double));
    w = (double *) calloc (NI*NO, sizeof(double));
    b = (double *) calloc (NI, sizeof(double));
    if(a == NULL) return 0;
    if(w == NULL) return 0;


    w[4][5] =       {{ 0.25,  0.50, -3.20, -4.50, -2.00},
                     { 2.00,  3.25,  5.75,  6.25,  7.15},
                     { 0.25, -3.50,  0.25,  0.25,  0.25},
                     { 2.00,  3.25,  0.75, -6.50,  1.50}};

    a[0] = 2.5; a[1] = 3.0; a[2] = 2.5; a[3] = 1.5; a[4] = 1.0;
 
    b[0] = 2.0; b[1] = 1.5; b[2] = 2.5; b[3] = 3.5;
 

    NL(a,w,b,af);


    free(a); free(w); free(b);

}