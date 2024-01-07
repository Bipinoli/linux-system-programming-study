### nice() and setpriority()

Kernel provides various ways for user process to influence the cpu and io scheduling.
Such as nice() and setpriority() system calls. These sys calls influence how much timeslice
the schedular will allocate for the process. Only root user can increase the priority however
normal users can decrease the priority.

Kernel gives timeslice to each process in the runnable list.
It schedules them and as the timeslice is used up they move to the non-runnable list.
This way runnable processes are scheduled one by one until the timeslice is used up.
However, if the process does any IO activity, it will automatically be stopped until the IO succeds.
During this time other runnable processes can be scheduled.

Kernel keeps track of the IO and cpu time spent by processes and adjusts their priority.
It gives more priority to the IO bound processes as they are stopping frequently even before
using up their timeslice and are usually interactive processes.


### processor affinity

In a multiprocessor system, the process schedular has a responsiblity of balancing the load between the avaiable processes. Each cpus have their own cpu cache, cpu cache speed up the processes when they have the time locality behavior in use of data. So, moving a process from one cpu to another means the process won't get to use the cache and furthermore the cache in the old cpu need to be invalidated as well. Schedulars need to balance this tradeoff. 

Schedulars prefer to schedule the process to the same cpu. They do this with a concept called processor affinity. Affinity refers to the liklihood of the process to be scheduled to the same cpu. This tendency is refered as the "soft affinity" as it is just an inclination but not a hard guarantee. However, if the process really wants to benefit from the cache it can set a "hard affinity" by fixing the process to only one cpu, this can be done with the help of sched_setaffinity() and sched_getaffinity() system calls.

