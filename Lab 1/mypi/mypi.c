#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

int main(int argc,char **argv){
struct timeval start, end;
long t_us;
int n;

if (argc !=2){
	printf("Warning: Usage: %s n\n",argv[0]);
	printf("Using n = 10 as default\n");
	n = 10;
}
else n = atoi(argv[1]);




//double *F = (double *) calloc (n , sizeof(double));
double F  = 0.0;

gettimeofday(&start , NULL);

for(int i = 0; i < n; i++)
{
	double term = (i%2 == 0) ? 1.0 : -1.0;
	term /= (2*i + 1);
	F += term;

}

gettimeofday(&end , NULL);

printf("Vaule of pi: %lf",F*4.0);
printf("\n");
printf("start: %ld us\n",start.tv_usec);
printf("end: %ld us\n",end.tv_usec);
t_us = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec;
printf("Elapsed time: %ld us\n",t_us);

//free(F);
return 0;


}

