#include <stdio.h>

struct point {
	int x;
	int y;
};

int main() {
	struct point p1;
	printf("%d\n", p1.x = 1);
	struct point* p2;
	printf("%d\n", p2->x = 1);
}