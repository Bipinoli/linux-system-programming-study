/*
    Linux kernel provides file system events via inotify APIs.
    Kernel provides bunch of APIs to monitor filesystem events.
    inotify can be used to monitor individual file or directory to monitor for events. 
    Each watch to the directory only watches the directory and the files within it 
    but it won't watch for events inside the subdirectory. 

    Inotify() however is not in the POSIX standard. 
    So, other tecnhinques should be used in other OS, 
    such as using an existing tools such as libuv/libevent etc.


    In this example:
    I want to create a child process which creates a file and deletes a file.
    And in the parent I want to register for filesystem events and log the file events 
    to the stdout.

    NOTE: This example will work only on Linux.
*/

#include<stdio.h>
#include<sys/inotify.h>
#include<fcntl.h>
#include<unistd.h>
#include<assert.h>

#define EVENT_SIZE  (sizeof(struct inotify_event))
#define BUF_LEN     (1024 * (EVENT_SIZE + 16))

void read_filesystem_events(int notif_file, char *buffer) {
        int read_len = read(notif_file, buffer, BUF_LEN);
        assert(read_len > 0);

        int i = 0;
        while (i < read_len) {
                struct inotify_event *event = (struct inotify_event *) &buffer[i];
                if (event->len) {
                        if (event->mask & IN_CREATE) {
                                printf("Event: %s file was created.\n", event->name);
                        }
                        if (event->mask & IN_DELETE) {
                                printf("Event: %s file was deleted.\n", event->name);
                        }
                }
                i += EVENT_SIZE + event->len;
        }
}

int main() {
        pid_t child_pid = fork();
        assert(child_pid >= 0);

        if (child_pid == 0) {
                // child process

                // create a file
                int fd = open("special_file.txt", O_CREAT | O_RDWR);
                assert(fd >= 0);
                assert(close(fd) >= 0);

                // delete a file
                assert(remove("special_file.txt") >= 0);

        } else {
                // parent process
                char buffer[BUF_LEN];

                int notif_file = inotify_init();
                assert(notif_file >= 0);

                // watch for file creation and deletion in the directory
                int watch_id = inotify_add_watch(notif_file, ".", IN_CREATE | IN_DELETE);

                // poll for filesystem events
                for (int i=0; i<2; i++) {
                        read_filesystem_events(notif_file, (char *)&buffer);
                }

                (void) inotify_rm_watch(notif_file, watch_id);
                (void) close(notif_file);

        }

        return 0;
}