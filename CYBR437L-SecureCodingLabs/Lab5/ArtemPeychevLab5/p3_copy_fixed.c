/* Part 3, Task 2: Grow the destination so all requested items fit. */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

/* to points to the caller's array pointer, so realloc can update it.
   capacity records the number of doubles currently allocated.
   from contains at least n elements, as the assignment assumes. */
int fun(double **to, const double *from, int pos, int n, size_t *capacity) {
    /* Prevent invalid indexes and overflow while calculating the size.
       A valid request that exceeds capacity is handled by growing the array. */
    if (pos < 0 || n < 0 || n > INT_MAX - pos) return -1;
    size_t needed = (size_t)pos + (size_t)n;
    if (needed > SIZE_MAX / sizeof(double)) return -1;

    if (needed > *capacity) {
        /* Use a temporary pointer so an allocation failure does not
           lose the original array. */
        double *larger = realloc(*to, needed * sizeof(double));
        if (larger == NULL) return -1;
        for (size_t k = *capacity; k < needed; k++) larger[k] = 0;
        *to = larger;
        *capacity = needed;
    }
    for (int k = 0; k < n; k++) {
        (*to)[pos] = from[k];
        pos++;
    }
    return pos;
}

int main(void) {
    size_t capacity = 5;
    double *to = calloc(capacity, sizeof(double));
    double from[6] = {10, 20, 30, 40, 50, 60};
    if (to == NULL) return 1;
    printf("Original capacity: %zu; start: 3; number to copy: 4\n", capacity);
    int end = fun(&to, from, 3, 4, &capacity);
    if (end < 0) {
        fprintf(stderr, "Could not complete the copy.\n");
        free(to);
        return 1;
    }
    printf("New capacity: %zu; returned position: %d\n", capacity, end);
    printf("Destination: ");
    for (size_t k = 0; k < capacity; k++) printf("%.0f ", to[k]);
    printf("\n");
    free(to);
    return 0;
}
