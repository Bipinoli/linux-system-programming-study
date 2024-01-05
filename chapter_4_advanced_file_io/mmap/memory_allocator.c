/*
    source: https://youtu.be/sFYFuBzu9Ow?t=1187

    Using mmap() sys call to allocate memory to calculate N fibonacci numbers
*/

#include<stdio.h>
#include<sys/mman.h>
#include<assert.h>

#define N 12


int main() {

    int *buffer = mmap(NULL, N * sizeof(int), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (buffer == MAP_FAILED) {
        perror("mmap failed!");
        return 1;
    }
    
    assert(N >= 2);
    buffer[0] = 1;
    buffer[1] = 1;
    for (int i=2; i<N; i++) {
        buffer[i] = buffer[i-1] + buffer[i-2];
    }

    for (int i=0; i<N; i++) {
        printf("%d, ", buffer[i]);
    }
    printf("\n");

    assert(munmap(buffer, N * sizeof(int)) == 0);

    return 0;
}