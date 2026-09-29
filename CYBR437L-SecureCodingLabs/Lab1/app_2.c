#include <stdio.h>


void fun1(int xval) {
    int x = xval; 
    // x: memory of address x
    // *x: the value stored at the memory address in x
    // below print address and value of x 
    printf("Memory address: %p\n", &x);
    printf("Value stored: %d\n", x);
}

void fun2() {
    int y = 10;
    // print address and valye of y below 
    printf("Memory address: %p\n", &y); 
    printf("Value stored: %d", y);
}

int main(void) {

    fun1(200);
    fun2();
    return 0; 
}