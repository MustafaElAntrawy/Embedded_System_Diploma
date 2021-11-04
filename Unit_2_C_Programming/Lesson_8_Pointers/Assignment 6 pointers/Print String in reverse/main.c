#include <stdio.h>
#include<string.h>

int main(){
	char st[30];
	printf("Input a string: ");
	fflush(stdin); fflush(stdout);
	gets(st);

	char *ptr = st + strlen(st) - 1;

	printf("Reverse of the String is: ");
	int i;
	for (i = 0; i < strlen(st); ++i){
		printf("%c",*ptr);
		ptr--;
	}


	return 0;
}
