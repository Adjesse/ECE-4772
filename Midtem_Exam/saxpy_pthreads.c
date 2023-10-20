#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>


int nthreads;
int vector_len;
float *x,*y,*y_s,*y_p;
float a = 1.618;

void *saxpy (void *arg)
{
    int i, start, end;
    int Thread = (int)arg;

    start = (Thread*vector_len)/nthreads;
    end = (((Thread+1)*vector_len)/nthreads) - 1;

    for(i = start; i <= end; i++)
    {
        y_p[i] = a*x[i] + y[i];
    }

}
void saxpy_seq()
{
    for(int i = 0; i < vector_len; i++)
    {
        y_s[i] = a*x[i] + y[i];
    }
}
int main(int argc, char *argv[])
{
    struct timeval start, end;
    struct timeval start2,end2;
    long t_us;
    void *status;
    if (argc != 3) {
        printf("Warning: Usage: %s n\n", argv[0]);
        printf("Using nthreads = 1 as default\n");
        nthreads = 1;
        vector_len = 1;
    }
    else 
    {
        nthreads = atoi(argv[1]); 
        vector_len = atoi(argv[2]);
    }
    pthread_t threads[nthreads];
    printf("Creating %d Threads\n",nthreads);

    x = (float *) calloc (vector_len, sizeof(float));
    y = (float *) calloc (vector_len, sizeof(float));
    y_s = (float *) calloc (vector_len, sizeof(float));
    y_p = (float *) calloc (vector_len, sizeof(float));

    for(int i = 0; i < vector_len; i++)
    {
        x[i] = sinh((i*3.416)/(vector_len * 1.00));
        y[i] = cosh((i*3.416)/(vector_len * 1.00));
    }

    gettimeofday (&start, NULL);
    saxpy_seq();
    gettimeofday (&end, NULL);

    gettimeofday (&start2, NULL);
    for(int i = 0; i < nthreads; i++) {
        pthread_create(&threads[i], NULL , saxpy, (void*)i);
    }
    for (int i = 0; i < nthreads; i++) pthread_join(threads[i], &status);
    gettimeofday (&end2, NULL);

    printf("x(input)\t\ty(input)\t\t\ty(output)\n");
    for(int i = 0; i < 20;i++)
    {
        printf("x[%d]=%f\t\ty[%d]=%f\t\t\ty[%d]=%f\n",i,x[i],i,y[i],i,y_s[i]);
    }

    printf("\nnthreads = %d\nvector_len = %d\n\n",nthreads,vector_len);

    printf("Time Measurments\n****************\n");
    printf("pthreads implementation - nthreads = %d\n",nthreads);
    printf ("start: %ld us\n", start2.tv_usec); // start.tv_sec
    printf ("end: %ld us\n", end2.tv_usec);    // end.tv_sec; 
    t_us = (end2.tv_sec - start2.tv_sec)*1000000 + end2.tv_usec - start2.tv_usec; // for ms: define t_ms as double and divide by 1000.0
    printf ("Elapsed time: %ld us\n\n", t_us);

    printf("Sequential implementation\n");
    printf ("start: %ld us\n", start.tv_usec); // start.tv_sec
    printf ("end: %ld us\n", end.tv_usec);    // end.tv_sec; 
    t_us = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec; // for ms: define t_ms as double and divide by 1000.0
    // gettimeofday: returns current time. So, when the secs increment, the us resets to 0.
    printf ("Elapsed time: %ld us\n", t_us);


    free(x); free(y); free(y_s);
    pthread_exit(NULL);
    return 0;
}