#include <stdio.h>

int main() {

	int nine = 9; 
	int *p = &nine; 


	// p: memory address of nine
	printf("p is %p\n", (void *)p); 
	// *p: mem address p points to and take that value
	printf("The value at p is %d\n", *p); 

	// Pointer arithmetic
	// take the memory address that p points to which is c


	// this adds 1 int to the memory address
	// since int = 4 bytes 
	// adds 4 bytes to the memory address
	p = p + 1;
	printf("Now p is %p\n", (void *)p);

}
