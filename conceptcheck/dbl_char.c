#include <stdio.h>

/*
	Try compiling this code in two different ways:
	gcc -m32 dbl_char.c -o dbl_char32
	gcc -m64 dbl_char.c -o dbl_char64
*/

int main() {
	char** dbl_char = NULL;
	
	return sizeof(*dbl_char);
	//printf("%lu\n", sizeof(*dbl_char)); // Why doesn't this error? Refer to https://en.wikipedia.org/wiki/Sizeof
}
