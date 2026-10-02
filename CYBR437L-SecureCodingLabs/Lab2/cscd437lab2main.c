#include "cybr437lab2.h"


int main(int argc, char**argv)
{
	int size = 4;
	int * myArray = NULL;
	
	myArray = fillArray(&size);
	printArray(size, myArray);
	size = cleanUp(&myArray);
		
}
