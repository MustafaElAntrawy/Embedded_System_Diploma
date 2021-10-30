#include <stdio.h>
#include <string.h>

struct Complex {
	float imj;
	float real;
};

struct Complex get_number();
void print_sum(struct Complex n1, struct Complex n2);

int main()
{
	struct Complex num_1, num_2;
	printf("Enter 1st complex number:\n");
	num_1 = get_number();
	printf("Enter 2nd complex number:\n");
	num_2 = get_number();
	print_sum(num_1, num_2);
	return 0;
}

struct Complex get_number(){
	struct Complex num;
	printf("Enter real and imaginary representatively: ");
	fflush(stdin); fflush(stdout);
	scanf("%f %f", &num.real, &num.imj);
	return num;
}

void print_sum(struct Complex n1, struct Complex n2){
	printf("Sum= %.2f + %.2fi\n", n1.real + n2.real, n1.imj + n2.imj);
}
