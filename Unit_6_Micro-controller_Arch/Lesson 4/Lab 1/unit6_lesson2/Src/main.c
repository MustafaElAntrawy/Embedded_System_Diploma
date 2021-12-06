#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef volatile unsigned int vint32_t;

// Register Address
//RCC
#define RCC_BASE			0x40021000
#define RCC_APB2ENR 		*(volatile uint32_t *)(RCC_BASE + 0x18)
#define RCC_IOPAEN			(1<<2)

// GPIO
#define GPIOA_BASE			0x40010800
#define GPIOA_CRH 			*(volatile uint32_t *)(GPIOA_BASE + 0x04)
#define GPIOA_CRL 			*(volatile uint32_t *)(GPIOA_BASE + 0x00)
#define GPIO_ODP	 		*(volatile uint32_t *)(GPIOA_BASE + 0x0C)
#define GPIOA13 			(1UL<<13)

// EXTI
#define EXTI_BASE			0x40010400
#define EXTI_IMR 			*(volatile uint32_t *)(EXTI_BASE + 0x00)
#define EXTI_RTSR 			*(volatile uint32_t *)(EXTI_BASE + 0x08)
#define EXTI_PR 			*(volatile uint32_t *)(EXTI_BASE + 0x14)

//AFIO
#define AFIO_BASE			0x40010000
#define AFIO_EXTICR1 		*(volatile uint32_t *)(AFIO_BASE + 0x08)

#define NVIC_EXTIE0 		*(volatile uint32_t *)(0xE000E100)

void clock_init(){
	// Enable clock GPIOA
	RCC_APB2ENR |= RCC_IOPAEN;
	// Alternate Function I/O clock enable
	RCC_APB2ENR |= (1<<0);
}
void GPIO_init(){
	// Pin13 Port_A output
	GPIOA_CRH &= 0xFF0FFFFF;
	GPIOA_CRH |= 0x00200000;
	// Pin0 Port_A input floating
	GPIOA_CRL |= (1<<2);
}

int main()
{
	clock_init();
	GPIO_init();
	// EXIT0 configuration to Port_A0
	AFIO_EXTICR1 = 0;
	// Enable rising edge trigger
	EXTI_RTSR |= (1<<0);
	// Enable EXTI Line 0 (Mask 1)
	EXTI_IMR |= (1<<0);

	// Enable NVCI Irq6 >> EXTI0
	NVIC_EXTIE0 |= (1<<6);

	while (1){

	}


	return 0;
}


void EXTI0_IRQHandler(){
	// IRQ from Pin0 Port_A
	//LED toggle pin 13 Port_A
	GPIO_ODP ^= (1<<13);

	// Clear Pending Request
	EXTI_PR |= (1<<0);
}


















