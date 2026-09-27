#include <stdio.h>

int main() {

	// pi to 8 decimal points
	double pi = 3.14159265; 
	double *p = &pi; 


	// p: memory address of nine
	printf("p is %p\n", (void *)p); 
	// *p: mem address p points to and take that value
	printf("The value at p is %f\n", *p); 

	// Pointer arithmetic
	// take the memory address that p points to which is c


	// this adds 1 double 
	// since double  = 8 bytes 
	// adds 8 bytes to the memory address
	p = p + 1;
	printf("Now p is %p\n", (void *)p);

}
