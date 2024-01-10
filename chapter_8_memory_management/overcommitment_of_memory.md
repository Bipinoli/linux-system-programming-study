### Overcommitment of memory

Kernel performs opportunistic allocation to provide memory to processes. When a process asks for a memory, kernel commits for the memory but doesn't immediately provide that. As, needed it provides the pages and does copy-on-write whenever that makes sense. This was one of the motivation behind the invention of virtual memory in operation system.

However, if the processes end up using all the memory they ask for, the physical memory + avaialble swap in disk may not be sufficient. The kernel then finds the least improtant process with highest memory and kills that with "Out of memory" handler. 

Sometimes the system might be critical and want everything to be deterministic. In that case the "/proc/sys/vm/overcommitment_ratio" can be edited to modify the kernel's overcommitment behaviour. 