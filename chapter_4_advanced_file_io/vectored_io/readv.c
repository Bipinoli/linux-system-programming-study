#include<stdio.h>
#include<sys/uio.h>
#include<fcntl.h>
#include<unistd.h>


int main() {
    char first[23], second[20], third[18];

    int fd = open("no_git_tracking_writev.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    struct iovec io_vec[3];
    io_vec[0].iov_base = first;
    io_vec[0].iov_len = 23;
    io_vec[1].iov_base = second;
    io_vec[1].iov_len = 20;
    io_vec[2].iov_base = third;
    io_vec[2].iov_len = 18;

    ssize_t read_bytes = readv(fd, io_vec, 3);
    if (read_bytes == -1) {
        perror("readv");
        return 1;
    }

    close(fd);

    printf("read %zd bytes\n\n", read_bytes);
    for (int i=0; i<3; i++) {
        printf("%d: %s", i, (char *)io_vec[i].iov_base);
    }


    printf("%s", (char *)io_vec[0].iov_base);

    return 0;
}