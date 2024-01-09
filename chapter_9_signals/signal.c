/*
    Signal can arrive at any point in the process execution. 
    The process might have been doing something critical like writing to a shared buffer,
    updating some critical data-stracture etc. When the process later resumes
    it has to be able to continue without problem.
    So, the signal handler needs to make sure it doesn't mess up anything. Signal handler
    should assume that the process was in the middle of something important and shoudln't
    use any code that is not re-entrant safe, such as file IO buffer etc.

    If the handler wants to work with the same datastructure that the main program is working on,
    we can use the sigprocmask() mechanism. With sigprocmask() we can block the signal, so before the
    process works with any critical data it can block the signals. Later, the process can retrieve
    pending signals with sigpending() / sigsuspend().

    Signals provide a way to do IPC. With sigaction() and sigqueue() we can even send a payload information
    along with the signal. However, the re-entrancy limitation makes it difficult to safely use signals.
    For IPC, there are better alternative than signals such as event-loops, sockets, pipes, etc.
*/

#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<assert.h>

void sigint_handler(int signo) {
    assert(signo == SIGINT);
    // printf is not reentrant safe function
    // so it shouldn't be used in signal handler
    // however I am using here for ease of demo
    printf("\nhandled the SIGINT signal \n");
}

int main() {
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("cannot handle signal SIGINT");
        return 1;
    }

    // pause() system call stops the process and invokes scheduler to schedule another process
    // the process will resume only when a signal is received
    while (1) {
        printf("pausing the process. If you want to resume, send interrupt signal SIGINT\n");
        pause();
    }

    return 0;
}