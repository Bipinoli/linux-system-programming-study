/*
    Processor and main memory is much much faster than disk.
    Because of this, during file write, the kernel keeps the buffer and just replies to the userspace program
    with the success reply. Kernel batches multiple writes and writebacks to the disk as it sees fit.
    The successive reads from the usersapce is handled via this buffer, so for userspace it looks like the data was committed.
    However, in case of failure (eg. power failure) the data in the buffer will be lost and no disk write occurs.

    Kernel provides a way to avoid this by providing synchronous FILE IO system calls such as fsync(), fdatasync(), 
    which forces the buffer to be synced with the disk before returing.
    However, disks themselves keep a cache so the data maynot have been exactly written in right place in the physical disk yet
    when the fsync() replies. But, that shouldn't cause any problem as disk formats such as ext3, ext4, keep a write ahead log in disk.
    https://pages.cs.wisc.edu/~remzi/OSTEP/file-journaling.pdf

    Kernel also provides Direct IO mechanism to avoid kernel detailed involement if needed. Suspecting database people might use
    this mechanism.

    Note:
    fsync() -> sync data to disk
    fdatasync() -> sync only data to disk, don't care about it's metadata

    However, they don't guarrantee that the directory inode where the file resides also reflect the changes. As that might not have been synced yet.
    If that is desired, the directory should be explicitly synced as well.

    Kernel also provides other methods of synching buffer, such as sync() call and opening writeable file in sync mode with O_SYNC.
    However, fsync() provides finer control to sync only when absolutely necessary. As sync process is costly this is desired.
*/

#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
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

    int fd = open("file_sync.txt", O_CREAT | O_WRONLY);
    if (fd == -1) {
        perror("file_sync.txt open error");
        return -1;
    }

    int rt = write(fd, buffer, sizeof(char) * length);
    if (rt == -1) {
        perror("file_sync.txt write error");
        return -1;
    }

    rt = fsync(fd);
    if (rt == -1) {
        perror("file_sync.txt buffer sync error");
        return -1;
    }
    return 0;
}