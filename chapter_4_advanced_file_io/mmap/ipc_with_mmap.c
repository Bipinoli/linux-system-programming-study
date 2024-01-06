#include<stdio.h>
#include<assert.h>
#include<sys/wait.h>
#include <unistd.h>
#include<sys/mman.h>

int main() {
    int normal_memory[5] = {1,2,3,4,5};

    // create shared memory
    // MAP_SHARED = shared memory map
    // MAP_ANONNYMOUS = no corresponding file (i.e just memory)
    int *shared_memory = mmap(NULL, sizeof(int) * 5, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    for (int i=0; i<5; i++) {
        shared_memory[i] = i+1;
    }

    pid_t child_pid = fork();
    assert(child_pid != -1);

    if (child_pid == 0) {
        // child modifies normal memory
        for (int i=0; i<5; i++) {
            normal_memory[i] = 100;
        }
        // child modifies shared memory
        for (int i=0; i<5; i++) {
            shared_memory[i] = 100;
        }
    } else {
        // parent
        pid_t ret = waitpid(child_pid, NULL, 0);
        assert(ret != -1);

        // check that the normal memory in parent hasn't been modified
        // as process have their own virtual memory and memory address isolation
        printf("Normal memory content in parent: ");
        for (int i=0; i<5; i++) {
            assert(normal_memory[i] == i+1);
            printf(" %d", normal_memory[i]);
        }   

        printf("\nShared memory content in parent: ");
        // check that the shared memory in the parent has been modified by the child
        for (int i=0; i<5; i++) {
            assert(shared_memory[i] == 100);
            printf(" %d", shared_memory[i]);
        }
        printf("\n");
    }

    return 0;
}