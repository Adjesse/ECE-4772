#include <iostream>
#include <math.h>
#include <cstdio>
#include <sys/time.h>
#include "tbb/tbb.h"
using namespace std;
using namespace tbb;

int nt;
class Lab6{
    int **input;
public:
    int *my_sum;
    void operator() (const blocked_range<int> &r) {
        int **I = input;
        int *sum = my_sum;
        for(int i=r.begin(); i != r.end(); ++i)
        {
           for (int j = 0; j < 256; ++j) sum[j] += I[i][j]; 
        }
    
    
    }
    Lab6 (int **I): input(I), my_sum(new int[256]()){}
    Lab6 (Lab6 &x, split): input(x.input), my_sum(new int[256]()){}
    void join (Lab6 &y) {
        for (int j = 0; j < 256; ++j) {
            my_sum[j] += y.my_sum[j];
            }
        }   
};
int* index(int  *I, int ji, int nt, int n){
    int* hp = new int[256];


    for(int i = ji*(n/nt);i < (ji+1)*(n/nt); i++)
    {
        hp[I[i]]++;
    }
    return hp;
    // Lab6 histo(hp);
    // parallel_reduce(blocked_range<int>(0,256),histo);
    // return histo.my_sum;
} 
int* sum(int **hpi, int nti)
{
    Lab6 red(hpi);
    parallel_reduce(blocked_range<int>(0,nti),red);
    return red.my_sum;
};


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

int main(int argc, char* argv[])
{
    if (argc != 2) {
        nt = 1;
    }
    else 
    {
        nt = atoi(argv[1]);
    }

    FILE *file_o;
    size_t result;
    char *in_file = "puppet.bif";

    char *out_file = "puppet_histogram.bof";

    struct timeval start, end;
    long t_us;


    int X = 1600; int Y = 1200;
    int *I , *h;
    int **hp;
    I = (int *) calloc(X*Y, sizeof(int));
    h = (int *) calloc(256, sizeof(int));
    hp = (int **) calloc ((nt), sizeof(int*));
    for(int i = 0; i < nt; i++)
    {
        hp[i] = (int*)calloc(256,sizeof(int));
    }
    read_binfile (I, X*Y, in_file, 0);

    gettimeofday (&start, NULL);
    parallel_for(blocked_range<int>(0,nt), [&] (blocked_range<int> r){
        for (int j = r.begin(); j!=r.end(); ++j){
            hp[j] = index(I,j,nt,X*Y); 
        }
    });

    h = sum(hp,nt);  
    gettimeofday (&end, NULL);


    //write results to the .bof file
    //*************************************
    file_o = fopen (out_file,"wb");
       if (file_o == NULL) return -1;// check that the file was actually opened
       
       result = fwrite (h, sizeof(int), 256, file_o); // each element (pixel) is of size int (4 bytes)
                   
       printf ("Output binary file: # of elements written = %d\n", result); // Total # of elements successfully read
       fclose (file_o);	


    printf ("start: %ld us\n", start.tv_usec); // start.tv_sec
    printf ("end: %ld us\n", end.tv_usec);    // end.tv_sec; 
    t_us = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec; // for ms: define t_ms as double and divide by 1000.0
     // gettimeofday: returns current time. So, when the secs increment, the us resets to 0.
    printf ("Elapsed time: %ld us\n", t_us);

    for(int j = 0; j<256; j++)
    {
        printf("h[%d] = %d",j,h[j]);
    }
    
    free(h); free(I); free(hp);
    // for(int i = 0; i < nt; ++i)
    // {
    //     free(hp[i]);
    // }
    return 0;
}