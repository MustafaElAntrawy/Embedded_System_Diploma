#include <stdio.h>

int main(){
	int arr[15];
	int n;
	printf("Input the number of elements to store in the array (max 15): ");
	fflush(stdin); fflush(stdout);
	scanf("%d",&n);
	printf("Input %d number of elements in the array : \n", n);
	int i;
	for (i = 0; i < n; ++i){
		printf("element - %d : ", i + 1);
		fflush(stdin); fflush(stdout);
		scanf("%d",&arr[i]);
	}

	int* ptr = arr + n - 1;

	printf("The elements of array in reverse order are : \n");
	for (i = n - 1; i >= 0; --i){
		printf("element - %d : %d\n", i + 1, *ptr);
		ptr--;
	}



	return 0;
}
