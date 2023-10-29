#include "tbb/tbb.h"
#include <stdio.h>
#include <iostream>

/* Place here:
 *  - Class definitions (functions, constructors inside class: only declare them)
 *  - Other function declarations 
 */

typedef struct {
   int SX;
   int SY;
   int KX;
   int KY;
   int *I;
   int *K;
   int *O;
} MORPHO_args;

int write_binfile (int *data_i, int Length, char *out_file, int typ);
int read_binfile (int *data, int Length, char *in_file, int typ);
int minI (MORPHO_args morpho_data, int i, int j);
int maxI (MORPHO_args morpho_data, int i, int j);
int im_morph (MORPHO_args morpho_data, size_t t);
int im_morph_tbb (MORPHO_args morpho_data, size_t t);