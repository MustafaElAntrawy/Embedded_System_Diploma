#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "conio.h"

#define Dprintf(...) {fflush(stdout); fflush(stdin); printf(__VA_ARGS__);fflush(stdout); fflush(stdin);}


// Effective data
struct Sdata{
	int ID;
	char name[40];
	float height;
};

// Linked list node
struct SStudent{
	struct Sdata student;
	struct SStudent* PNextStudent;
};


static struct SStudent* gpFirstStudent = NULL;
// APIs
void AddStudent();
int delete_student();
void view_students();
void delete_all();
void display_student();
void list_length();
void display_student_backword();
void display_mid();
void is_loop();
void reverse();

#endif /* LINKED_LIST_H_ */
