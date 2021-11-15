#include <stdint.h>

#define SYSCTL_RCGC2_R (*((volatile unsigned long*)0x400FE000+0x108))
#define GPIO_PORTF_DIR_R (*((volatile unsigned long*)0x40025000+0x400))
#define GPIO_PORTF_DEN_R (*((volatile unsigned long*)0x40025000+0x51C))
#define GPIO_PORTF_DATA_R (*((volatile unsigned long*)0x40025000+0x3FC))

int main(){
	volatile unsigned long delay_count;
	SYSCTL_RCGC2_R = 0x00000020;
	//delay to make sure GPIO is up and running
	for(delay_count =0; delay_count < 200; delay_count++);
	GPIO_PORTF_DIR_R |= 0x00000008; //DIR is output for pin 3 port f
	GPIO_PORTF_DEN_R |= 0x00000008;
	while(1){
		GPIO_PORTF_DATA_R |= 0x00000008;
		for(delay_count =0; delay_count < 2000; delay_count++);
		GPIO_PORTF_DATA_R &= ~(0x00000008);
		for(delay_count =0; delay_count < 2000; delay_count++);
		
	}
	
	return 0;
}