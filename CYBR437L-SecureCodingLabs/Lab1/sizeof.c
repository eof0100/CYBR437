#include <stdio.h>

int main(void) {


	char chars = 'a';
	int ints = 1;
	float floats = 3.14;
	double doubles = 3.149876046573;

	printf("Char size: %zu\n", sizeof(chars));
	printf("Int size: %zu\n", sizeof(ints));
	printf("Float size: %zu\n", sizeof(floats));
	printf("Double size: %zu\n", sizeof(doubles));

	return 0;
}
