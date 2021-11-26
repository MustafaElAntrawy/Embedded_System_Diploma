#include "linked_list.h"

void AddStudent(){

	struct SStudent *pNewStudent = NULL;
	struct SStudent *pLastStudent = NULL;
	char temp_text[40];
	// Cheek if list is empty
	if(gpFirstStudent == NULL){
		// Create new record
		pNewStudent = (struct SStudent* ) malloc (sizeof (struct SStudent));
		// Assign to gpfirst
		gpFirstStudent = pNewStudent;
	}
	else{
		pLastStudent = gpFirstStudent;
		while( pLastStudent->PNextStudent){
			pLastStudent = pLastStudent->PNextStudent;
		}
		pNewStudent = (struct SStudent* ) malloc (sizeof (struct SStudent));
		pLastStudent->PNextStudent = pNewStudent;
	}
	//Fill student data
	Dprintf("\nEnter Student ID: ");
	gets(temp_text);
	pNewStudent->student.ID = atoi(temp_text);

	Dprintf("\nEnter Student full name: ");
	gets(pNewStudent->student.name);

	Dprintf("\nEnter Student height: ");
	gets(temp_text);
	pNewStudent->student.height = atof(temp_text);

	pNewStudent->PNextStudent = NULL;
}

int delete_student(){
	char temp_text[40];
	unsigned selected_id;
	// Get the selected id
	Dprintf("\nEnter Student ID: ");
	gets(temp_text);
	selected_id = atoi(temp_text);
	// Cheek if list is empty
	if (gpFirstStudent){
		struct SStudent *pSelectedStudent = gpFirstStudent;
		struct SStudent *pPreviosStudent = NULL;
		while(pSelectedStudent){
			if(pSelectedStudent->student.ID == selected_id){
				if (pPreviosStudent){
					pPreviosStudent->PNextStudent = pSelectedStudent->PNextStudent;
				}else{
					gpFirstStudent = pSelectedStudent->PNextStudent;
				}
				free(pSelectedStudent);
				return 1;
			}
			pPreviosStudent = pSelectedStudent;
			pSelectedStudent = pSelectedStudent->PNextStudent;
		}
	}
	Dprintf("\n Student with id = %d is not in the list", selected_id);
	return 0;
}

void view_students(){
	struct SStudent *pCurrentStudent = gpFirstStudent;
	int count = 0;
	if(!gpFirstStudent){
		Dprintf("\nList is empty");
	}
	else{
		while(pCurrentStudent){
			Dprintf("\n Record number %d", count+1);
			Dprintf("\n \t ID : %d", pCurrentStudent->student.ID);
			Dprintf("\n \t Name : %s", pCurrentStudent->student.name);
			Dprintf("\n \t height : %f", pCurrentStudent->student.height);
			pCurrentStudent = pCurrentStudent->PNextStudent;
			count++;
		}
	}
}

void delete_all(){
	struct SStudent *pCurrentStudent = gpFirstStudent;
	if(!gpFirstStudent){
		Dprintf("\nList is empty");
	}
	else{
		while(pCurrentStudent){
			struct SStudent *ptemp = pCurrentStudent;
			pCurrentStudent = pCurrentStudent->PNextStudent;
			free(ptemp);
		}
	}
	gpFirstStudent = NULL;
}

void _index_student(int index){
	if(!gpFirstStudent){
		Dprintf("\nList is empty");
		return;
	}else{
		struct SStudent *pCurrentStudent = gpFirstStudent;
		int count = 0;
		while(count < index && pCurrentStudent){
			pCurrentStudent = pCurrentStudent->PNextStudent;
			count++;
		}
		if(!pCurrentStudent){ Dprintf("\n List length is only %d",count);}
		else
		{
			Dprintf("\n The node at index %d", count);
			Dprintf("\n \t ID : %d", pCurrentStudent->student.ID);
			Dprintf("\n \t Name : %s", pCurrentStudent->student.name);
			Dprintf("\n \t height : %f", pCurrentStudent->student.height);
		}

	}
}

void display_student(){
	char temp_text[10];
	Dprintf("\n Enter index: ");
	gets(temp_text);
	int index = atoi(temp_text);
	if(index < 0) {
		Dprintf("\n You can not enter a negative number");
		return;
	}
	_index_student(index);
}

int _get_length(struct SStudent *pCurrentStudent){
	if(!pCurrentStudent) return 0;
	return 1 + _get_length(pCurrentStudent->PNextStudent);
}

void list_length(){
	if(!gpFirstStudent){
		Dprintf("\n List length = %d",0);
	}
	else{
		Dprintf("\n List length = %d",_get_length(gpFirstStudent));
	}
}

void display_student_backword(){
	char temp_text[10];
	Dprintf("\n Enter index from the end of the list: ");
	gets(temp_text);
	int index_in = atoi(temp_text);
	int length = _get_length(gpFirstStudent);
	int index = length - index_in - 1;
	if(index < 0) {
		Dprintf("\n list is only %d elements",length);
		return;
	}
	_index_student(index);
}

void display_mid(){
	if(!gpFirstStudent){
		Dprintf("\n List is empty");
		return;
	}
	struct SStudent *pFast = gpFirstStudent;
	struct SStudent *pSlow = gpFirstStudent;
	while(pFast->PNextStudent){
		pFast = pFast->PNextStudent->PNextStudent;
		pSlow = pSlow->PNextStudent;
		if(!pFast) break;
	}
	Dprintf("\n The middle Student is: -\n");
	Dprintf("\n \t ID : %d", pSlow->student.ID);
	Dprintf("\n \t Name : %s", pSlow->student.name);
	Dprintf("\n \t height : %f", pSlow->student.height);
}

void is_loop(){
	if(!gpFirstStudent){
		Dprintf("\n List length is empty");
		return;
	}
	struct SStudent *pFast = gpFirstStudent;
	struct SStudent *pSlow = gpFirstStudent;
	while(pFast->PNextStudent){
		pFast = pFast->PNextStudent->PNextStudent;
		pSlow = pSlow->PNextStudent;
		if(pFast == pSlow){
			Dprintf("\n Found loop\n");
			return;
		}
		if(!pFast) break;
	}
	Dprintf("\n No loop is found\n");
}

void reverse(){
	if(!gpFirstStudent){
		Dprintf("\n List is empty");
		return;
	}
	struct SStudent *pCurrent = gpFirstStudent;
	struct SStudent *pNext = gpFirstStudent->PNextStudent;
	struct SStudent *pPrev = NULL;
	while (pNext){
		pNext = pCurrent->PNextStudent;
		pCurrent->PNextStudent = pPrev;
		pPrev = pCurrent;
		pCurrent = pNext;
	}
	gpFirstStudent = pPrev;
	Dprintf("\n List is reversed");
}



















