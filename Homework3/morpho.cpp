#include "tbb/tbb.h"
#include <stdio.h>
#include <iostream>
#include <sys/time.h>
#include "morpho_fun.h"

using namespace tbb;
using namespace std;
// Computation time (inspiron laptop):
// Dilation:  Sequential: 46003 us    TBB: 31733 us
// Erosion:   Sequential: 46248 us    TBB: 29275 us
int main (int argc, char **argv) {
   size_t t;
   int i, status;
   MORPHO_args morpho;   
   char fname_i[10] = "uchip.bif";
   char *fname_o;   
   struct timeval start, end, start_p, end_p;
   long t_us, t_us_p;
   
   morpho.SY = 602; morpho.SX = 940;
   morpho.KY = 5; morpho.KX = 5;
   
   // Reading Input parameters
   if (argc != 2) {
      cout << "Warning: Usage: "<< argv[0] << " <modifier (1,2)>\n";
      cout << "Using modifier = 1 (dilation) as default\n"; t = 1; }
   else t = atoi(argv[1]); // argv[0]: ./vector_op, argv[1]: n

   if (t != 1 && t != 2) { cout << "Incorrect modifier (1,2)\n"; exit(-1); }
   if (t == 1) fname_o = "uchip_d.bof";  else  fname_o = "uchip_e.bof";

   morpho.I = (int *) calloc (morpho.SY*morpho.SX,sizeof (int));
   morpho.K = (int *) calloc (morpho.KY*morpho.KX,sizeof (int));
   morpho.O = (int *) calloc (morpho.SY*morpho.SX,sizeof (int));   
   
   // Structural element (SE): disk with radius=2
   morpho.K[0]  = 0; morpho.K[1]  = 0; morpho.K[2]  = 1; morpho.K[3]  = 0; morpho.K[4]  = 0;
   morpho.K[5]  = 0; morpho.K[6]  = 1; morpho.K[7]  = 1; morpho.K[8]  = 1; morpho.K[9]  = 0;
   morpho.K[10] = 1; morpho.K[11] = 1; morpho.K[12] = 1; morpho.K[13] = 1; morpho.K[14] = 1;
   morpho.K[15] = 0; morpho.K[16] = 1; morpho.K[17] = 1; morpho.K[18] = 1; morpho.K[19] = 0;
   morpho.K[20] = 0; morpho.K[21] = 0; morpho.K[22] = 1; morpho.K[23] = 0; morpho.K[24] = 0;
        
   // Read input data
   status = read_binfile(morpho.I, morpho.SX*morpho.SY, fname_i, 0); if (status == -1) return 0; // 0: uint8
   // The 2D image is stored in a raster scan fashion. morpho.I: one-dimensional vector.

   // Computation (sequential): Perform image dilation
   // ************************
      gettimeofday(&start,NULL);
      status = im_morph (morpho, t);
      gettimeofday(&end,NULL);

      for (i = 0; i < morpho.SX*morpho.SY; i++) morpho.O[i] = 0;
      
   // Computation (TBB): Perform image dilation
   // ***************************
      gettimeofday(&start_p,NULL);
      status = im_morph_tbb (morpho, t);
      gettimeofday(&end_p,NULL);
      
   // Saving result in a .bof file
   // ******************************
    status = write_binfile (morpho.O, morpho.SX*morpho.SY, fname_o, 0); if (status == -1) return 0; // '0' data will be stored as uint8
   
   // Printing time measurements (sequential approach):
   // --------------------------
    printf ("Sequential Approach:\n");
    printf ("\tstart: %ld us\t", start.tv_usec); // start.tv_sec
    printf ("end: %ld us\n", end.tv_usec);    // end.tv_sec; 
    t_us = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec; // for ms: define t_ms as double and divide by 1000.0
    // gettimeofday: returns current time. So, when the secs increment, the us resets to 0.
    printf ("\tElapsed time (actual computation): %ld us\n", t_us);  
   
   // Printing time measurements (parallel approach:TBB):
   // --------------------------
    printf ("TBB Approach:\n");
    printf ("\tstart: %ld us\t", start_p.tv_usec); // start.tv_sec
    printf ("end: %ld us\n", end_p.tv_usec);    // end.tv_sec; 
    t_us_p = (end_p.tv_sec - start_p.tv_sec)*1000000 + end_p.tv_usec - start_p.tv_usec; // for ms: define t_ms as double and divide by 1000.0
    printf ("\tElapsed time (actual computation): %ld us\n", t_us_p);  
  
   free(morpho.I); free(morpho.K); free(morpho.O);
   return 0;
}