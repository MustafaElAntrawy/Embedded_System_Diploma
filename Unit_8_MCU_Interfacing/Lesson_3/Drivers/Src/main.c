#include "stm32f103x6.h"
#include "Stm32F103C6_GPIO_Driver.h"
#include "Stm32F103C6_EXTI_Driver.h"
#include "Stm32F103C6_USART_Driver.h"
#include "lcd.h"
#include "keypad.h"



void clock_init()
{
	//Clock Enable
	RCC_GPIOA_CLK_EN();
	RCC_GPIOB_CLK_EN();
	RCC_AFIO_CLK_EN();
}
unsigned char ch;
void UART_IRQ_Callback(void)
{
	MCAL_UART_ReceiveData(USART1, &ch, disable);
	MCAL_UART_SendData(USART1, &ch, enable);
}



int main()
{ 
	UART_Config uartCFG;
	clock_init();

	uartCFG.BoudRate = UART_BaudRate_115200;
	uartCFG.HwFlowCtl = UART_HwFlowCtl_NONE;
	uartCFG.IRQ_Enable = UART_IRQ_Enable_RXNEIE;
	uartCFG.P_IRQ_CallBack = UART_IRQ_Callback;
	uartCFG.Parity = UART_Parity__NONE;
	uartCFG.Payload_length = UART_Payload_Length_8B;
	uartCFG.StopBits = UART_StopBits__1;
	uartCFG.USART_Mode = UART_Mode_Tx_Rx;

	MCAL_UART_Init(USART1, &uartCFG);
	MCAL_UART_GPIO_Set_Pins(USART1);

	while (1){

	}

	return 0;
}


