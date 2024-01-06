## About mmap()

mmmap() is a system call that has been standardise by POSIX.
It maps the pages of a file to the memory, such as a part of a file from some offset. With this users can work with file content with normal variable operations. No extra system calls are necessary. No extra user-space buffering is needed so it avoid extra copies which can make read and write faster. With msync() system call, the memory can be synced with the disk. 

mmap() lets user choose the segment of file they want to map to the memory, however it works in the contenxt of pages, which means the mapping must exist in the unit of pages. For a very small byte mapping compared to the size of the page, there can be a lot of wasted space as the kernel maps the page but pads the rest with zeros. During sync() only the non-padded part of the pages will be examined. 

User can choose to lay the mapping in the memory location of their choice (in the process virtual space). So, the mmap() system call is not bounded in the context of heap memory. It lies outside. 

mmap() does mapping in two main modes, MAP_PRIVATE and MAP_SHARED. With MAP_PRIVATE the modifications to the memory are private to the process. This is done with copy-on-write scheme. With MAP_SHARED the kernel maps the same memeory chunks to the virtual memory of different processes so the mapping is shared. Which means the processes can observe the modificitaions as if they were observing the actual file underneath. This can be used for inter-process communication. Users must specify one of these method to use the mmap(). 

mmap() provides more detailed contorls, such as mapping the memory which doesn't have any correspondence to the real file. Which means it is just an area of allocated memrory. Internally malloc() function of libc uses mmap() like this to allocate the memory. Kernel just maps the dev/zero in such mode.

