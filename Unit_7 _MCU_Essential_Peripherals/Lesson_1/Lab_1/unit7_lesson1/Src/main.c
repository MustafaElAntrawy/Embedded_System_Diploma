#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Register Address
//RCC
#define RCC_BASE			0x40021000
#define RCC_APB2ENR 		*(volatile uint32_t *)(RCC_BASE + 0x18)

// GPIO
#define GPIOA_BASE			0x40010800
#define GPIOA_CRH 			*(volatile uint32_t *)(GPIOA_BASE + 0x04)
#define GPIOA_CRL 			*(volatile uint32_t *)(GPIOA_BASE + 0x00)
#define GPIOA_ODR	 		*(volatile uint32_t *)(GPIOA_BASE + 0x0C)
#define GPIOA_IDR	 		*(volatile uint32_t *)(GPIOA_BASE + 0x08)

#define GPIOB_BASE			0x40010C00
#define GPIOB_CRH 			*(volatile uint32_t *)(GPIOB_BASE + 0x04)
#define GPIOB_CRL 			*(volatile uint32_t *)(GPIOB_BASE + 0x00)
#define GPIOB_ODR	 		*(volatile uint32_t *)(GPIOB_BASE + 0x0C)
#define GPIOB_IDR	 		*(volatile uint32_t *)(GPIOB_BASE + 0x08)



void clock_init(){
	// Enable clock GPIOA
	RCC_APB2ENR |= (1<<2);
	// Enable clock GPIOB
	RCC_APB2ENR |= (1<<3);
}
void GPIO_init(){

	GPIOA_CRL = 0x0;
	GPIOB_CRL = 0x0;

	//PA1 input Highz
	GPIOA_CRL |= 1<<6;

	//PB1 output push pull mode
	GPIOB_CRL &= ~(0b11 << 6);
	GPIOB_CRL |= (0b01 << 4);

	//PA13 input Highz
	GPIOA_CRH &= ~(0b11 << 20);
	GPIOA_CRH |= (0b01 << 22);

	//PB13 output push pull mode
	GPIOB_CRH |= (0b01 << 20);
	GPIOB_CRH &= ~(0b11 << 22);
}
void my_wait(int x){
	unsigned int i,j;
	for (i = 0; i < x; ++i){
		for(j = 0; j < 255 ; ++j);
	}
}

int main()
{
	clock_init();
	GPIO_init();

	while (1){
		// PA1 >> Connected external PUR
		if (((GPIOA_IDR & (1<<1)) >>1) == 0) //Press
		{
			GPIOB_ODR ^= 1<<1;
			while(((GPIOA_IDR & (1<<1)) >>1) == 0); // Single press
		}
		// PA13 >> Connected external PUR
		if (((GPIOA_IDR & (1<<13)) >>13) == 1) //Press Multi pressing
		{
			GPIOB_ODR ^= 1<<13;
		}
		my_wait(1);

	}

	return 0;
}



















