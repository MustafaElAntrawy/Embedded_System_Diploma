#include <stdio.h>

int main(){
	float a, b;
	printf("Enter value of a: ");
	fflush(stdin); fflush(stdout);
	scanf("%f", &a);
	printf("Enter value of b: ");
	fflush(stdin); fflush(stdout);
	scanf("%f", &b);
	float temp = a;
	a = b;
	b = temp;
	printf("After swapping, value of a = %.6g\n", a);
	printf("After swapping, value of b = %.6g", b);
	return 0;
}


