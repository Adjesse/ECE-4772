#include <iostream>
#include <math.h>
#include <cstdio>
#include <sys/time.h>
using namespace std;
int read_binfile (int *data, int Length, char *in_file, int typ);

typedef struct {
    int SX;
    int SY;
    int KX;
    int KY;
    int *I;
    double *O;
    int *output;
} CONV_args;

//global structure
CONV_args convo;


int main()
{
    char *file = "uchip.bif";
    int length = 565880;
    FILE *file_o;
    char *file_o_name = "uchip.bof";
    

    struct timeval start, end;
    long t_us;
    
    convo.SX = 940; convo.SY = 602; // X direction: column, Y direction: row.

    // Dynamic Memory allocation
    convo.I = (int *) calloc(convo.SX*convo.SY, sizeof(int)); // raster scan       
    convo.O = (double *) calloc(convo.SX*convo.SY, sizeof(double)); 
    convo.output = (int *) calloc(convo.SX*convo.SY, sizeof(int));

    read_binfile(convo.I,length,file,0);


    gettimeofday (&start, NULL);

    for(int i = 0; i < convo.SX*convo.SY;i++)
    {
        convo.O[i] = (pow((double)(convo.I[i]/256.0),0.6))*256.0;
        
    }

    for(int i = 0; i < convo.SX*convo.SY;i++)
    {

        if (convo.O[i] >= 0) convo.O[i] = convo.O[i] + 0.5; else convo.O[i] = convo.O[i] - 0.5;
        convo.output[i]  = (int) convo.O[i];

    }
    gettimeofday (&end, NULL);

    // OUTPUT IMAGE: writing result on output binary file
    // ***************************************************
    size_t result;
    file_o = fopen (file_o_name,"wb");
    if (file_o == NULL) return -1;// check that the file was actually opened
    result = fwrite (convo.output, sizeof(int), convo.SX*convo.SY, file_o); // each element (pixel) is of size int (4 bytes)
    printf ("Output binary file: # of elements written = %ld\n", result); // Total # of elements successfully read
    fclose (file_o);


    printf ("start: %ld us\n", start.tv_usec); // start.tv_sec
    printf ("end: %ld us\n", end.tv_usec);    // end.tv_sec; 
    t_us = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec; // for ms: define t_ms as double and divide by 1000.0
    // gettimeofday: returns current time. So, when the secs increment, the us resets to 0.
    printf ("Elapsed time (only convolution computation): %ld us\n", t_us);
    
    free(convo.I); free(convo.O); free(convo.output);
    return 0;
}


int read_binfile (int *data, int Length, char *in_file, int typ) {
// data: array where the data read from file is placed
// type: type = 0: each element is unsigned 8-bit integer. ==> 'unsigned char'
// type = 1: each element is a signed integer (32 bits) ==> 'int'
// Length: # of elements to read (if type =1 --> number of 32-bit words)
    FILE *file_i;
    int i;
    size_t result, ELEM_SIZE;

    if (typ != 0 && typ != 1) { printf ("Wrong modifier (only 0, 1 accepted)\n"); return -1; }

    file_i = fopen(in_file,"rb");
    if (file_i == NULL) { printf ("Error opening file!\n"); return -1; }

    if (typ == 0) { // each element is an unsigned integer of 8 bits
        unsigned char *IM;
        IM = (unsigned char *) calloc (Length, sizeof(unsigned char));
        ELEM_SIZE = sizeof(unsigned char);
        result = fread (IM, sizeof(unsigned char), Length, file_i);

        for (i = 0; i < Length; i++) data[i] = (int) IM[i];
        free (IM); }
    else { // if (typ == 1) // each element is a signed 32-bit integer
        int *IM;
        IM = (int *) calloc (Length, sizeof(int));
        ELEM_SIZE = sizeof(int);
        result = fread (IM, sizeof(int), Length, file_i);

        for (i = 0; i < Length; i++) data[i] = IM[i];
        free (IM); }

    fclose (file_i);
    printf ("(read_binfile) Input binary file '%s': # of elements read = %ld\n", in_file, result);
    printf ("(read_binfile) Size of each element: %ld bytes\n", ELEM_SIZE);
    return 0;
} 