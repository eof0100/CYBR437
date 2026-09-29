#include <stdio.h>

/**
 *  Takes two arguments and prints out
 *  address, value, and amount of memory
 *  variable occupies.
 */
void func(double varDouble, int varInt) {

    double x = varDouble;
    int y = varInt;

    printf("Address of x of type double: %p\n", &x);
    printf("Value of x of type double: %f\n", x);
    printf("Variable x occupies memory (bytes): %zu\n\n", sizeof(x)); 

    printf("Address of y of type int: %p\n", &y);
    printf("Value of y of type int: %d\n", y);
    printf("Variable y occupies memory (bytes): %zu\n", sizeof(y)); 

}


int main(void) {

    func(3.14, 10);
    return 0;
}