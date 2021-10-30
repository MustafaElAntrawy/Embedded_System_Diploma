#include <stdio.h>

#define PI 3.14
#define AREA(r) (PI * (r) *(r))
int main()
{
int radius;
float area;
printf("Enter the radius: ");
fflush(stdin); fflush(stdout);
scanf("%d", &radius);
area = AREA(radius);
printf("Area = %.2f", area);

return 0;
}
