/*
    poll() works in similar ways to select()
    but provides a better interface to use
*/

// an example program that uses poll() to simultaneously check 
// whether a read from stdin and write to stdout will block


#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/poll.h>

#define TIMEOUT 5 // 5 seconds poll timeout
#define BUFFER_LENGTH 1024

int main() {
    struct pollfd fds[2];
    
    // watch STDIN for input
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    // watch STDOUT for ability to write (almost always true)
    fds[1].fd =STDOUT_FILENO;
    fds[1].events = POLLOUT;

    // poll for events - blocks cpu
    int ret = poll(fds, 2, TIMEOUT * 1000);
    if (ret == -1) {
        perror("poll error");
        return 1;
    }
    if (ret == 0) {
        printf ("poll timeout - %d seconds elapsed.\n", TIMEOUT);
        return 0;
    }


    char buffer[BUFFER_LENGTH + 1]; // +1 because string needs to have \0 at the end
    int stdin_available = 0;

    if (fds[0].revents & POLLIN) {
        printf ("good - stdin is readable\n");
        stdin_available = 1;

        // read is guaranteed to not block as fd was provided by poll()
        int len = read (STDIN_FILENO, buffer, BUFFER_LENGTH);
        if (len == -1) {
            perror ("read");
            return 1;
        }

        if (len) {
            buffer[len] = '\0';
        } 
    } else {
        fprintf(stderr, "no input. try redirecting input to the program eg. echo 'hello' > input.txt && ./a.out < input.txt\n");
    }
    
    if (fds[1].revents & POLLOUT) {
        printf ("good - stdout is writable\n");
        if (stdin_available) {
            printf("value from stdin: %s\n", buffer);
        }
    }

}