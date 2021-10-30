#include <stdio.h>
#include <string.h>

struct Distance {
	float feet;
	float inch;
};

struct Distance get_distance();
void print_sum(struct Distance d1, struct Distance d2);
int main()
{
	struct Distance dis_1, dis_2;
	printf("Enter 1st distance:\n");
	dis_1 = get_distance();
	printf("Enter 2nd distance:\n");
	dis_2 = get_distance();
	print_sum(dis_1, dis_2);
	return 0;
}

struct Distance get_distance(){
	struct Distance d;
	printf("Enter feet: ");
	fflush(stdin); fflush(stdout);
	scanf("%f", &d.feet);
	printf("Enter inch: ");
	fflush(stdin); fflush(stdout);
	scanf("%f", &d.inch);
	return d;
}

void print_sum(struct Distance d1, struct Distance d2){
	struct Distance sum;
	sum.inch = d1.inch + d2.inch;
	sum.feet = d1.feet + d2.feet + (int) sum.inch / 12;
	while(sum.inch >= 12){
		sum.inch -= 12;
	}
	printf("Sum of distances = %.0f\" - %.2f'", sum.feet, sum.inch);
}

