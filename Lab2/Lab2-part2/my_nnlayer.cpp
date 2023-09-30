#include <iostream>
#include <cmath>
using namespace std;

class Network_Layex {
public:
    double *a;   // Pointer to input values
    double **w;   // Pointer to weights
    double *b;    // Bias
    double *z;    //membrane potential

    size_t NI;   // Number of inputs
    size_t NO;   // Number of outpus
    size_t af;   // Activation function (1: ReLU, 2: tanh)
    double *a_o;  // Output after activation

    // Constructors
    Network_Layex () { // Default constructor
        NI = 10; cout << "Default constructor called. Setting NI=10 by default" << endl;
        NO = 10; cout << "Default constructor called. Setting NO=10 by default" << endl; }
    Network_Layex(size_t NI_i, size_t NO_o) : NI(NI_i), NO(NO_o) {
        cout << "Parameterized constructor called." << endl;
    }
    //Destructor
    ~Network_Layex() {
        free(z); free(a_o);
    }

    void operator() (double *a_i, double **w_i,double *b_i,size_t af_i)
    {
        a = a_i; w = w_i; b = b_i; af = af_i; 
        z = (double *) calloc (NO, sizeof(double));
        a_o= (double *) calloc (NO, sizeof(double));
        
        for(int i = 0; i < NO; i++)
        { z[i] = 0;
            for(int j = 0; j < NI; j++)
            {
                z[i] += a[j]*w[i][j];
            }
            z[i] += b[i];
        }
            

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
        };
        void result() 
        {
        int i;
        for (i = 0; i < NO; i++)
        {
        cout << "Membrane potential (z): " << z[i] << endl;
        }
        for (i = 0; i < NO; i++)
        {
        cout << "Action potential (a_o): " << a_o[i] << endl; 
        }
        };

        };

int main()
{   
    size_t NI = 5;
    size_t NO = 4;
    double *a,*b;
    double** w;
    int af = 1;

    Network_Layex NL(NI,NO);

    // a = new double[NI];
    // b = new double[NI];
    // w = new double*[NO];
    a = (double *) calloc (NI, sizeof(double));
    w = (double **) calloc (NO, sizeof(double*));
    for(int i = 0; i < NO; ++i)
    {
        w[i] = (double*)calloc(NI,sizeof(double));
    }
    b = (double *) calloc (NI, sizeof(double));
    // if(a == NULL) return 0;
    // if(w == NULL) return 0;


    // {{ 0.25,  0.50, -3.20, -4.50, -2.00},
    // { 2.00,  3.25,  5.75,  6.25,  7.15},
    // { 0.25, -3.50,  0.25,  0.25,  0.25},
    // { 2.00,  3.25,  0.75, -6.50,  1.50}};

    w[0][0] = 0.25; w[0][1] =0.5; w[0][2] = -3.20; w[0][3] = -4.50; w[0][4] = -2.00;
    w[1][0] = 2.00; w[1][1] =3.25; w[1][2] = 5.75; w[1][3] = 6.25;  w[1][4] = 7.15;
    w[2][0] = 0.25; w[2][1] =-3.50;w[2][2] = 0.25;w[2][3] = 0.25; w[2][4] = 0.25; 
    w[3][0] = 2.00; w[3][1] =3.25; w[3][2] = 0.75; w[3][3] = -6.50; w[3][4] = 1.50;


    a[0] = 2.5; a[1] = 3.0; a[2] = 2.5; a[3] = 1.5; a[4] = 1.0;
 
    b[0] = 2.0; b[1] = 1.5; b[2] = 2.5; b[3] = 3.5;
 

    NL(a,w,b,af);
    NL.result();


    // free(a);  free(b);
    // for(int i = 0; i < NO; i++)
    // {
    //     delete[] w[i];
    // }
    // delete[] w;
    // delete[] a;
    // delete[] b;
    for(int i = 0; i < NO; ++i)
    {
        free(w[i]);
    }
    free(w);
    free(a);
    free(b);
    NL.~Network_Layex();
    
    return 0;
}
//g++ my_program.cpp -o my_program