#include "tbb/tbb.h"
#include <stdio.h>
#include <iostream>
#include <cmath>

// it seems that 'const' must be used by every filter in the pipeline (according to online version of the Intel TBB, even though
//                        the intel tbb pdf does not specify that. This is an ERROR)

// g++ pip_mod.cpp -ltbb -o pip_mod
using namespace tbb;
using namespace std;

class MyMod {
public: 
  float av;
  float bv;
};

class my_in {
  float *a;
  float *b;
  int n;
  mutable int i; // learn more theory about 'mutable': pros and cons

// if I define MyMod*, it seems that I need to allocate memory for this object.  
public: 
  my_in (float *ap, float *bp, int np): a(ap), b(bp), n(np), i(0) {} // a = ap, b = bp, n = np
    
  MyMod operator () (flow_control& fc) const {
    
    MyMod t;
    //cout << i << "\t" << n << "\n";
    if (i < n) {
      t.av = *(a+i); t.bv = *(b+i);
      //cout << t.av << "\t" << t.bv << "\n";
      i++;
      return t; }
    else
      fc.stop();
    
  }
};

class my_transf {
public:
  float operator() (MyMod input) const { // 'input': when called by runpipeline, it includes its data
  // 'const' required. Otherwise, I get the following error:
  // "error: passing â€˜const my_transfâ€™ as â€˜thisâ€™ argument of â€˜float my_transf::operator()(MyMod*)â€™ discards qualifiers [-fpermissive]"

     float result;
     
     result = sqrt ( (input.av)*(input.av) + (input.bv)*(input.bv));
     return result;
  }  
};

class my_out {

  mutable int j; // so it can be modified by a 'const' operator()
  
public: 
  float *ci;
  my_out (float *cp): ci(cp), j(0) {} // initilize constructor. ci = cp
    
  void operator () (float result) const { // 'const' required
    
    *(ci+j) = result;
    j++;
        
  }
};

void RunPipeline (int ntoken, int n, float *a, float *b, float *c) {
  parallel_pipeline(
          ntoken,
	  make_filter<  void, MyMod>(filter::serial_in_order, my_in(a,b,n) )
      &   make_filter<MyMod,  float>(filter::parallel,        my_transf() )
      &   make_filter<float,    void>(filter::serial_in_order, my_out(c) ) );
}

int main () {
  
  int n = 10;
  int ntoken = 16;
  float a[10], b[10], c[10];
  int i;
  
//  a[0] = 0.6; a[1] = 0.3; a[2] = 0.6; a[3] = 0.7; a[4] = 0.9; a[5] = 0.46; a[6] = 0.64; a[7] = 1.6; a[8] = 1.5; a[9] = 1.8;
//  b[0] = 0.4; b[1] = 0.4; b[2] = 0.8; b[3] = 0.2; b[4] = 0.9; b[5] = 0.46; b[6] = 0.64; b[7] = 0.2; b[8] = 0.1; b[9] = 0.3;
  
  for (i=0; i < n; i++) {
    a[i] = sin(i * 3.1416/n);
    b[i] = tan(i * 3.1416/n);
   }
  
  
  RunPipeline (ntoken, n, a, b, c);
  
  cout << "Result:\n";
  for (i = 0; i < n; i++) {
    cout << c[i] << "\n";
  }
  
  return 0;
}
