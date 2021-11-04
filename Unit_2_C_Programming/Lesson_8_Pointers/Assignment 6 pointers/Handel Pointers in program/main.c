#include <stdio.h>

int main(){
	int m = 29;
	printf("Address of m : 0x%x\n", (unsigned int)&m);
	printf("Value of m : %d\n", m);

	int* ab = &m;
	printf("Address of pointer ab : 0x%x\n", (unsigned int)ab);
	printf("Content of pointer ab : %d\n", *ab);

	m = 34;
	printf("Address of pointer ab : 0x%x\n", (unsigned int)ab);
	printf("Content of pointer ab : %d\n", *ab);

	*ab = 7;
	printf("Address of m : 0x%x\n", (unsigned int)&m);
	printf("Value of m : %d\n", m);


	return 0;
}
