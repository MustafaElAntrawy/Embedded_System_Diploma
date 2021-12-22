#include "stm32f103x6.h"
#include "Stm32F103C6_GPIO_Driver.h"


void clock_init(){
	// Enable clock GPIOA
	RCC_GPIOA_CLK_EN();
	// Enable clock GPIOB
	RCC_GPIOB_CLK_EN();
}
void GPIO_init(){

	GPIO_PinConfig_t PinConfig;

	//PA1 input Highz
	PinConfig.GPIO_PinNumber = GPIO_PIN_1;
	PinConfig.GPIO_MODE = GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(GPIOA, &PinConfig);

	//PB1 output push pull mode
	PinConfig.GPIO_PinNumber = GPIO_PIN_1;
	PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinConfig.GPIO_Output_Speed = GPIO_SPEED_10M;
	MCAL_GPIO_Init(GPIOB, &PinConfig);

	//PA13 input Highz
	PinConfig.GPIO_PinNumber = GPIO_PIN_13;
	PinConfig.GPIO_MODE = GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(GPIOA, &PinConfig);

	//PB13 output push pull mode
	PinConfig.GPIO_PinNumber = GPIO_PIN_13;
	PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinConfig.GPIO_Output_Speed = GPIO_SPEED_10M;
	MCAL_GPIO_Init(GPIOB, &PinConfig);

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
		if (MCAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == 0) //Press
		{
			MCAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
			while (MCAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == 0); // Single press
		}
		// PA13 >> Connected external PUR
		if (MCAL_GPIO_ReadPin(GPIOA, GPIO_PIN_13) == 1) //Press Multi pressing
		{
			MCAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
		}
		my_wait(1);

	}

	return 0;
}


