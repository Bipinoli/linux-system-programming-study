#include<stdio.h>
#include<sys/uio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main() {
    char *contents[] = {
        "My name is Bipin Oli.\n",
        "I am 26 years old.\n",
        "I am from Nepal.\n",
    };

    struct iovec vec_buffer[3];

    int fd = open("no_git_tracking_writev.txt", O_CREAT | O_WRONLY);
    if (fd == -1) {
        perror("open failed!");
        return 1;
    }

    for (int i=0; i<3; i++) {
        vec_buffer[i].iov_base = contents[i];
        vec_buffer[i].iov_len = strlen(contents[i]);
    }

    ssize_t bytes_written = writev(fd, vec_buffer, 3);
    if (bytes_written == -1) {
        perror("writev failed!");
        return 1;
    }

    if (close(fd) == -1) {
        perror("close failed!");
        return 1;
    }

    printf("Wrote %zd bytes into the file.\n", bytes_written);


    return 0;
}