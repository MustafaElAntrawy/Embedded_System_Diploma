#include <stdio.h>
#include <string.h>

struct Employee{
	char name[30];
	int id;
};

int main(){
	struct Employee employee1, employee2 , employee3;
	strcpy(employee1.name,"Ahmad"); employee1.id = 20;
	strcpy(employee2.name, "Amr"); employee2.id = 21;
	strcpy(employee3.name, "Maher"); employee3.id = 22;

	struct Employee (*arr[]) = {&employee1, &employee2, &employee3};
	struct Employee (*(*ptr)[3]) = &arr;

	printf("First Employee name: %s\n", (**(*ptr)).name);
	printf("First Employee Id: %d\n", (**(*ptr)).id);
	printf("Second Employee name: %s\n", (**(*ptr + 1)).name);
	printf("Second Employee Id: %d\n", (**(*ptr + 1)).id);
	printf("Third Employee name: %s\n", (**(*ptr + 2)).name);
	printf("Third Employee Id: %d\n", (**(*ptr + 2)).id);

	return 0;
}
