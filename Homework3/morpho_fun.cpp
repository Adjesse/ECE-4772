#include "tbb/tbb.h"
#include <stdio.h>
#include <math.h>
#include <mutex>
#include <atomic>
#include <iostream>
#include "morpho_fun.h"

int read_binfile (int *data, int Length, char *in_file, int typ) {
  // data: where we place the data. Each data element is int32.
  // type: data type of each element:
  //    type = 0: each element is unsigned 8-bit integer. ==> 'unsigned char'
  //    type = 1: each element is a signed integer (32 bits) ==> 'int'
    
  // Length: # of elements to read (if type = 0 --> # of bytes, if type =1 --> number of 32-bit words)
  
  FILE *file_i;
  int i;
  size_t result, ELEM_SIZE;
  
  if (typ != 0 && typ != 1) { printf ("(read_binfile) Wrong modifier (only 0, 1 accepted)\n"); return -1; }
  
  file_i = fopen(in_file,"rb");
  if (file_i == NULL) { printf ("(read_binfile) Error opening file!\n"); return -1; } // check that the file was actually opened
      
  if (typ == 0) // each element (pixel) is an unsigned integer of 8 bits
  {
      unsigned char *IM;
      IM = (unsigned char *) calloc (Length, sizeof(unsigned char));

      ELEM_SIZE = sizeof(unsigned char);
      result = fread (IM, sizeof(unsigned char), Length, file_i);
            
      for (i = 0; i < Length; i++)  data[i] = (int) IM[i];
	  // This conversion transform elements of type 'unsigned char' to 'int'
	  // IMPORTANT: DO NOT use 'char' for input data. When transforming char to int, it will assume your data to be
	  //      signed, and it will screw up everything. Use unsigned char such that it transform from unsigned into signed integer.
      
      free (IM);
  }
  else //  if (typ == 1) // each element (pixel) is a signed integer (32 bits, 4 bytes)
  {
      int *IM;
      IM = (int *) calloc (Length, sizeof(int));

      ELEM_SIZE = sizeof(int);
      result = fread (IM, sizeof(int), Length, file_i);
          
      for (i = 0; i < Length; i++)  data[i] = IM[i];
      
      free (IM);
  }
    
  fclose (file_i);

  printf ("(read_binfile) Input binary file '%s': # of elements read = %ld\n", in_file, result); // Total # of elements successfully read
  printf ("(read_binfile) Size of each element: %ld bytes\n", ELEM_SIZE); // Size of each element
  
  return 0;
}   



int write_binfile (int *data_i, int Length, char *out_file, int typ) {
  // Writes binary file. 
  // Data is placed on 'data' pointer
  //  Only data type supported for each element: uint8
  
   FILE *file_o;
   size_t result, ELEM_SIZE;
   int i;
  
   file_o = fopen (out_file, "wb");
   if (file_o == NULL) {printf ("(write_binfile) Error opening file!\n"); return -1; }
    
   if (typ == 0) { // each element is uint8
       unsigned char *IM;
       IM = (unsigned char *) calloc (Length, sizeof(unsigned char));
       for (i = 0; i < Length; i++)  IM[i] = (unsigned char) data_i[i];
       
       result = fwrite (IM, sizeof(unsigned char), Length, file_o);
       ELEM_SIZE = sizeof(unsigned char);
       free(IM); }
       
   else {// each element is int 32
      result = fwrite (data_i, sizeof(int), Length, file_o);
      ELEM_SIZE = sizeof(int);
   }
   printf ("(write_binfile) Output binary file '%s': # of elements written = %ld\n", out_file, result);
   printf ("(read_binfile) Size of each element: %ld bytes\n", ELEM_SIZE); // Size of each element

   fclose (file_o);
  
  return 0; 

}

int minI (MORPHO_args morpho_data, int i, int j)
{
  int min, m, n;
  int kCX, kCY, kX, kY, sX, sY;
  int *I, *K;
  int i_I, j_I;
  
  I = morpho_data.I; sX = morpho_data.SX; sY = morpho_data.SY;
  K = morpho_data.K; kX = morpho_data.KX; kY = morpho_data.KY;
  
   kCX = kX / 2; kCY = kY / 2;    // find center position of kernel (half of kernel size, flooring)
 
   min = I[sX*i + j]; //   O[sX*i + j] = I[sX*i + j]
   for (m = 0; m < kY; m++) // SE: rows
	for (n = 0; n < kX; n++) // SE: columns
	{
		// out-of-bounds input samples are not considered
		if (K[kX*m + n] == 1)
		{
  		         i_I = i + m-kCY;
		         j_I = j + n-kCX;
		         if (i_I >= 0 && i_I < sY && j_I >= 0 && j_I < sX)
  			    if ( I[sX*i_I + j_I] <= min )  min = I [sX*i_I + j_I];
//			    if ( I[sX*i_I + j_I] <= O[sX*i + j] )  O[sX*i + j] = I [sX*i_I + j_I];				    
	        }
	}
  return min;
}

int maxI (MORPHO_args morpho_data, int i, int j)
{
  int max, m, n;
  int kCX, kCY, kX, kY, sX, sY;
  int *I, *K;
  int i_I, j_I;
  
  I = morpho_data.I; sX = morpho_data.SX; sY = morpho_data.SY;
  K = morpho_data.K; kX = morpho_data.KX; kY = morpho_data.KY;
  
   kCX = kX / 2; kCY = kY / 2;    // find center position of kernel (half of kernel size, flooring)
 
   max = 0; // O[sX*i + j] = 0;
   for (m = 0; m < kY; m++) // SE: rows
	for (n = 0; n < kX; n++) // SE: columns
		{
		// out-of-bounds input samples are not considered
		if (K[kX*m + n] == 1)
		{
  			         i_I = i + m-kCY;
			         j_I = j + n-kCX;
			         if (i_I >= 0 && i_I < sY && j_I >= 0 && j_I < sX)
  				    if ( I[sX*i_I + j_I] >= max )  max = I [sX*i_I + j_I];
//				    if ( I[sX*i_I + j_I] >= O[sX*i + j] )  O[sX*i + j] = I [sX*i_I + j_I];				    
			      }
		}
  return max;
}

int im_morph (MORPHO_args morpho_data, size_t t)
// t: morphological operation
//    t = 1 --> Grayscale Dilation
//    t = 2 --> Grayscale Erosion
// morpho_data.I: input image matrix represented as a 1D vector (raster-scan)
{
   // it computes convolution (the centered part)
   // It performs the same job as MATLAB conv2(I,K,'same')
   int sX, sY, kX, kY;
   int *I, *O, *K; // these are provided as linear arrays
   int i, j;
 
   I = morpho_data.I; O = morpho_data.O; sX = morpho_data.SX; sY = morpho_data.SY;
   K = morpho_data.K; kX = morpho_data.KX; kY = morpho_data.KY;

   // check validity of parameters
   if (!I || !O || !K) return 0;
   if (sX <= 0 || sY <= 0 || kX <= 0 || kY <= 0) return 0;   
   
   if (t == 1) // DILATION
   {
	for (i = 0; i < sY; i++) // Output matrix: rows
		for (j = 0; j < sX; j++) // Output matrix: columns
		{	    
			O[sX*i + j] = maxI(morpho_data,i,j);
      /*
                        // O[sX*i + j] = 0;
			max = 0; //using max seems to be slightly faster
			for (m = 0; m < kY; m++) // Kernel: rows
			{
				for (n = 0; n < kX; n++) // Kernel: columns
				{
				    // out-of-bounds input samples are not considered
				    if (K[kX*m + n] == 1)
				    {
				      i_I = i + m-kCY;
				      j_I = j + n-kCX;

				      if (i_I >= 0 && i_I < sY && j_I >= 0 && j_I < sX)
			//      	  if ( I[sX*i_I + j_I] >= O[sX*i + j] )  O[sX*i + j] = I [sX*i_I + j_I];
					  if ( I[sX*i_I + j_I] >= max )  max = I [sX*i_I + j_I];
				    }
				}
			}
			O[sX*i + j] = max;
			*/
		}
   }
   else // (t==2) // EROSION
   {
     	for (i = 0; i < sY; i++) // Output matrix: rows
		for (j = 0; j < sX; j++) // Output matrix: columns
		{	    
			O[sX*i + j] = minI(morpho_data,i,j);
		}
   } 
   return 1;
}


int im_morph_tbb (MORPHO_args morpho_data, size_t t)
// t: morphological operation
//    t = 1 --> Grayscale Dilation
//    t = 2 --> Grayscale Erosion
// morpho_data.I: input image matrix represented as a 1D vector (raster-scan)
// * This is a good example to show the pitfalls of parallel programming
//   If we do not use a function for the body of the loop, then each thread that
//     updates O[i,j] will interfere with each other (they might overwrite the local variables
//     at the wrong times). This is a race condition difficult to spot unless you run
//     the loop for large datasets (like an image)
{
   int sX, sY, kX, kY;
   int *I, *O, *K; // these are provided as linear arrays
   
   I = morpho_data.I; O = morpho_data.O; sX = morpho_data.SX; sY = morpho_data.SY;
   K = morpho_data.K; kX = morpho_data.KX; kY = morpho_data.KY;

   // check validity of parameters
   if (!I || !O || !K) return 0;
   if (sX <= 0 || sY <= 0 || kX <= 0 || kY <= 0) return 0;
   
      // OLD CODE (only for dilation)
      //  The following works (it is very slow), but I am not familiar with the syntax.
      /*
	  int i_I, j_I, m, n;
	  int kX, kY, kCX, kCY;
	  kCX = kX / 2; kCY = kY / 2; // Find center position of kernel (half of kernel size, flooring
	  // simple mutex lock/unlock didn't work (seems that when there are loops, a lot of mutexes are created and they interefere).
	  std::atomic<int> max;
	  std::atomic<int> i_I;
	  std::atomic<int> j_I;
      
	  using my_mutex_t = tbb::spin_mutex;
	  my_mutex_t my_mutex;   
      
	  tbb::parallel_for (int(0), int(sY), [&] (int i) { // 0 <= i <= SY-1
	    //   for (i = 0; i < sY; i++) // Output matrix: rows
	    my_mutex_t::scoped_lock my_lock(my_mutex);
	      for (j = 0; j < sX; j++) // Output matrix: columns
	      {
		      max = 0; // O[sX*i + j] = 0;		  
		      for (m = 0; m < kY; m++) // Kernel: rows
		      {
			      for (n = 0; n < kX; n++) // Kernel: columns
			      {
				  // out-of-bounds input samples are not considered
				  if (K[kX*m + n] == 1)
				  {
				    i_I = i + m-kCY;
				    j_I = j + n-kCX;
				    if (i_I >= 0 && i_I < sY && j_I >= 0 && j_I < sX)
    //				    if ( I[sX*i_I + j_I] >= O[sX*i + j] )  O[sX*i + j] = I [sX*i_I + j_I];
					if ( I[sX*i_I + j_I] >= max )  max = I [sX*i_I + j_I];
				  }
			      }
		      }		  
		      O[sX*i + j] = max;		  
		    
	      }
	    } );

      */
  
  // Tried different approaches (defining 'max' as atomic, mutexes). Ultimately, the problem
  //   is that variables inside the loop should be unique to (i,j). 
  //   A better solution is to encapsulate the processing in a function that depens on i,j.
  // this works better (with atomic max) ... but still see 2 dots (2 points where ther ei smistake)
  //   tried to use parallel_for for n and m.. did n't work.
  
     if (t == 1) // DILATION
     {
	// BEST APPROACH
	tbb::parallel_for (int(0), int(sY), [&] (int i) { // 0 <= i <= SY-1
	      tbb::parallel_for (int(0), int(sX), [&] (int j) { // 0 <= j <= SX-1
		      O[sX*i + j] = maxI(morpho_data,i,j);
		      } );
	    } );   
     }
     else // t==2: EROSION
     {
	tbb::parallel_for (int(0), int(sY), [&] (int i) { // 0 <= i <= SY-1
	      tbb::parallel_for (int(0), int(sX), [&] (int j) { // 0 <= j <= SX-1
		      O[sX*i + j] = minI(morpho_data,i,j);
		      } );
	    } );          
     }
     
    return 1;
}

