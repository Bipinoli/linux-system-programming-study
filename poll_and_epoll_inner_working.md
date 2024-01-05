## Comparing inner working of poll() with epoll()


Kernel provides multiplex IO feature by maintaining the data structure to keep track of the file descriptors ready for IO. When it handles the respective interrupts it updates the data structure.

With poll() we provide the list of file descriptors we are interested in.
Kernel has to go through each file descriptor to match the file descriptors against the set it is internally maintaining. This means that the time taken is propostional to the file descriptors we want to monitor i.e O(num of file descriptors). So, if there are many file descriptors such as for a 1000s of sockets for a server, it can be slow.

Because of this epoll() was introduced in Linux. Macos has kqueue() which is equivalent to epoll(). For other systems, we can use libuv/libevent which wraps the existing system calls to provide the similar interface.

epoll() works by keeping a list of file descriptors we want to track in the kernel. With this extra datastructure in the kernel, we don't have to pass the list every time we want to poll. As kernel already has the list of file descriptors we are interested in, it can keep a set of file descriptors which is ready among them. This way, when we poll, it doesn't have to go through the list to know what to return. It already has a set that it can reply with. Which means the time taken is propostional to the number of events instead of file descriptors, i.e O(number of occured events). This way epoll() can be much faster in cases when there are many file descriptors to be monitored but the events are sporadic.

However, due to having to keep an interest list in kernel, epoll() needs more system calls. There are 3 system calls in epoll().
- epoll_create() -> to create the interest list of file descriptors in kernel
- epoll_ctl() -> to modify/delete the item from the list
- epoll_wait() -> to poll() for the ready file descriptors

Due to these extra system call overhead. It can be faster to use poll() when the number of file descriptors to monitors are much less. Also, poll() being in the POSIX standard is available across the operating systems that support POSIX such as Mac, Windows, Linux, etc.

#### Interesting Reference

[Excellent guide about the inner working of epoll() by Cindy Sridharan](https://copyconstruct.medium.com/the-method-to-epolls-madness-d9d2d6378642)