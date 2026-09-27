#include <stdio.h>

int main() {

	char c = 'C'; 
	char *p = &c; 


	// p: memory address of c
	printf("p is %p\n", p); 
	// *p: the value stored at the memory address in p
	// since memory address of p is c this ultimately means
	// go to memory address of c and read its value 
	printf("The value at p is %c\n", *p); 

	// Pointer arithmetic
	// take the memory address that p points to which is c
	// and add one to ti
	p = p + 1;
	printf("Now p is %p\n\n", p);

}
