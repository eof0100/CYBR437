/* Part 4, Task 2: Run with "unsafe" to demonstrate the original bug.
   Run with no argument to test the corrected function. */
#include <stdio.h>
#include <limits.h>
#include <string.h>

void fun2(int x, int y) {
    int diff;
    diff = x - y; /* Original code: signed overflow is undefined behavior. */
    printf("Result = %d\n", diff);
}

void fun2_fixed(int x, int y) {
    printf("x = %d, y = %d: ", x, y);
    /* Subtracting positive y can go below INT_MIN.
       Subtracting negative y can go above INT_MAX.
       Check before carrying out the subtraction. */
    if ((y > 0 && x < INT_MIN + y) ||
        (y < 0 && x > INT_MAX + y)) {
        printf("Error: integer overflow.\n");
        return;
    }
    printf("Result = %d\n", x - y);
}

int main(int argc, char **argv) {
    printf("INT_MIN = %d; INT_MAX = %d\n", INT_MIN, INT_MAX);
    if (argc > 1 && strcmp(argv[1], "unsafe") == 0) {
        printf("Original function: x = INT_MIN, y = 1\n");
        fflush(stdout);
        fun2(INT_MIN, 1);
    } else {
        printf("Corrected function:\n");
        fun2_fixed(INT_MIN, 1);   /* Same test that caused overflow. */
        fun2_fixed(INT_MAX, -1);  /* Also test the upper limit. */
        fun2_fixed(30, 20);       /* Ordinary safe subtraction: 10. */
        fun2_fixed(-20, 30);      /* Another safe subtraction: -50. */
    }
    return 0;
}
