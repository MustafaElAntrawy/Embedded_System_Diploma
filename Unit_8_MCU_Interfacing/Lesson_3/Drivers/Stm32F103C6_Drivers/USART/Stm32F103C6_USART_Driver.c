 /*
 * Stm32F103C6_USART_Driver.c
 *
 *  Created on: Dec 20, 2021
 *      Author: Antrawy
 */
#include "Stm32F103C6_USART_Driver.h"


//=================================================================
//						Generic Variables
//=================================================================
UART_Config* Global_UART_Config[3] = {NULL, NULL, NULL};

//=================================================================
//						Generic Functions
//=================================================================



//=================================================================
//							APIS
//=================================================================

/**================================================================
* @Fn				-MCAL_UART_Init
* @brief			-Initializes UART (Supported feature ASYNCH. Only)
* @param [in] 		-USARTx: where x can be (1..3 depending on device used)
* @param [in] 		-UART_Config: All UART Configuration EXTI_PinConfig_t
* @retval 			-None
* Note				-Support for Now Asynch. mode & clock 8 MHZ
*/


void MCAL_UART_Init (USART_TypeDef* USARTx, UART_Config* UART_Config)
{

	uint32_t pclk, BRR;

	// enable the clock for given USART peripheral
	if		(USARTx == USART1) 		{	RCC_UASRT1_CLK_EN(); 	Global_UART_Config[0] = UART_Config;	}
	else if	(USARTx == USART2) 		{	RCC_UASRT2_CLK_EN();	Global_UART_Config[1] = UART_Config;	}
	else if	(USARTx == USART3) 		{	RCC_UASRT3_CLK_EN();	Global_UART_Config[2] = UART_Config;	}

	//Enable USART Module
	//Bit 13 UE: USART enable
	USARTx->CR1 |= 1<<13;

	//Enable USART TX & RX according to the UART_Config mode
	//USARTx CR1 Bit 3 TE: Transmitter enable & Bit 3 TE: Transmitter enable
	USARTx->CR1 |= UART_Config->USART_Mode;

	//Payload Length
	//USARTx CR1 Bit 12 M: Word length
	USARTx->CR1 |= UART_Config->Payload_length;

	//Parity
	//USARTx CR1 Bit 12 M: Word length & Bit 9 PS: Parity selection
	USARTx->CR1 |= UART_Config->Parity;

	//STOP bits
	//USARTx CR2 Bits 13:12 STOP: STOP bits
	USARTx->CR2 |= UART_Config->StopBits;

	//USART hardware Flow Control
	//USARTx CR3 Bit 9 CTSE: CTS enable & Bit 8 RTSE: RTS enable
	USARTx->CR3 |= UART_Config->HwFlowCtl;

	//Configuration of BRR(BaudRate register)
	//PCLK1 for USART2,3
	//PCLK2 for USART1
	if (USARTx == USART1)
	{
		pclk = MCAL_RCC_GetPCLK2Freq();
	}
	else
	{
		pclk = MCAL_RCC_GetPCLK1Freq();
	}

	BRR = UART_BRR_Register(pclk, UART_Config->BoudRate);
	USARTx->BRR = BRR;

	//Enable / Disable Interrupt
	//USART_CR1
	if (UART_Config->IRQ_Enable != UART_IRQ_Enable_NONE)
	{
		USARTx->CR1 |= (UART_Config->IRQ_Enable);

		//Enable NVIC for USARTx IRQ
		if 		(USARTx == USART1) 		NVIC_IRQ37_USART1_Enable;
		else if (USARTx == USART2)		NVIC_IRQ38_USART2_Enable;
		else if (USARTx == USART3) 		NVIC_IRQ39_USART3_Enable;
	}
}

/**================================================================
* @Fn				-MCAL_UART_DeInit
* @brief			-DEInitializes UART (Supported feature ASYNCH. Only)
* @param [in] 		-USARTx: where x can be (1..3 depending on device used)
* @retval 			-None
* Note				-Reset the Model by RCC
*/
void MCAL_UART_DeInit (USART_TypeDef* USARTx)
{
	if (USARTx == USART1)
	{
		RCC_UASRT1_Reset();
		NVIC_IRQ37_USART1_Disable;
	}
	else if (USARTx == USART2)
	{
		RCC_UASRT2_Reset();
		NVIC_IRQ38_USART2_Disable;
	}
	else if (USARTx == USART3)
	{
		RCC_UASRT3_Reset();
		NVIC_IRQ39_USART3_Disable;
	}
}

/**================================================================
* @Fn				-MCAL_UART_SendData
* @brief			-Send Buffer on UART
* @param [in] 		-USARTx: where x can be (1..3 depending on device used)
* @param [in] 		-pTxBuffer Buffer
* @param [in] 		-PollingEn Enable/Disable polling
* @retval 			-None
* Note				-Should initialize UART first
* 					When transmitting with the parity enabled (PCE bit set to 1 in the USART_CR1 register),
* 					the value written in the MSB (bit 7 or bit 8 depending on the data length) has no effect
* 					because it is replaced by the parity.
* 					When receiving with the parity enabled, the value read in the MSB bit is the received parity bit
*/
void MCAL_UART_SendData (USART_TypeDef* USARTx, uint16_t* pTxBuffer, enum Polling_mechanism PollingEn)
{
	UART_Config* UART_Config = NULL;
	if		(USARTx == USART1) 		UART_Config = Global_UART_Config[0];
	else if	(USARTx == USART2) 		UART_Config = Global_UART_Config[1];
	else if	(USARTx == USART3) 		UART_Config = Global_UART_Config[2];

	// Wait until TXE flag is set in the SR
	if(PollingEn == enable)	while ( !(USARTx->SR & 1<<7) );

	// Check the USART_WordLength item for 9BIT or 8BIT in a frame
	if(UART_Config->Payload_length == UART_Payload_Length_9B)
	{
		USARTx->DR = (*pTxBuffer & (uint16_t)0x01FF);
	}else
	{
		USARTx->DR = (*pTxBuffer & (uint16_t)0xFF);
	}
}

/**================================================================
* @Fn				-MCAL_UART_WAIT_TC
* @brief			-Wait till all data is sent
* @param [in] 		-USARTx: where x can be (1..3 depending on device used)
* @retval 			-None
* Note				-None
*/
void MCAL_UART_WAIT_TC (USART_TypeDef* USARTx)
{
	//Wait till TC flag is set in the SR
	while ( !(USARTx->SR & 1<<6) );
}

/**================================================================
* @Fn				-MCAL_UART_ReceiveData
* @brief			-Receive Buffer on UART
* @param [in] 		-USARTx: where x can be (1..3 depending on device used)
* @param [in] 		-pTxBuffer Buffer
* @param [in] 		-PollingEn Enable/Disable polling
* @retval 			-None
* Note				-None
*/
void MCAL_UART_ReceiveData (USART_TypeDef* USARTx, uint16_t* pTxBuffer, enum Polling_mechanism PollingEn)
{
	UART_Config* UART_Config = NULL;
	if		(USARTx == USART1) 		UART_Config = Global_UART_Config[0];
	else if	(USARTx == USART2) 		UART_Config = Global_UART_Config[1];
	else if	(USARTx == USART3) 		UART_Config = Global_UART_Config[2];
	//Wait until RXNE flag is set in the SR
	if(PollingEn == enable)	while ( !(USARTx->SR & 1<<5) );

	// Check the USART_WordLength item for 9BIT or 8BIT in a frame
	if(UART_Config->Payload_length == UART_Payload_Length_9B)
	{
		if(UART_Config->Parity == UART_Parity__NONE)
		{
			//No parity -> All 9BITs are considered data
			*((uint16_t*) pTxBuffer) = USARTx->DR;
		}else
		{
			//Parity is used, so 8BITs will be of user data and 1 bit is for parity
			*((uint16_t*) pTxBuffer) = (USARTx->DR & (uint8_t)0xFF);
		}
	}else
	{
		if(UART_Config->Parity == UART_Parity__NONE)
		{
			//No parity -> All 8BITs are considered data
			*((uint16_t*) pTxBuffer) = (USARTx->DR & (uint8_t)0xFF);
		}else
		{
			//Parity is used, so 7BITs will be of user data and 1 bit is for parity
			*((uint16_t*) pTxBuffer) = (USARTx->DR & (uint8_t)0x7F);
		}
	}

}

/**================================================================
* @Fn				-MCAL_UART_GPIO_Set_Pins
* @brief			-Initialize GPIO Pins
* @param [in] 		-USARTx: where x can be (1..3 depending on device used)
* @retval 			-None
* Note				-Should enable the corresponding ALT & GPIO in RCC clock Also called after MCAL_UART_Init()
*/
void MCAL_UART_GPIO_Set_Pins (USART_TypeDef* USARTx)
{
	GPIO_PinConfig_t PinConfig;
	if (USARTx == USART1)
	{
		//PA9 TX
		PinConfig.GPIO_PinNumber = GPIO_PIN_9;
		PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
		PinConfig.GPIO_Output_Speed = GPIO_SPEED_10M;
		MCAL_GPIO_Init(GPIOA, &PinConfig);

		//PA10 Rx
		PinConfig.GPIO_PinNumber = GPIO_PIN_10;
		PinConfig.GPIO_MODE = GPIO_MODE_AF_INPUT;
		MCAL_GPIO_Init(GPIOA, &PinConfig);

		//PA11 CTS
		if (Global_UART_Config[0]->HwFlowCtl == UART_HwFlowCtl_CTS || Global_UART_Config[0]->HwFlowCtl == UART_HwFlowCtl_RTS_CTS)
		{
			PinConfig.GPIO_PinNumber = GPIO_PIN_11;
			PinConfig.GPIO_MODE = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA, &PinConfig);
		}
		//PA12 RTS
		if (Global_UART_Config[0]->HwFlowCtl == UART_HwFlowCtl_RTS || Global_UART_Config[0]->HwFlowCtl == UART_HwFlowCtl_RTS_CTS)
		{
			PinConfig.GPIO_PinNumber = GPIO_PIN_12;
			PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			PinConfig.GPIO_Output_Speed = GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOA, &PinConfig);
		}
	}

	else if (USARTx == USART2)
	{
		//PA2 TX
		PinConfig.GPIO_PinNumber = GPIO_PIN_2;
		PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
		PinConfig.GPIO_Output_Speed = GPIO_SPEED_10M;
		MCAL_GPIO_Init(GPIOA, &PinConfig);

		//PA3 Rx
		PinConfig.GPIO_PinNumber = GPIO_PIN_3;
		PinConfig.GPIO_MODE = GPIO_MODE_AF_INPUT;
		MCAL_GPIO_Init(GPIOA, &PinConfig);

		//PA0 CTS
		if (Global_UART_Config[1]->HwFlowCtl == UART_HwFlowCtl_CTS || Global_UART_Config[1]->HwFlowCtl == UART_HwFlowCtl_RTS_CTS)
		{
			PinConfig.GPIO_PinNumber = GPIO_PIN_0;
			PinConfig.GPIO_MODE = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA, &PinConfig);
		}
		//PA1 RTS
		if (Global_UART_Config[1]->HwFlowCtl == UART_HwFlowCtl_RTS || Global_UART_Config[1]->HwFlowCtl == UART_HwFlowCtl_RTS_CTS)
		{
			PinConfig.GPIO_PinNumber = GPIO_PIN_1;
			PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			PinConfig.GPIO_Output_Speed = GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOA, &PinConfig);
		}
	}

	else if (USARTx == USART3)
	{
		//PB10 TX
		PinConfig.GPIO_PinNumber = GPIO_PIN_10;
		PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
		PinConfig.GPIO_Output_Speed = GPIO_SPEED_10M;
		MCAL_GPIO_Init(GPIOB, &PinConfig);

		//PB11 Rx
		PinConfig.GPIO_PinNumber = GPIO_PIN_11;
		PinConfig.GPIO_MODE = GPIO_MODE_AF_INPUT;
		MCAL_GPIO_Init(GPIOB, &PinConfig);

		//PB13 CTS
		if (Global_UART_Config[2]->HwFlowCtl == UART_HwFlowCtl_CTS || Global_UART_Config[2]->HwFlowCtl == UART_HwFlowCtl_RTS_CTS)
		{
			PinConfig.GPIO_PinNumber = GPIO_PIN_13;
			PinConfig.GPIO_MODE = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOB, &PinConfig);
		}
		//PB14 RTS
		if (Global_UART_Config[2]->HwFlowCtl == UART_HwFlowCtl_RTS || Global_UART_Config[2]->HwFlowCtl == UART_HwFlowCtl_RTS_CTS)
		{
			PinConfig.GPIO_PinNumber = GPIO_PIN_14;
			PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			PinConfig.GPIO_Output_Speed = GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOB, &PinConfig);
		}
	}

}


//ISR

void USART1_IRQHandler(void)
{
	Global_UART_Config[0]->P_IRQ_CallBack();
}
void USART2_IRQHandler(void)
{
	Global_UART_Config[1]->P_IRQ_CallBack();
}
void USART3_IRQHandler(void)
{
	Global_UART_Config[2]->P_IRQ_CallBack();
}
