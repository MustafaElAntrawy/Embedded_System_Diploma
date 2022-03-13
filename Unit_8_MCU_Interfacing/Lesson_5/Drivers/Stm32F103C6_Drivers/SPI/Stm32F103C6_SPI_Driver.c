/*
 * Stm32F103C6_SPI_Driver.c
 *
 *  Created on: Mar 11, 2022
 *      Author: moustafa
 */
#include "Stm32F103C6_SPI_Driver.h"

//=================================================================
//						Generic Variables
//=================================================================
SPI_Config* Global_SPI_Config[2] = {NULL, NULL};

//=================================================================
//						Generic Macros
//=================================================================
#define SPI1_INDEX			0
#define SPI2_INDEX			1

#define SPI_SR_TXE				((uint8_t)0x02)		//Transmit buffer empty
#define SPI_SR_RXNE				((uint8_t)0x01)		//Receive buffer Not empty

//=================================================================
//						Generic Functions
//=================================================================



//=================================================================
//							APIS
//=================================================================

/**================================================================
* @Fn				-MCAL_SPI_Init
* @brief			-Initializes SPI according to the specified parameters
* @param [in] 		-SPIx: where x can be (1..2 depending on device used)
* @param [in] 		-SPI_Config: All SPI Configuration EXTI_PinConfig_t
* @retval 			-None
* Note				-Support for SPI Full Duplex Master/Slave only & NSS HW/SW
* 					-in case of master you have to configure pin and drive it
*/
void MCAL_SPI_Init (SPI_TypeDef *SPIx, SPI_Config* SPI_Config)
{
	//Safety for register
	uint16_t tmpreg_CR1 = 0;
	uint16_t tmpreg_CR2 = 0;

	if		(SPIx == SPI1) 		{	RCC_SPI1_CLK_EN(); 	Global_SPI_Config[SPI1_INDEX] = SPI_Config;	}
	else if	(SPIx == SPI2) 		{	RCC_SPI2_CLK_EN();	Global_SPI_Config[SPI2_INDEX] = SPI_Config;	}

	//Enable SPI CR1 Bit 6 SPE: SPI enable
	tmpreg_CR1 = (0x1U<<6);

	//Master or Slave
	tmpreg_CR1 |= SPI_Config->Device_Mode;

	//SPI_Communication_Mode
	tmpreg_CR1 |= SPI_Config->Communication_Mode;

	//SPI_Frame_Format
	tmpreg_CR1 |= SPI_Config->Frame_Format;

	//SPI_DataSize
	tmpreg_CR1 |= SPI_Config->DataSize;

	//SPI_CLKPolarity
	tmpreg_CR1 |= SPI_Config->CLKPolarity;

	//SPI_CLKPhase
	tmpreg_CR1 |= SPI_Config->CLKPhase;

	//SPI_NSS
	if(SPI_Config->NSS == SPI_NSS_Hard_Master_SS_output_enable)
	{
		tmpreg_CR2 |= SPI_Config->NSS;
	}
	else if (SPI_Config->NSS == SPI_NSS_Hard_Master_SS_output_disable)
	{
		tmpreg_CR2 &= SPI_Config->NSS;
	}
	else
	{
		tmpreg_CR1 |= SPI_Config->NSS;
	}

	//SPI_BAUDRATEPRESCALER
	tmpreg_CR1 |= SPI_Config->SPI_BAUDRATEPRESCALER;

	//SPI_IRQ_
	if (SPI_Config->IRQ_Enable != SPI_IRQ_Enable_NONE)
	{
		tmpreg_CR2 |= SPI_Config->IRQ_Enable;
		if		(SPIx == SPI1) 		NVIC_IRQ35_SPI1_Enable;
		else if	(SPIx == SPI2) 		NVIC_IRQ36_SPI2_Enable;
	}

	SPIx->CR1 = tmpreg_CR1;
	SPIx->CR2 = tmpreg_CR2;
}

/**================================================================
* @Fn				-MCAL_SPI_DeInit
* @brief			-DEInitializes SPI according to the specified parameters
* @param [in] 		-SPIx: where x can be (1..2 depending on device used)
* @retval 			-None
* Note				-Reset the Model by RCC
*/
void MCAL_SPI_DeInit (SPI_TypeDef *SPIx)
{
	if (SPIx == SPI1)
	{
		RCC_SPI1_Reset();
		NVIC_IRQ35_SPI1_Disable;
	}
	else if (SPIx == SPI2)
	{
		RCC_SPI2_Reset();
		NVIC_IRQ36_SPI2_Disable;
	}
}

/**================================================================
* @Fn				-MCAL_SPI_GPIO_Set_Pins
* @brief			-Initialize GPIO Pins
* @param [in] 		-SPIx: where x can be (1..2 depending on device used)
* @retval 			-None
* Note				-Should enable the corresponding ALT & GPIO in RCC clock Also called after MCAL_SPI_Init()
*/
void MCAL_SPI_GPIO_Set_Pins (SPI_TypeDef *SPIx)
{
	GPIO_PinConfig_t PinConfig;
	if(SPIx == SPI1)
	{
		if(Global_SPI_Config[SPI1_INDEX]->Device_Mode == SPI_Device_Mode_MASTER)
		{
			//PA4 : SPI1_NSS
			switch(Global_SPI_Config[SPI1_INDEX]->NSS)
			{
			case SPI_NSS_Hard_Master_SS_output_disable:
				//Hardware master / slave => input floating
				PinConfig.GPIO_PinNumber = GPIO_PIN_4;
				PinConfig.GPIO_MODE = GPIO_MODE_INPUT_FLO;
				MCAL_GPIO_Init(GPIOA, &PinConfig);
				break;
			case SPI_NSS_Hard_Master_SS_output_enable:
				PinConfig.GPIO_PinNumber = GPIO_PIN_4;
				PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
				PinConfig.GPIO_Output_Speed = GPIO_SPEED_10M;
				MCAL_GPIO_Init(GPIOA, &PinConfig);
				break;
			}

			//PA5 : SPI1 SCK
			// Master => Alternate function push-pull
			PinConfig.GPIO_PinNumber = GPIO_PIN_5;
			PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			PinConfig.GPIO_Output_Speed = GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOA, &PinConfig);

			//PA6 : SPI1 MISO
			// Full duplex / master => input floating
			PinConfig.GPIO_PinNumber = GPIO_PIN_6;
			PinConfig.GPIO_MODE = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA, &PinConfig);

			//PA7 : SPI1 MOSI
			// Full duplex / Master => Alternate function push-pull
			PinConfig.GPIO_PinNumber = GPIO_PIN_7;
			PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			PinConfig.GPIO_Output_Speed = GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOA, &PinConfig);
		}
		else	//Slave
		{
			if (Global_SPI_Config[SPI1_INDEX]->NSS == SPI_NSS_Hard_Slave)
			{
				//Hardware master / slave => input floating
				PinConfig.GPIO_PinNumber = GPIO_PIN_4;
				PinConfig.GPIO_MODE = GPIO_MODE_INPUT_FLO;
				MCAL_GPIO_Init(GPIOA, &PinConfig);
			}
			//PA5 : SPI1 SCK
			// Slave => Input floating
			PinConfig.GPIO_PinNumber = GPIO_PIN_5;
			PinConfig.GPIO_MODE = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA, &PinConfig);

			//PA6 : SPI1 MISO
			// Full duplex / slave => Alternate function push-pull
			PinConfig.GPIO_PinNumber = GPIO_PIN_6;
			PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			PinConfig.GPIO_Output_Speed = GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOA, &PinConfig);

			//PA7 : SPI1 MOSI
			// Full duplex / Slave => Input floating
			PinConfig.GPIO_PinNumber = GPIO_PIN_7;
			PinConfig.GPIO_MODE = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA, &PinConfig);
		}
	}
	else if(SPIx == SPI2)
	{
		if(Global_SPI_Config[SPI2_INDEX]->Device_Mode == SPI_Device_Mode_MASTER)
		{
			//PB12 : SPI1_NSS
			switch(Global_SPI_Config[SPI2_INDEX]->NSS)
			{
			case SPI_NSS_Hard_Master_SS_output_disable:
				//Hardware master / slave => input floating
				PinConfig.GPIO_PinNumber = GPIO_PIN_12;
				PinConfig.GPIO_MODE = GPIO_MODE_INPUT_FLO;
				MCAL_GPIO_Init(GPIOB, &PinConfig);
				break;
			case SPI_NSS_Hard_Master_SS_output_enable:
				PinConfig.GPIO_PinNumber = GPIO_PIN_12;
				PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
				PinConfig.GPIO_Output_Speed = GPIO_SPEED_10M;
				MCAL_GPIO_Init(GPIOB, &PinConfig);
				break;
			}

			//PB13 : SPI1 SCK
			// Master => Alternate function push-pull
			PinConfig.GPIO_PinNumber = GPIO_PIN_13;
			PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			PinConfig.GPIO_Output_Speed = GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOB, &PinConfig);

			//PB14 : SPI1 MISO
			// Full duplex / master => input floating
			PinConfig.GPIO_PinNumber = GPIO_PIN_14;
			PinConfig.GPIO_MODE = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOB, &PinConfig);

			//PB15 : SPI1 MOSI
			// Full duplex / Master => Alternate function push-pull
			PinConfig.GPIO_PinNumber = GPIO_PIN_15;
			PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			PinConfig.GPIO_Output_Speed = GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOB, &PinConfig);
		}
		else	//Slave
		{
			if (Global_SPI_Config[SPI2_INDEX]->NSS == SPI_NSS_Hard_Slave)
			{
				//Hardware master / slave => input floating
				PinConfig.GPIO_PinNumber = GPIO_PIN_12;
				PinConfig.GPIO_MODE = GPIO_MODE_INPUT_FLO;
				MCAL_GPIO_Init(GPIOB, &PinConfig);
			}
			//PB13 : SPI1 SCK
			// Slave => Input floating
			PinConfig.GPIO_PinNumber = GPIO_PIN_13;
			PinConfig.GPIO_MODE = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOB, &PinConfig);

			//PB14 : SPI1 MISO
			// Full duplex / slave => Alternate function push-pull
			PinConfig.GPIO_PinNumber = GPIO_PIN_14;
			PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			PinConfig.GPIO_Output_Speed = GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOB, &PinConfig);

			//PB15 : SPI1 MOSI
			// Full duplex / Slave => Input floating
			PinConfig.GPIO_PinNumber = GPIO_PIN_15;
			PinConfig.GPIO_MODE = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOB, &PinConfig);
		}
	}
}

/**================================================================
* @Fn				-MCAL_SPI_SendData
* @brief			-Send Buffer on SPI
* @param [in] 		-SPIx: where x can be (1..2 depending on device used)
* @param [in] 		-pTxBuffer Buffer
* @param [in] 		-PollingEn Enable/Disable polling
* @retval 			-None
* Note				-Should initialize SPI first
*/
void MCAL_SPI_SendData (SPI_TypeDef* SPIx, uint16_t* pTxBuffer, enum PollingMechanism PollingEn)
{
//TO do receive only

	if (PollingEn == Pollingenable)		while(!((SPIx->SR) & SPI_SR_TXE));

	SPIx->DR = *pTxBuffer;

}

/**================================================================
* @Fn				-MCAL_SPI_ReceiveData
* @brief			-Receive Buffer on SPI
* @param [in] 		-SPIx: where x can be (1..2 depending on device used)
* @param [in] 		-pTxBuffer Buffer
* @param [in] 		-PollingEn Enable/Disable polling
* @retval 			-None
* Note				-None
*/
void MCAL_SPI_ReceiveData (SPI_TypeDef* SPIx, uint16_t* pTxBuffer, enum PollingMechanism PollingEn)
{
	if (PollingEn == Pollingenable)		while(!((SPIx->SR) & SPI_SR_RXNE));
	*pTxBuffer = SPIx->DR;

}

/**================================================================
* @Fn				-MCAL_SPI_TX_RX
* @brief			-Send & Receive Buffer on SPI
* @param [in] 		-SPIx: where x can be (1..2 depending on device used)
* @param [in] 		-pTxBuffer Buffer
* @param [in] 		-PollingEn Enable/Disable polling
* @retval 			-None
* Note				-None
*/
void MCAL_SPI_TX_RX (SPI_TypeDef* SPIx, uint16_t* pTxBuffer, enum PollingMechanism PollingEn)
{
	if (PollingEn == Pollingenable)		while(!((SPIx->SR) & SPI_SR_TXE));
	SPIx->DR = *pTxBuffer;

	if (PollingEn == Pollingenable)		while(!((SPIx->SR) & SPI_SR_RXNE));
	*pTxBuffer = SPIx->DR;
}


//IRQ
void SPI1_IRQHandler(void)
{
	struct S_IRQ_SRC irq_src;
	irq_src.TXE = 	( (SPI1->SR & (1<<1)) >> 1);
	irq_src.RXNE = 	( (SPI1->SR & (1<<0)) >> 0);
	irq_src.ERRI = 	( (SPI1->SR & (1<<4)) >> 4);
	Global_SPI_Config[SPI1_INDEX]->P_IRQ_Callback(irq_src);
}

void SPI2_IRQHandler(void)
{
	struct S_IRQ_SRC irq_src;
	irq_src.TXE = 	( (SPI2->SR & (1<<1)) >> 1);
	irq_src.RXNE = 	( (SPI2->SR & (1<<0)) >> 0);
	irq_src.ERRI = 	( (SPI2->SR & (1<<4)) >> 4);
	Global_SPI_Config[SPI2_INDEX]->P_IRQ_Callback(irq_src);
}


