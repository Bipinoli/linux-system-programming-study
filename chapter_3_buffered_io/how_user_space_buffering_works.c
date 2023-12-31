/*
    Block writes to the kernel buffer is more efficient than character writes as it requires less system calls.
    Also if the block size aligns with kernel buffer size it will result in less maintainence work in kernel.
    We can build a buffering mechanism in user-space to do this block write. 
    Normal writes will be written to the buffer and when the buffer gets full it will be flushed to the kernel buffer 
    with block write system calls. Luckily, we don't have to implement this from scratch.
    C has a stdio library which provides user-space buffering with functions like 
    fread(), fwrite() and file stream (FILE) concept.

    Writing to the buffered stream via fwrite() simply writes to the buffer in user-space.
    If needed we can fflush() to trigger the flush to the kernel buffer manually. 
    It is important to understand that fflush() merely flushes data from the c-libray (user-space) buffer to the
    kernel buffer. It doens't mean that the data has reached the disk.
    We can explicity, call fsync() system call to trigger the disk sync with kernel buffer.
    Thus if we want to persist the file stream data to the disk immediately we should call 
    fflush() followed by fsync(). However, disk and cpu have a huge speed mismatch so this is a costly
    synchronous call, so should be called with careful thoughts.
*/


// example program
// writing & reading custom struct to the file stream
#include<stdio.h>

int main() {
    FILE *input_stream, *output_stream;
    struct Person {
        char name[40];
        unsigned int age;
    };

    struct Person bipin = { "Bipin Oli", 26};
    struct Person read_person;

    output_stream = fopen("person.data", "w");
    if (!output_stream) {
        perror("output stream");
        return 1;
    }
    int ret = fwrite(&bipin, sizeof(struct Person), 1, output_stream);
    if (ret == 0) {
        perror("fwrite error");
        return 1;
    }

    input_stream = fopen("person.data", "r");
    if (!input_stream) {
        perror("input stream");
        return 1;
    }

    // without fflush() the output file might not have been written completely
    // as file inode and data are kernel space things
    // alternatively we could fclose() which also flushes the content to kernel buffer
    fflush(output_stream);

    ret = fread(&read_person, sizeof(struct Person), 1, input_stream);
    if (ret == 0) {
        perror("no data to read");
        return 1;
    }

    if (fclose(input_stream) | fclose(output_stream)) {
        perror("fclose error");
        return 1;
    }
    
    printf("Read data: Person(%s, %d yrs old)\n", read_person.name, read_person.age);
    return 0;
}