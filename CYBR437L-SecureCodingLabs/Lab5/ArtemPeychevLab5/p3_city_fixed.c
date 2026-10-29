/* Part 3, Task 1: Read the full city instead of rejecting long names. */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(void) {
    char *city = NULL;
    size_t capacity = 0;
    printf("Enter city: ");
    /* On Linux, getline allocates space and grows it when needed.
       The return value includes the newline but not the final null byte. */
    ssize_t length = getline(&city, &capacity, stdin);
    if (length < 0) {
        fprintf(stderr, "Could not read city.\n");
        free(city);
        return 1;
    }
    if (length > 0 && city[length - 1] == '\n') {
        city[length - 1] = '\0';
    }
    printf("City: %s\n", city);
    free(city); /* Release the memory that getline allocated. */
    return 0;
}
