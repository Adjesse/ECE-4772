#include<stdio.h>
#include<unistd.h>
#include<signal.h>
 // To compile: gcc basic_sigalrm.c -o basic_sigalrm.
 
void sig_handler(int signum){ 
  printf("Inside handler function\n");
}
 
int main(){ 
  int i;
  signal(SIGALRM,sig_handler); // Register signal handler 
  alarm(2);  // Scheduled alarm after 2 seconds 
  for(i=1;;i++){ 
    printf("%d : Inside main function\n",i);
    sleep(1);  // Delay for 1 second
  }
return 0;
}