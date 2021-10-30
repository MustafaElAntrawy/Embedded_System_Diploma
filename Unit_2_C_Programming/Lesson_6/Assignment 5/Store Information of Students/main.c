#include <stdio.h>
#include <string.h>

struct Student {
	char name[30];
	int roll_number;
	float marks;
};

struct Student get_student();
void print_student(struct Student student);

int main(){
	struct Student students[10];
	printf("Enter students information:\n");
	int i;
	for(i = 0; i < 10; ++i){
		printf("For roll number %d\n" , i+1);
		students[i] = get_student();
		students[i].roll_number = i + 1;
	}
	printf("Displaying information: \n");
	for(i = 0; i < 10; ++i){
		printf("Information for roll number %d\n" , i+1);
		print_student(students[i]);
	}
return 0;
}

struct Student get_student(){
	struct Student student;
	printf("Enter student name: ");
	fflush(stdin); fflush(stdout);
	gets(student.name);
	printf("Enter marks: ");
	fflush(stdin); fflush(stdout);
	scanf("%f", &student.marks);
	return student;
}

void print_student(struct Student student){
	printf("name: %s\n", student.name);
	printf("Roll: %d\n", student.roll_number);
	printf("Marks: %f\n", student.marks);
}
