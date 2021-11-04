#include <stdio.h>

int main(){
	char alph[27];
	char* ptr = alph;
	int i;
	for (i = 0; i < 26; ++i){
		*ptr = 'A' + i;
		ptr++;
	}
	ptr = alph;
	printf("The Alphabets are: \n");
	for (i = 0; i < 26; ++i){
		printf("%c  ",*ptr++);
	}

	return 0;
}
