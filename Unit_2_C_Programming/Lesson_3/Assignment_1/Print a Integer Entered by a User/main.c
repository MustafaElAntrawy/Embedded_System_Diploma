/*
 * main.c
 *
 *  Created on: Oct 22, 2021
 *      Author: moustafa
 */
#include <stdio.h>

int main(){
	int input = 0;
	printf("Enter a integer: ");
	fflush(stdout);
	scanf("%d", &input);
	printf("You entered: %d", input);
	return 0;
}


