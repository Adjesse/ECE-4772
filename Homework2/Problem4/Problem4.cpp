#include <iostream>
#include <stdio.h> /* printf */
#include <stdlib.h>
#include <math.h> /* tanh, log */
using namespace std;
class Neuron {
private: // data members can ONLY be accessed by functions inside the class
 double *a; // neuron input vector. This is the output vector from the neurons in previous layer
 double *w; // vector of weights (associated with the elements of a)
 double b; // bias

public:
 size_t NI; // number of inputs
 size_t af; // activation function.. 1: ReLU, 2: tanh
 double z; // membrane potential of neuron
 double a_o; // action potential of neuron

 Neuron () { // Default constructor
 NI = 10; cout << "Default constructor called. Setting NI=10 by default" << endl; }

 Neuron (size_t NI_i) { // Parameterized constructor
 cout << "Parameterized constructor called." << endl;
 NI = NI_i; }
 // overload function call: functor
 void operator() (double *a_i, double *w_i, double b_i, int af_i) {
 int i;
 a = a_i; w = w_i; b = b_i; af = af_i;

 z = 0;
 for (i = 0; i < NI; i++) z = z + a[i]*w[i];
 z = z + b; // membrane potential

 if (af == 1) { if (z >= 0) a_o = z; else a_o = 0; }
 else if (af == 2) { a_o = tanh(z); }
 else a_o = z; // invalid activation function, no processing
 }

 void display_results () {
 cout << "Membrane potential (z): " << z << endl;
 cout << "Action potential (a_o): " << a_o << endl; }
};

int main() {
 size_t i;
 double b, result;
 double *a, *w;
 size_t NI = 10; // number of inputs.

 Neuron AN(NI); // AN.NI = NI -> Parameterized constructor.
 // Defining the inputs, as well as weights and bias of the Neuron.
 a = (double *) calloc (NI, sizeof(double));
 w = (double *) calloc (NI, sizeof(double));
 for (i = 0; i < NI; i++) { a[i] = 0.75; w[i] = -0.5; }
 b = 1.25;
 AN(a,w,b,1); // af = 1 (ReLU), af = 2 (tanh)

 AN.display_results(); // display 'z' and 'a_o'

 // we can also get AN.z and AN.ao individually:
 cout << "AN.z: " << AN.z << endl;
 cout << "AN.a_o: " << AN.a_o << endl;
 free (a); free(w);
 return 0;
}
