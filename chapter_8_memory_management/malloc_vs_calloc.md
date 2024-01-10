### malloc() vs calloc()

malloc() and calloc() both allocate the memory in heap. When the requested memory size is too big they may also mmap() outside the existing heap as a different heap, to limit fragmantation. They also make use of brk() system call to adjust the boundary of heap as needed.

However, they differ in a way that with calloc() the kernel tries to find the area in memory with null values if possible.
So, it can be more efficient that malloc() + memeset() with null.