#include "stm32f103x6.h"
#include "Stm32F103C6_GPIO_Driver.h"
#include "Stm32F103C6_EXTI_Driver.h"
#include "lcd.h"
#include "keypad.h"


unsigned int IRQ_Flag = 0;

void my_wait(int x){
	unsigned int i,j;
	for (i = 0; i < x; ++i){
		for(j = 0; j < 255 ; ++j);
	}
}


void EXTI9_Callback(){
	IRQ_Flag = 1;
	LCD_WRITE_STRING("IRQ EXTI9 is  happened ");
	my_wait(1000);

}
int main()
{
	//Clock Enable
	RCC_GPIOA_CLK_EN();
	RCC_GPIOB_CLK_EN();
	RCC_AFIO_CLK_EN();

	LCD_INIT();
	LCD_clear_screen();

	EXTI_PinCongfig_t EXTI_CFG;
	EXTI_CFG.EXTI_PIN = EXTI9PB9;
	EXTI_CFG.Trigger_Case = EXTI_Trigger_RISING;
	EXTI_CFG.P_IRQ_CallBack = EXTI9_Callback;
	EXTI_CFG.IRQ_EN = EXTI_IRQ_Enable;

	MCAL_EXTI_GPIO_Init(&EXTI_CFG);

	IRQ_Flag = 1;
	while (1){
		if (IRQ_Flag){
			LCD_clear_screen();
			IRQ_Flag = 0;
		}

		my_wait(1000);

	}

	return 0;
}


