/*
    Although, sigqueue() is in POSIX.
    It is not avaialbe in macos.

    Note: following example requires Linux
*/

#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>


void signal_handler(int signo, siginfo_t *info, void *extra) {
    // we know that this printf won't create re-entrancy issue
    // as we are not signalling when any printf is being run 
    void *ptr_val = info->si_value.sival_ptr;
    printf("Signal: %d, value: %s\n", signo, (char*)ptr_val);
    exit(0);
}


int main() {
    struct sigaction action;
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = &signal_handler;

    if (sigaction(SIGUSR2, &action, NULL) == -1) {
        perror("sigusr: sigaction");
        return 0;
    }

    printf("signal handle registered\n");
    printf("I am going to send signal to myself\n");

    char *data = "<top_secret>Custom data that I am sending to the signal. Shhhh!!</top_secret>";
    union sigval mysignal;
    mysignal.sival_ptr = data;

    sigqueue(getpid(), SIGUSR2, mysignal);

    while(1) {
            sleep(2);
    }
}