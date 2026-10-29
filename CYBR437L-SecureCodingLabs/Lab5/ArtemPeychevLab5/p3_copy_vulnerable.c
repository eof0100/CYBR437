/* Part 3, Task 2: Original function with a main that tests its bounds. */
#include <stdio.h>

int fun(double *to, double *from, int pos, int n) {
    int k;
    for (k = 0; k < n; k++) {
        to[pos] = from[k]; /* No check of the destination capacity. */
        pos++;
    }
    return pos;
}

int main(void) {
    double to[5] = {0};
    double from[6] = {10, 20, 30, 40, 50, 60};
    printf("Destination capacity: 5; start: 3; number to copy: 4\n");
    printf("This tries to write destination indexes 3, 4, 5, and 6.\n");
    fflush(stdout);
    int end = fun(to, from, 3, 4);
    printf("Returned position: %d\n", end);
    for (int k = 0; k < 5; k++) printf("%.0f ", to[k]);
    printf("\n");
    return 0;
}
