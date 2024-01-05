/*
    Note: kqueue() is the MacOS/BSD equivalent of epoll() in linux

    The following code will setup a timer that will trigger a kevent every 1 second. 
    Once it does, the process will fork and the child will execute the date(1) command.

    It is based on this article: https://wiki.netbsd.org/tutorials/kqueue_tutorial/
*/

#include<stdio.h>
#include<sys/event.h>
#include<sys/time.h>
#include<unistd.h>

int main() {

    struct kevent monitoring_event;
    struct kevent event_from_kernel;

    int kernel_queue = kqueue();
    if (kernel_queue == -1) {
        perror("kqueue()");
        return 1;
    }

    // initialize kevent structure
    uintptr_t identifier = 1;
    // EV_SET(&kev, ident, filter, flags, fflags, data, udata);
    EV_SET(&monitoring_event, identifier, EVFILT_TIMER, EV_ADD | EV_ENABLE, 0, 1000, 0);


    while (1) {
        int num_events = kevent(kernel_queue, &monitoring_event, 1, &event_from_kernel, 1, NULL);
        if (num_events < 0) {
            perror("kevent poll fail");
            return 1;
        }

        if (event_from_kernel.flags & EV_ERROR) {
            perror("kernel provided error event");
            return 1;
        }

        // create a child to show the date
        int pid = fork();
        if (pid < 0) {
            perror("fork error");
            return 1;
        }
        if (pid == 0) {
            int date_program_ret = execlp("date", "date", (char *)0);
            if (date_program_ret < 0) {
                perror("date call error");
                return 1;
            }
        }
    }


    return 0;
}