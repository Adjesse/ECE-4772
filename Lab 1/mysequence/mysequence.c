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

//printf("Enter the value of n: ");
//scanf("%d",&n);


int *F = (int *) calloc (n , sizeof(int));
F[0] = 1;
F[1] = 0;
F[2] = 0;

gettimeofday(&start , NULL);

for(int i = 3; i < n; i++)
{
    F[i] = F[i-2] + F[i-3];

}

gettimeofday(&end , NULL);

printf("The sequence is: ");
for(int i = 0; i<=n;i++)
{
    printf("%d ",F[i]);


}
printf("\n");
printf("start: %ld us\n",start.tv_usec);
printf("end: %ld us\n",end.tv_usec);
t_us = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec;
printf("Elapsed time: %ld us\n",t_us);

free(F);
return 0;


}

