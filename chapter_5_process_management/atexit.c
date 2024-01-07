/*
    atexit() function is the POSIX standardised library call (libc).
    It is not a system call. It allows for a function to be called when the process exits.
    Mutltiple functions can be registerd with atexit() and they will be called in LIFO stack order.
    However, there is a maxiumum number (ATEXIT_MAX) of such functions allowed which 
    can be viewed with sysconf() library call (libc).
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>

void on_exit_function1(void) {
    printf("func1 - atexit() called me :)\n");
}

void on_exit_function2(void) {
    printf("func2 - atexit() called me :)\n");
}

void on_exit_function3(void) {
    printf("func3 - atexit() called me :)\n");
}

void register_at_exit(void(*function)(void)) {
    if(atexit(function) == -1) {
        perror("atexit failed");
        exit(1);
    }
}

int main() {

    long at_exit_max_funcs = sysconf(_SC_ATEXIT_MAX);
    assert(at_exit_max_funcs >= 3);
    printf("%ld atexit function can be registered \n", at_exit_max_funcs);

    register_at_exit(on_exit_function1);
    register_at_exit(on_exit_function2);
    register_at_exit(on_exit_function3);

    printf("program is exiting!\n");
    return 0;
}