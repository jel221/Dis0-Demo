

#include <stdio.h>
#include <string.h>

int main() {
	char* a = "162 is the best";
	char b[] = "162 is the best";
	static int num1 = 1;
	int num2 = 1;

	printf("a is at address: %p\n", (void*)a);
	printf("b is at address: %p\n", (void*)b);
	printf("Reference addresses: \n");
	printf("num1 is at address: %p\n", (void*)&num1);
	printf("num2 is at address: %p\n", (void*)&num2);

	return 0;
}