/* Part 4, Task 1: Run with "unsafe" to demonstrate the original bug.
   Run with no argument to test the corrected function. */
#include <stdio.h>
#include <limits.h>
#include <string.h>

void fun1(int a, int b) {
    int sum;
    sum = a + b; /* Original code: signed overflow is undefined behavior. */
    printf("Result = %d\n", sum);
}

void fun1_fixed(int a, int b) {
    printf("a = %d, b = %d: ", a, b);
    /* For positive b, check the upper limit before adding.
       For negative b, check the lower limit before adding.
       The checks themselves stay within the int range. */
    if ((b > 0 && a > INT_MAX - b) ||
        (b < 0 && a < INT_MIN - b)) {
        printf("Error: integer overflow.\n");
        return;
    }
    printf("Result = %d\n", a + b);
}

int main(int argc, char **argv) {
    printf("INT_MIN = %d; INT_MAX = %d\n", INT_MIN, INT_MAX);
    if (argc > 1 && strcmp(argv[1], "unsafe") == 0) {
        printf("Original function: a = INT_MAX, b = 1\n");
        fflush(stdout);
        fun1(INT_MAX, 1);
    } else {
        printf("Corrected function:\n");
        fun1_fixed(INT_MAX, 1);   /* Same test that caused overflow. */
        fun1_fixed(INT_MIN, -1);  /* Also test the lower limit. */
        fun1_fixed(20, 30);       /* Ordinary safe addition: 50. */
        fun1_fixed(-20, -30);     /* Another safe addition: -50. */
    }
    return 0;
}
