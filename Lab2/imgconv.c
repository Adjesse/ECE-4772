#include <stdio.h>
#include <stdlib.h>
#include "imgconv_fun.h"
#include <sys/time.h>
#include <string.h>
#include<math.h>

// Tutorial # 2
// Input file: binary .bif
// Specify the raster scan procedure (from image to binary file: linear file)

// K = [0 -1 0; -1 4 -1; 0 -1 0] --> Edge Detection kernel
// Result: with I and 3x3 K: it works (same as MATLAB conv2(I,K,'same')
//  This code computes the central part of the convolution, the output is the same size as I.

int main (int argc, char **argv) {
    int i, status,t;
    size_t result;
    FILE *file_k, *file_o;
    CONV_args convo; 

    char out_file[10];
    char in_file[10] = "iss.bif"; // binary input file.

    if (argc !=2){
	printf("Warning: Usage: %s n\n",argv[0]);
	printf("Using t = 1 as default\n");
	t = 1;
    }
    else t = atoi(argv[1]);
    printf("t = %d\n",t);

    // char kn_file[20] = "edge_kernel.txt"; // text file
    if (t==1)
    {
        strcpy(out_file, "iss_a.bof"); 
        convo.KX = 3; convo.KY = 3; // convolution kernel size
    }
    else
    {
        strcpy(out_file, "iss_b.bof");
        convo.KX = 5; convo.KY = 5; // convolution kernel size
    } // binary output file.
    char ln_text[35];

   
    ln_text[32] = '\0';

    struct timeval start, end;
    long t_us;
    
    convo.SX = 640; convo.SY = 480; // X direction: column, Y direction: row.
    
 
    // Dynamic Memory allocation
    convo.I = (int *) calloc(convo.SX*convo.SY, sizeof(int)); // raster scan      
    convo.K = (double *) calloc(convo.KX*convo.KY, sizeof(double)); 
    convo.O = (double *) calloc(convo.SX*convo.SY, sizeof(double)); 

    // INPUT IMAGE: Reading binary file that contains the binary image: raster scan fashion
    // ************************************************************************************
   status = read_binfile (convo, in_file, 0); // '0': each element is unsigned 8-bit integer, '1': each element is signed int.
   if (status == -1) return 0;
	
    // KERNEL: Opening textfile that containts the kernel. Each line has a 32-bit signed element
    // **********************************************************************************
	// file_k = fopen (kn_file, "r");
	
	// if (file_k == NULL) return -1; /* check that the file was actually opened */

	// for (i = 0; i < convo.KX*convo.KY; i++)
	//    if (fgets (ln_text, sizeof (ln_text), file_k) != NULL) convo.K[i] = binstr2int(ln_text);
 
	// fclose(file_k);
    if (t == 1){
        double temp[9] = {-3.0, -3.0,  6.0, -3.0,  6.0, -3.0, 4.0, -5.0,  1.0};
            for(int i = 0; i < 9; i++)
            {
                convo.K[i] = temp[i];
            }
        
    }
    else{
        double temp[25] = {1.0,4.0,6.0,4.0,1.0, 4.0,16.0,24.0,16.0,4.0, 6.0,24.0,36.0,24.0,6.0, 4.0,16.0,24.0,16.0,4.0, 1.0,4.0,6.0,4.0,1.0};
        for (int i = 0; i < 25; i++) {
            temp[i] = (1.0/256.0) * temp[i];
            convo.K[i] = temp[i];
        }

    }

	// Print kernel
    for (i = 0; i < convo.KX*convo.KY; i++) printf ("K[%d] = %f\n",i,convo.K[i]);
        
    
     // Performing 2D convolution
     // **************************
	gettimeofday (&start, NULL);
	conv2D (convo);
    gettimeofday (&end, NULL);

    int intConvo[convo.SX*convo.SY];
    int xr;
    for(int i = 0; i < convo.SX*convo.SY; i++) 
    {
        if(convo.O[i] >= 0) xr = convo.O[i] + 0.5; else xr = convo.O[i] - 0.5;
        intConvo.O[i] = (int) xr;

    }
    


    // OUTPUT IMAGE: writing result on output binary file
    // ***************************************************
    file_o = fopen (out_file,"wb");
    if (file_o == NULL) return -1;// check that the file was actually opened
    result = fwrite (intConvo, sizeof(int), convo.SX*convo.SY, file_o); // each element (pixel) is of size int (4 bytes)
    printf ("Output binary file: # of elements written = %ld\n", result); // Total # of elements successfully read
    fclose (file_o);	

       
    printf ("start: %ld us\n", start.tv_usec); // start.tv_sec
    printf ("end: %ld us\n", end.tv_usec);    // end.tv_sec; 
    t_us = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec; // for ms: define t_ms as double and divide by 1000.0
    // gettimeofday: returns current time. So, when the secs increment, the us resets to 0.
    printf ("Elapsed time (only convolution computation): %ld us\n", t_us);
    
    
    free(convo.I); free (convo.K); free(convo.O);
    return 0;
}