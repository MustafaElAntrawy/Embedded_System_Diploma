#include <stdio.h>
#include <string.h>

struct Student {
char name[30];
int roll_number;
float marks;
};

struct Student get_student();
void print_student(struct Student student);
int main()
{
struct Student student;
printf("Enter students information:\n");
student = get_student();
printf("Displaying information: \n");
print_student(student);
return 0;
}

struct Student get_student(){
struct Student student;
printf("Enter student name: ");
fflush(stdin); fflush(stdout);
gets(student.name);
printf("Enter roll number: ");
fflush(stdin); fflush(stdout);
scanf("%d", &student.roll_number);
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
