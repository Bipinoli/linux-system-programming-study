/*
    Source: https://youtu.be/sFYFuBzu9Ow?t=7014

    To avoid having to parse through ELF or other ABI file we are generating a raw executable from assembly.
    We are using NASM assembler for that which works for X86 machines.
    We map the binay to the memory with mmap(). Then the pointer to the memory is casted to function pointer and called in C.
*/

#include<stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<assert.h>

int main() {
    int fd = open("hello_world", O_RDWR);
    assert(fd >= 0);

    struct stat file_stat;
    assert(fstat(fd, &file_stat) >= 0);

    void *raw_executable = mmap(NULL, file_stat.st_size, PROT_EXEC, MAP_PRIVATE, fd, 0);
    assert(raw_executable != MAP_FAILED);

    // cast raw executable to the function pointer and call it
    ((void (*)(void)) raw_executable)();

    return 0;
}