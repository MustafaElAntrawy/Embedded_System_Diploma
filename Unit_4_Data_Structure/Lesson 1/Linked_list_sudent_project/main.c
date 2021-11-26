#include "linked_list.h"

int main(){
	char temp_text[40];
	while (1){
		Dprintf("\n ========================");
		Dprintf("\n\t Choose one of the following options\n");
		Dprintf("\n 1- Add Student");
		Dprintf("\n 2- Delete Student");
		Dprintf("\n 3- View all Students");
		Dprintf("\n 4- Delete all Students");
		Dprintf("\n 5- View student by index");
		Dprintf("\n 6- Display list length");
		Dprintf("\n 7- View student by index from the end of the list");
		Dprintf("\n 8- View the middle student");
		Dprintf("\n 9- Find if there is a loop");
		Dprintf("\n 10- Reverse the list");
		Dprintf("\n\t Enter option number: ");
		gets(temp_text);
		Dprintf("\n ========================");
		switch(atoi(temp_text)){
		case 1:
			AddStudent();
			break;
		case 2:
			delete_student();
			break;
		case 3:
			view_students();
			break;
		case 4:
			delete_all();
			break;
		case 5:
			display_student();
			break;
		case 6:
			list_length();
			break;
		case 7:
			display_student_backword();
			break;
		case 8:
			display_mid();
			break;
		case 9:
			is_loop();
			break;
		case 10:
			reverse();
			break;
		default:
			printf("\n Wrong option");
			break;
		}
	}
	return 0;
}
