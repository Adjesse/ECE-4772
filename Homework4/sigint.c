#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// To compile:
// gcc basic_sigint.c -o basic_sigint

void sig_handler(int signum){ //Return type of the handler function should be void
  printf("\nInside handler function\n");
}

int main(){
  int i;
  signal(SIGINT,sig_handler); // Register signal handler

  for(i=1; ;i++){    //Infinite loop
    printf("%d : Inside main function\n",i);
    sleep(1);  // Delay for 1 second
  }
  return 0;
}
