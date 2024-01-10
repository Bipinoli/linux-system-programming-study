## About mmap()

mmmap() is a system call that has been standardise by POSIX.
It maps the pages of a file to the memory, such as a part of a file from some offset. With this users can work with file content with normal variable operations. No extra system calls are necessary. No extra user-space buffering is needed so it avoid extra copies which can make read and write faster. With msync() system call, the memory can be flushed to sync with the disk. Since the kernel is not directly involved and is letting the process directly read/modify the file's pages in kernel page cache, the process has to call msync() to make sure that the memory is synced with the file-system.

mmap() lets user choose the segment of file they want to map to the memory, however it works in the contenxt of pages, which means the mapping must exist in the unit of pages. For a very small byte mapping compared to the size of the page, there can be a lot of wasted space as the kernel maps the page but pads the rest with zeros. During msync() only the non-padded part of the pages will be examined.

User can choose to lay the mapping in the memory location of their choice (in the process virtual space). So, the mmap() system call is not bounded in the context of heap memory. It lies outside. 

mmap() does mapping in two main modes, MAP_PRIVATE and MAP_SHARED. With MAP_PRIVATE the modifications to the memory are private to the process. This is done with copy-on-write scheme. With MAP_SHARED the kernel maps the same memeory chunks to the virtual memory of different processes so the mapping is shared. Which means the processes can observe the modificitaions as if they were observing the actual file underneath. This can be used for inter-process communication. Users must specify one of these method to use the mmap(). 

mmap() provides more detailed contorls, such as mapping the memory which doesn't have any correspondence to the real file. Which means it is just an area of allocated memrory. Internally malloc() function of libc uses mmap() like this to allocate the memory. On an os without MAP_ANONYMOUS dev/zero file can be mapped which effectively gives the same result. 

mmap() has various protection mode (read, write, execute). It can even map the memory to allow the pages to be executed although that may require parsing the binary (ELF as per linux ABI) etc. See follwing discussions: 
- [Can I exec an entirely new process without an executable file?](https://unix.stackexchange.com/questions/230472/can-i-exec-an-entirely-new-process-without-an-executable-file)
- [How can one use exec() to execute a file stored in a buffer?](https://www.reddit.com/r/C_Programming/comments/ycrmao/how_can_one_use_exec_to_execute_a_file_stored_in/)
- [Memory map executable code boilerplate (with mmap)](https://gist.github.com/rLinks234/6c7000fda4d2ab4c6b8bc52479b03353)

I have written an example program which maps the raw binary to the memeory and runs that. Check that if you are interested :)