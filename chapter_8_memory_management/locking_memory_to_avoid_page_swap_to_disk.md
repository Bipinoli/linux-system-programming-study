### Locking memory to prevent swap to disk

Sometimes we want to prevent the swap of pages to the disk. We may want this for reasons such as:

- **Determinism**: Such as with real-time-systems we don't want a sudden spike in memory access/write time due to page swap. Since, disk IO is much much slower than memory IO, this can make us miss the critical deadline.

- **Security**: The process might have just unencrypted the secret in memory. We don't want that to be swapped to disk as that is a potential security risk.

Kernel provides a way to lock memory. Mainly via two system calls. 

- mlock() - locks the provided addresses in process address space (virtual) to the memory. With this we can prevent a segment of address space from being swapped out.

- mlockall() - locks all of the address space from being swapped out. We can choose to lock the the future addresses or the one already allocated. This can be convinient to lock everything in situations like with real-time systems.


kernel provides *munlock()* and *munlockall()* system call to unlock the locks.


Kernel also provides *mincore()* system call to see if the pages are in memory on in the disk. 


Locking is done in the page level. The pages in the address span of locks are prevented from being swapped. Even if only part of the address in the page are to be locked the whole page will be locked.

Child processes created via fork() don't inherit the lock. However, as the memory is basically shared until written (copy-on-write) they are indirectly locked as well.



