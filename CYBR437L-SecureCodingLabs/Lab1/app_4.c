#include <stdio.h>


void swapVals(double *first, double *second) {

    double temp = *first;
    *first = *second;
    *second = temp;


    //printf("The first variable contains: %f\n", *first);
    //printf("The second variable contains: %f\n", *second);

}


int main(void) {

    double first = 3.14;
    double second = 41.3;
    printf("Before Swap:\n");
    printf("First var memory address: %p\n", (void *)&first);
    printf("Second var memory address: %p\n", (void *)&second);
    printf("First var value: %f\n", first);
    printf("Second var value: %f\n", second);
    swapVals(&first, &second);

    printf("\nAfter Swap:\n");
    printf("First var memory address: %p\n", (void *)&first);
    printf("Second var memory address: %p\n", (void *)&second);
    printf("First var value: %f\n", first);
    printf("Second var value: %f\n", second);

    return 0;
}