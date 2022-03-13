/*
 * main
 *
 *  Created on: Mar 13, 2022
 *      Author: moustafa
 */

//#define MCU_Act_As_Master
#define MCU_Act_As_Slave

#include "stm32f103x6.h"
#include "Stm32F103C6_GPIO_Driver.h"
#include "Stm32F103C6_EXTI_Driver.h"
#include "Stm32F103C6_USART_Driver.h"
#include "Stm32F103C6_SPI_Driver.h"


unsigned char ch;

void clock_init()
{
	//Clock Enable
	RCC_GPIOA_CLK_EN();
	RCC_GPIOB_CLK_EN();
	RCC_AFIO_CLK_EN();
}

void SPI1_IRQ_Callback(struct S_IRQ_SRC irq_src)
{
#ifdef MCU_Act_As_Slave
	if (irq_src.RXNE)
	{
		ch = 0xf;
		MCAL_SPI_TX_RX(SPI1, &ch, Pollingenable);
		MCAL_UART_SendData(USART1, &ch, enable);
	}
#endif
}

void UART_IRQ_Callback(void)
{
#ifdef MCU_Act_As_Master
	MCAL_UART_ReceiveData(USART1, &ch, disable);
	MCAL_UART_SendData(USART1, &ch, enable);
	//Send to SPI
	MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
	MCAL_SPI_TX_RX(SPI1, &ch, Pollingenable);
	MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
#endif
}



int main()
{ 
	clock_init();

	//================UART INIT=================
	UART_Config uartCFG;

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

	//================SPI INIT==================
	SPI_Config SPI1CFG;
	// Common Configuration
	SPI1CFG.CLKPhase = SPI_CLKPhase_2EDGE_first_data_capture_edge;
	SPI1CFG.CLKPolarity = SPI_CLKPolarity_HIGH_when_idle;
	SPI1CFG.DataSize = SPI_DataSize_8BIT;
	SPI1CFG.Frame_Format = SPI_Frame_Format_MSB_transmitted_first;
	//Assume by default pclk2 = 8MHZ
	SPI1CFG.SPI_BAUDRATEPRESCALER = SPI_BAUDRATEPRESCALER_8;
	SPI1CFG.Communication_Mode = SPI_DIRECTION_2LINES;

#ifdef MCU_Act_As_Master
	SPI1CFG.Device_Mode = SPI_Device_Mode_MASTER;
	SPI1CFG.IRQ_Enable = SPI_IRQ_Enable_NONE;
	SPI1CFG.NSS = SPI_NSS_Soft_NSSInternalSoft_Set;
	SPI1CFG.P_IRQ_Callback = NULL;

	//Configure SS on P4A by GPIO
	GPIO_PinConfig_t PinCfg;
	PinCfg.GPIO_PinNumber = GPIO_PIN_4;
	PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
	MCAL_GPIO_Init(GPIOA, &PinCfg);

	//Force the slave select (High) idle mode
	MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
#endif

#ifdef MCU_Act_As_Slave
	SPI1CFG.Device_Mode = SPI_Device_Mode_SLAVE;
	SPI1CFG.IRQ_Enable = SPI_IRQ_Enable_RXNEIE;
	SPI1CFG.NSS = SPI_NSS_Hard_Slave;
	SPI1CFG.P_IRQ_Callback = SPI1_IRQ_Callback;
#endif

	MCAL_SPI_Init(SPI1, &SPI1CFG);
	MCAL_SPI_GPIO_Set_Pins(SPI1);



	while (1){

	}

	return 0;
}


