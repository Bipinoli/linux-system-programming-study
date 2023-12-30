/*
    Select takes a set of file descripters and checks them to see if any of them are ready for reading/writing/error. 
    It takes a timeout arugment to avoid waiting infinitely. Timeout = 0 means it will immediately return descriptors
    that are ready, i.e we are polling the ready ones.

    Compare this to read() which might infinitely wait for the file descriptoer. We can see, that it provides 
    an better way to do IO without spending too much of the cpu time just waiting for IO.

    However, with select we still have to poll for file descriptors agian and again. Kernel provides epoll() system
    call which is more of an event/iterrupt based mechansim by which we don't have to poll again and again.

    Furthermore, select() has it's hidden gotchas in how it should be used.
    For eg. the first param 'n' should be the value MAX_FD + 1 where MAX_FD is the maximum fd that we are inspecting.
    also the timout and other arugments need will be modified in function return so they need to be reinitialized before
    each select() call

    There is a better alternative to select() called poll() which provides an easier interface.

    In summary,
    epoll() > poll() > select()
*/



// This example:
// blocks waiting for input on stdin for up to 5 seconds.
#include<stdio.h>
#include<sys/time.h>
#include<unistd.h>
#include<sys/types.h>

#define TIMEOUT 5 // timeout of 5 seconds
#define BUFFER_LENGTH 1024

int main() {
    // wait on STDIN for input
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    // timeout
    struct timeval tv;
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;

    // block until we have timeout or ready fds with event
    int ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
    if (ret == -1) {
        perror("select error");
        return 1;
    }
    if (ret == 0) {
        printf ("selct timeout - %d seconds elapsed.\n", TIMEOUT);
        return 0;
    }

    // check if our fd is ready
    if (FD_ISSET(STDIN_FILENO, &readfds)) {
        char buffer[BUFFER_LENGTH + 1]; // +1 because string needs to have \0 at the end

        // read is guaranteed to not block as fd was provided by select
        int len = read (STDIN_FILENO, buffer, BUFFER_LENGTH);
        if (len == -1) {
            perror ("read");
            return 1;
        }

        if (len) {
            buffer[len] = '\0';
            printf ("read: %s\n", buffer);
        }
        return 0;
    }

    fprintf(stderr, "disaster - this shoudld never happen!!!\n");
    return 1;
}