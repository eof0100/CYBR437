#include <stdio.h>

int main() {

	char c = 'C'; 
	char *charPtr = &c; 


	// charPtr: memory address of c
	printf("charPtr is %p\n", (void *)charPtr); 
	// *charPtr: the value stored at the memory address in charPtr
	// since the memory address in charPtr is c, this ultimately means
	// go to memory address of c and read its value 
	printf("The value at charPtr is %c\n", *charPtr); 

	// Pointer arithmetic
	// take the memory address that p points to which is c
	// and add 2 to it which is 2 bytes since char = 1 byte
	charPtr = charPtr + 2;
	printf("Now charPtr is %p\n\n", (void *)charPtr);
    


	int nine = 9; 
	int *intPtr = &nine; 


	// intPtr: memory address of nine
	printf("intPtr is %p\n", (void *)intPtr); 
	// *intPtr: mem address intPtr points to and take that value
	printf("The value at intPtr is %d\n", *intPtr); 

	// Pointer arithmetic
	// take the memory address that intPtr points to which is nine


	// this adds 1 int to the memory address
	// since int = 4 bytes 
	// adds 8 bytes to the memory address
	intPtr = intPtr + 2;
	printf("Now intPtr is %p\n\n", (void *)intPtr);

    	// pi to 8 decimal points
	double pi = 3.14159265; 
	double *doublePtr = &pi; 


	// doublePtr: memory address of pi
	printf("doublePtr is %p\n", (void *)doublePtr); 
	// *doublePtr: mem address doublePtr points to and take that value
	printf("The value at doublePtr is %f\n", *doublePtr); 

	// Pointer arithmetic
	// take the memory address that doublePtr points to which is pi


	// this adds 1 double 
	// since double  = 8 bytes 
	// adds 16 bytes to the memory address
	doublePtr = doublePtr + 2;
	printf("Now doublePtr is %p\n", (void *)doublePtr);

}