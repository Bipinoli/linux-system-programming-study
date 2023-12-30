#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>


char *read_file(char *file_name, int *length) {
    FILE *file = fopen(file_name, "r");
    int len; char *buffer = NULL;
    if (file) {
        fseek(file, 0, SEEK_END);
        len = ftell(file);
        rewind(file);
        buffer = (char *) malloc(sizeof(char) * (len + 1));
        if (buffer) {
            fread(buffer, sizeof(char), len, file);
            buffer[len] = '\0';
        } else {
            perror("malloc error");
        }
        *length = len + 1;
        fclose(file);
    } else {
        perror("big file read error");
        exit(-1);
    }
    return buffer;
}


int main() {
    int length = 0;
    char *buffer = read_file("big_file.txt", &length);

    int fd = open("file_normal.txt", O_CREAT | O_RDWR);
    if (fd == -1) {
        perror("file_normal.txt open error");
        return -1;
    }

    int rt = write(fd, buffer, sizeof(char) * length);
    if (rt == -1) {
        perror("file_normal.txt write error");
        return -1;
    }

    free(buffer);
    return 0;
}