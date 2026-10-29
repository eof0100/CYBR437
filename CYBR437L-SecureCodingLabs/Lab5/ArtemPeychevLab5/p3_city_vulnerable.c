/* Part 3, Task 1: Original vulnerable code from the assignment. */
#include <stdio.h>
int main(void) {
    char city[10];
    printf("Enter city: ");
    /* No width limit: a long word writes beyond the 10 byte array. */
    scanf("%s", city);
    printf("City: %s\n", city);
    return 0;
}
