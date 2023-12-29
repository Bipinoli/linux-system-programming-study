#include <stdio.h>
#include <errno.h>
#include <string.h>

int main() {

    // errno, -> errno.h
    errno = EACCES;
    perror("should give permission denied error -> ");

    // strerror -> string.h
    fprintf(stderr, "Another error: %s\n", strerror(E2BIG));
}