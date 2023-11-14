#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sig_handler_int(int signum){
    printf("\nInside handler function\n");
    alarm(5);
}
void sig_handler_alrm(int signum){
    printf("Inside handler function\n");
    alarm(2);
}

int main(){
    int i;
    signal (SIGINT, sig_handler_int); // Register signal handler
    signal (SIGALRM, sig_handler_alrm); // Register signal handler
    alarm(2);
    for(i=1;;i++){ // Infinite loop
        printf("%d : Inside main function\n",i);
        sleep(1); } // Delay for 1 second
    return 0;
}
