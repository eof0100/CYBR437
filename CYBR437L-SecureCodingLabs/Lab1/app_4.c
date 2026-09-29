#include <stdio.h>


void swapVals(double *first, double *second) {

    // temp var since $first is overridden 
    // assign value of first var to temp
    double temp = *first;
    // assign value of second to value of first
    *first = *second;
    // assign value of temp to value of temp which stores val of first
    *second = temp;


}


int main(void) {

    double first = 3.14;
    double second = 41.3;
    // Before swap
    printf("Before Swap:\n");
    printf("First var memory address: %p\n", (void *)&first);
    printf("Second var memory address: %p\n", (void *)&second);
    printf("First var value: %f\n", first);
    printf("Second var value: %f\n", second);
    swapVals(&first, &second);

    // After Swap
    printf("\nAfter Swap:\n");
    printf("First var memory address: %p\n", (void *)&first);
    printf("Second var memory address: %p\n", (void *)&second);
    printf("First var value: %f\n", first);
    printf("Second var value: %f\n", second);

    return 0;
}