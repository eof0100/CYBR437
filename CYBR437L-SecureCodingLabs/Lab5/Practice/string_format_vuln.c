#include <stdio.h>

int add(int a, int b) {

	return a + b;	
}


int main() {


	//int x = add(2, 2); 
	//printf("\n%d\n", x);
	
	// declare function pointer
	int (*func_ptr) (int, int); 
	// assign address to it of add func address
	func_ptr = &add;

	// call the function
	int result1 = func_ptr(5, 3);
	int result2 = (*func_ptr) (10, 20);


	// print results
	printf("Result 1: %d\n", result1);
	printf("Result 2: %d\n", result2);
}

