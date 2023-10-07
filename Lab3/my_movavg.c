#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct{
    double *f;         //Output vector f
    int *in_data;      //Input data
} DATA;


#define num_elements 100000
char *in_file = "mydata.bif";
int NUM_THREADS;
//pthread_mutex_t mutex_movavg;
DATA data;




void *movavg (void *arg)
{
    int *in_data;
    int i, start, end;
    int Thread = (int)arg;

    start = (Thread*num_elements)/NUM_THREADS;
    end = (((Thread+1)*num_elements)/NUM_THREADS) - 1;


    printf("Thread %d computes slice %d (indices: %d to %d)\n",Thread, Thread, start , end);

    for(i=start; i <= end; i++)
    {
        if(i == 0)
        {
            data.f[i] = (data.in_data[i] + data.in_data[i + 1] + data.in_data[i + 2] + data.in_data[i + 3]) / 4.0;
        }
        else if(i == 1)
        {
            data.f[i] = (data.in_data[i-1] + data.in_data[i] + data.in_data[i + 1] + data.in_data[i + 2] + data.in_data[i + 3]) / 5.0;
        }
        else if(i == 2)
        {
            data.f[i] = (data.in_data[i-2] + data.in_data[i-1] + data.in_data[i] + data.in_data[i + 1] + data.in_data[i + 2] + data.in_data[i + 3]) / 6.0;
        }
        else if(i == num_elements - 1)
        {
            data.f[i] = (data.in_data[i-3] + data.in_data[i-2] + data.in_data[i-1] + data.in_data[i]) / 4.0;
        }
        else if(i == num_elements - 2)
        {
            data.f[i] = (data.in_data[i-3] + data.in_data[i-2] + data.in_data[i-1] + data.in_data[i] + data.in_data[i+1]) / 5.0;
        }
        else if(i == num_elements - 3)
        {
            data.f[i] = (data.in_data[i-3] + data.in_data[i-2] + data.in_data[i-1] + data.in_data[i] + data.in_data[i+1] + data.in_data[i+2]) / 6.0;
        }
        else
        {
            data.f[i] = (data.in_data[i-3] + data.in_data[i-2] + data.in_data[i-1] + data.in_data[i] + data.in_data[i+1] + data.in_data[i+2] + data.in_data[i+3]) / 7.0;
        }

    }

    // pthread_mutex_lock (&mutex_movavg);
    // pthread_mutex_unlock (&mutex_movavg);
    pthread_exit(NULL); // Important. It blocks main to support the threads it created till they’re done
}


int main(int argc, char *argv[])
{
    struct timeval start, end;
    long t_us;
    void *status;
    if (argc != 2) {
        printf("Warning: Usage: %s n\n", argv[0]);
        printf("Using NUM_THREADS = 1 as default\n");
        NUM_THREADS = 1;
    } else {
        NUM_THREADS = atoi(argv[1]); // Convert the argument to an integer
    }

    pthread_t threads[NUM_THREADS];
    printf("Creating %d Threads",NUM_THREADS);
    //pthread_mutex_init(&mutex_movavg, NULL); // mutex: dynamic initialization 

    data.in_data = (int *) calloc (num_elements, sizeof(int));
    data.f = (double *) calloc (num_elements, sizeof(double));
    read_binfile(data.in_data , num_elements , in_file , 1 );
    
    gettimeofday (&start, NULL);
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL , movavg, (void*)i);
    }
    for (int i = 0; i < NUM_THREADS; i++) pthread_join(threads[i], &status);
    gettimeofday (&end, NULL);

    for(int i = 0; i < 20; i++) printf("f[%d] = %lf\n", i , data.f[i]);
    for(int j = 0; j < 20; j++)  printf("f[%d] = %lf\n", j+1000 , data.f[j+1000]);
    for(int k = 0; k < 20; k++)  printf("f[%d] = %lf\n", k+99980 , data.f[k+99980]);


    printf ("start: %ld us\n", start.tv_usec); // start.tv_sec
    printf ("end: %ld us\n", end.tv_usec);    // end.tv_sec; 
    t_us = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec; // for ms: define t_ms as double and divide by 1000.0
    // gettimeofday: returns current time. So, when the secs increment, the us resets to 0.
    printf ("Elapsed time (only moving average computation): %ld us\n", t_us);

    free(data.in_data);
    free(data.f);
    //pthread_mutex_destroy(&mutex_movavg);
    pthread_exit(NULL); // Important. It blocks main to support the threads it created till they’re done
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