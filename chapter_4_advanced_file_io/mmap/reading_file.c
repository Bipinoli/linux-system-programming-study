#include<stdio.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<sys/stat.h>

int main() {

    int fd = open("file.txt", O_RDONLY);
    if (fd < 0) {
        perror("open failed");
        return 1;
    }

    struct stat file_stat;
    if (fstat(fd, &file_stat) < 0) {
        perror("fstat failed");
        return 1;
    }

    if (!S_ISREG(file_stat.st_mode)) {
        perror("file.txt is not a file");
        return 1;
    }


    char *mem = mmap(0, file_stat.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    for (int i=0; i<file_stat.st_size; i++) {
        putchar(mem[i]);
    }
    printf("\n");


    if (munmap(mem, file_stat.st_size) < 0) {
        perror("munmap error");
        return 1;
    }

    return 0;
}