/*
 * Stm32F103C6_I2C_Driver.c
 *
 *  Created on: Mar 11, 2022
 *      Author: moustafa
 */
#include "Stm32F103C6_I2C_Driver.h"

//=================================================================
//						Generic Variables
//=================================================================

I2C_InintTypeDef Global_I2C_Config[2] = {NULL, NULL};

//=================================================================
//						Generic Macros
//=================================================================

#define I2C1_INDEX						0
#define I2C2_INDEX						1

//=================================================================
//						Generic Functions
//=================================================================

void I2C_AcknowledgeConfig(I2C_TypeDef* I2Cx, FunctionalState state)
{
	if(state == ENABLE)
	{
		I2Cx->CR1 |= I2C_CR1_ACK;
	}
	else
	{
		I2Cx->CR1 &= ~(I2C_CR1_ACK);
	}
}


FlagStatus I2C_GetFlagStatus(I2C_TypeDef* I2Cx, Status flag)
{
	volatile uint32_t dummyRead;
	FlagStatus bitstatus = RESET;
	uint32_t flag1 = 0, flag2 = 0;
	uint32_t lastevent = 0;

	switch (flag)
	{
	case I2C_FLAG_BUSY:
	{
//		Bit 1 BUSY: Bus busy
//		0: No communication on the bus
//		1: Communication ongoing on the bus
//		– Set by hardware on detection of SDA or SCL low
//		– cleared by hardware on detection of a Stop condition.
//		It indicates a communication in progress on the bus. This information is still updated when
//		the interface is disabled (PE=0).
		if ((I2Cx->SR2) & (I2C_SR2_BUSY))
			bitstatus = SET;
		else
			bitstatus = RESET;
		break;
	}
	case EV5:
	{
		if ((I2Cx->SR1) & (I2C_SR1_SB))
			bitstatus = SET;
		else
			bitstatus = RESET;
		break;
	}
	case EV6:
	{
		if ((I2Cx->SR1) & (I2C_SR1_ADDR))
			bitstatus = SET;
		else
			bitstatus = RESET;
		break;
	}
	case EV7:
	{
		if ((I2Cx->SR1) & (I2C_SR1_RXNE))
			bitstatus = SET;
		else
			bitstatus = RESET;
		break;
	}
	case EV8_1:
	case EV8:
	{
		if ((I2Cx->SR1) & (I2C_SR1_TXE))
			bitstatus = SET;
		else
			bitstatus = RESET;
		break;
	}
	case MASTER_BYTE_TRANSMITTING:
	{
		flag1 = I2Cx->SR1;
		flag2 = I2Cx->SR2;
		flag2 = flag2 << 16;
		lastevent = (flag1 | flag2) & ((uint32_t) 0x00FFFFFF);
		if ((lastevent & flag) == flag)
			bitstatus = SET;
		else
			bitstatus = RESET;
		dummyRead = I2Cx->SR2;
		break;
	}
	}

	return bitstatus;
}
void I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState, Repeated_Start start)
{
	if(start != repeated_start)
	{
		//Check if the BUS is idle
		while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	}

	if(NewState != DISABLE)
	{
		//Generate Start Condition
		I2Cx->CR1 |= I2C_CR1_START;
	}
	else
	{
		// Disable Start condition generation
		I2Cx->CR1 &= ~(I2C_CR1_START);
	}
}

void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
	if(NewState != DISABLE)
	{
		//Generate Stop Condition
		I2Cx->CR1 |= I2C_CR1_STOP;
	}
	else
	{
		// Disable Stop condition generation
		I2Cx->CR1 &= ~(I2C_CR1_STOP);
	}
}


// 7 bit mode only
void I2C_SendAddress(I2C_TypeDef* I2Cx, uint16_t Address, I2C_Direction Direction)
{
	Address = (Address << 1);

	if (Direction != I2C_Direction_Transmitter)
	{
		// Set the address bit0 for read
		Address |= 1<<0;
	}
	else
	{
		// Reset the address bit0 for write
		Address &= ~(1<<0);
	}
	I2Cx->DR = Address;
}

//=================================================================
//							APIS
//=================================================================
/**================================================================
* @Fn				-MCAL_I2C_Init
* @brief			-Initializes I2C according to the specified parameters
* @param [in] 		-I2Cx: where x can be (1..2 depending on device used)
* @param [in] 		-I2C_InitStruct: All I2C Configuration I2C_InintTypeDef
* @retval 			-None
* Note				-None
*/
void MCAL_I2C_Init(I2C_TypeDef* I2Cx, I2C_InintTypeDef* I2C_InitStruct)
{
	uint16_t tmpreg = 0, freqrange = 0;
	uint32_t pclk1 = 8000000;
	uint16_t result = 0;
	// Enable Clock
	if		(I2Cx == I2C1) 		{	RCC_I2C1_CLK_EN();	Global_I2C_Config[I2C1_INDEX] = *I2C_InitStruct;	}
	else if	(I2Cx == I2C2) 		{	RCC_I2C2_CLK_EN();	Global_I2C_Config[I2C2_INDEX] = *I2C_InitStruct;	}

	if(I2C_InitStruct->Mode == I2C_Mode_I2C)
	{
		/*---------------------------- INIT Timing------------------------*/
		tmpreg = I2Cx->CR2;
		// Clear frequency I2C_CR2.FREQ[5:0] bits
		tmpreg &= ~(I2C_CR2_FREQ_Msk);
		//Get pclk1 frequency value
		pclk1 = MCAL_RCC_GetPCLK1Freq();
		// Set frequency bits depending on pclk1 value
		freqrange = (uint16_t) (pclk1 / 1000000);
		tmpreg |= freqrange;
		// Write to I2Cx_CR2
		I2Cx->CR2 = tmpreg;
		//• Configure the clock control registers I2C_CCR
		I2Cx->CR1 &= ~(I2C_CR1_PE);
		tmpreg = 0;

		if((I2C_InitStruct->ClockSpeed == I2C_SCLK_SM_50K) ||(I2C_InitStruct->ClockSpeed == I2C_SCLK_SM_100K))
		{
			// Tclk/2 = CRR * Tpclk1
			// CRR = Tclk / (2 * Tpclk1)
			// CRR = Fpclk1 / (2 * I2C_ClockFrequency)
			result = (uint16_t)(pclk1 / (I2C_InitStruct->ClockSpeed << 1));
			tmpreg |= result;
			I2Cx->CCR = tmpreg;
			/*---------------------------- I2C_TRISE Configuration ------------------------*/
			I2Cx->TRISE = freqrange + 1;

		}else
		{
			//Fast Mode not supported yet
		}
		/*---------------------------- I2C_CR1 Configuration ------------------------*/
		// Get the I2Cx CR1 value
		tmpreg = I2Cx->CR1;
		tmpreg |= (uint16_t)(I2C_InitStruct->ACK_Control | I2C_InitStruct->General_Call_Address_Detection | I2C_InitStruct->Mode | I2C_InitStruct->StretchMode);
		I2Cx->CR1 = tmpreg;

		/*---------------------------- I2Cx OAR1 & OAR2 Configuration ------------------------*/
		tmpreg = 0;
		if(I2C_InitStruct->I2C_Slave_Address.Enable_Dual_ADD_ack == 1)
		{
			tmpreg = I2C_OAR2_ENDUAL;
			tmpreg |= I2C_InitStruct->I2C_Slave_Address.Secondary_slave_addres << I2C_OAR2_ADD2_Pos;
			I2Cx->OAR2 = tmpreg;
		}
		tmpreg = 0;
		tmpreg |= I2C_InitStruct->I2C_Slave_Address.Primary_slave_addres << 1;
		tmpreg |= I2C_InitStruct->I2C_Slave_Address.Addressing_Slave_mode;
		I2Cx->OAR1 = tmpreg;

	}
	else
	{
		//SMBUS Not Supported yet
	}
	//Interrupt Mode (Slave Mode) // Check callback pointer != NULL
	if (I2C_InitStruct->P_Slave_Event_CallBack != NULL) // That enable Slave IRQ State mode
	{
		//Enable IRQ
		I2Cx->CR2 |= (I2C_CR2_ITERREN);
		I2Cx->CR2 |= (I2C_CR2_ITEVTEN);
		I2Cx->CR2 |= (I2C_CR2_ITBUFEN);
		if (I2Cx == I2C1)
		{
			NVIC_IRQ31_I2C1_EV_Enable;
			NVIC_IRQ32_I2C1_ER_Enable;
		}
		else if (I2Cx == I2C2)
		{
			NVIC_IRQ33_I2C2_EV_Enable;
			NVIC_IRQ34_I2C2_ER_Enable;
		}
		I2Cx->SR1 = 0;
		I2Cx->SR2 = 0;
	}

	//Enable the selected I2C peripheral
	I2Cx->CR1 |= I2C_CR1_PE;
}

/**================================================================
* @Fn				-MCAL_I2C_DeInit
* @brief			-DEInitializes I2Cx according to the specified parameters
* @param [in] 		-I2Cx: where x can be (1..2 depending on device used)
* @retval 			-None
* Note				-Reset the Model by RCC
*/
void MCAL_I2C_DeInit(I2C_TypeDef* I2Cx)
{
	if (I2Cx == I2C1)
	{
		RCC_I2C1_Reset();
		NVIC_IRQ31_I2C1_EV_Disable;
		NVIC_IRQ32_I2C1_ER_Disable;
	}
	else if (I2Cx == I2C2)
	{
		RCC_I2C2_Reset();
		NVIC_IRQ33_I2C2_EV_Disable;
		NVIC_IRQ34_I2C2_ER_Disable;
	}
}
/**================================================================
* @Fn				-MCAL_I2C_GPIO_Set_Pins
* @brief			-Initialize GPIO Pins
* @param [in] 		-I2Cx: where x can be (1..2 depending on device used)
* @retval 			-None
* Note				-Should enable the corresponding ALT & GPIO in RCC clock Also called after MCAL_I2C_Init()
*/
void MCAL_I2C_GPIO_Set_Pins(I2C_TypeDef* I2Cx)
{
	GPIO_PinConfig_t PinCfg;
	if (I2Cx == I2C1)
	{
		// PB6: I2C1_SCL
		// PB7: I2C1_SDA
		PinCfg.GPIO_PinNumber = GPIO_PIN_6;
		PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_OD;
		PinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
		MCAL_GPIO_Init(GPIOB, &PinCfg);

		PinCfg.GPIO_PinNumber = GPIO_PIN_7;
		PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_OD;
		PinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
		MCAL_GPIO_Init(GPIOB, &PinCfg);
	}
	else if (I2Cx == I2C2)
	{
		// PB10: I2C2_SCL
		// PB11: I2C2_SDA
		PinCfg.GPIO_PinNumber = GPIO_PIN_10;
		PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_OD;
		PinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
		MCAL_GPIO_Init(GPIOB, &PinCfg);

		PinCfg.GPIO_PinNumber = GPIO_PIN_11;
		PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_OD;
		PinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
		MCAL_GPIO_Init(GPIOB, &PinCfg);
	}
}

/**================================================================
* @Fn				-MCAL_I2C_Master_TX
* @brief			-Transmit data
* @param [in] 		-I2Cx: where x can be (1..2 depending on device used)
* @param [in] 		-devAddr: Slave address
* @param [in] 		-dataOut: data to be transmitted
* @param [in] 		-dataLen: data length
* @param [in] 		-Stop: with or without stop condition at the end
* @param [in] 		-start: with or without start condition
* @retval 			-None
*/
void MCAL_I2C_Master_TX (I2C_TypeDef* I2Cx, uint16_t devAddr, uint8_t* dataOut, uint32_t dataLen, Stop_Condition Stop, Repeated_Start start)
{
	int i = 0;
	// 1. Set The START bit in the I2C.CR1 register to generate a start condition
	I2C_GenerateSTART(I2Cx, ENABLE, start);

	// 2. Wait for EV5
	while (!I2C_GetFlagStatus(I2Cx, EV5));

	// 3. Send Address
	I2C_SendAddress(I2Cx, devAddr, I2C_Direction_Transmitter); // address

	// 4. Wait EV6
	while (!I2C_GetFlagStatus(I2Cx, EV6));

	// 5. TRA, BUSY, MSL, TXE flags
	while (!I2C_GetFlagStatus(I2Cx, MASTER_BYTE_TRANSMITTING));

	for (i = 0; i < dataLen; ++i)
	{
		// Write in the DR register the data to be sent
		I2Cx->DR = dataOut[i];
		// 6. Wait EV8
		while (!I2C_GetFlagStatus(I2Cx, EV8));
	}

	// 7. Wait EV8_2
	//while (!I2C_GetFlagStatus(I2Cx, EV8_2));

	if(Stop == With_Stop)
	{
		// 8. Send Stop condition
		I2C_GenerateSTOP(I2Cx, ENABLE);
	}
}

/**================================================================
* @Fn				-MCAL_I2C_Master_RX
* @brief			-Receive data
* @param [in] 		-I2Cx: where x can be (1..2 depending on device used)
* @param [in] 		-devAddr: Slave address
* @param [in] 		-dataOut: data to be received
* @param [in] 		-dataLen: data length
* @param [in] 		-Stop: with or without stop condition at the end
* @param [in] 		-start: with or without start condition
* @retval 			-None
*/
void MCAL_I2C_Master_RX (I2C_TypeDef* I2Cx, uint16_t devAddr, uint8_t* dataOut, uint32_t dataLen, Stop_Condition Stop, Repeated_Start start)
{
	int i = 0;
	uint8_t index = I2Cx == I2C1 ? I2C1_INDEX : I2C2_INDEX;
	// 1. Set The START bit in the I2C.CR1 register to generate a start condition
	I2C_GenerateSTART(I2Cx, ENABLE, start);

	// 2. Wait for EV5
	while (!I2C_GetFlagStatus(I2Cx, EV5));

	// 3. Send Address
	I2C_SendAddress(I2Cx, devAddr, I2C_Direction_Reciever); // address

	// 4. Wait EV6
	while (!I2C_GetFlagStatus(I2Cx, EV6));


	// 5. Read data
	if (dataLen)
	{
		// Read the data until Len becomes Zero
		for (i = dataLen; i > 1; --i)
		{
			// Wait until RXNE becomes 1
			while (!I2C_GetFlagStatus(I2Cx, EV7));
			// Read the data from data register in to buffer
			*dataOut = I2Cx->DR;
			//Read the buffer address
			dataOut++;
		}
		I2C_AcknowledgeConfig(I2Cx, DISABLE);
	}
	if (Stop == With_Stop)
	{
		// 6. Send Stop condition
		I2C_GenerateSTOP(I2Cx, ENABLE);
	}

	// Re-enable ACKing
	if (Global_I2C_Config[index].ACK_Control == I2C_ACK_Enable)
	{
		I2C_AcknowledgeConfig(I2Cx, ENABLE);
	}

}

/**================================================================
* @Fn				-MCAL_I2C_SlaveSendData
* @brief			-Slave Send data
* @param [in] 		-I2Cx: where x can be (1..2 depending on device used)
* @param [in] 		-data: data sent
* @retval 			-None
*/
void MCAL_I2C_SlaveSendData (I2C_TypeDef* I2Cx, uint8_t data)
{
	I2Cx->DR = data;
}

/**================================================================
* @Fn				-MCAL_I2C_SlaveReceiveData
* @brief			-Slave Send data
* @param [in] 		-I2Cx: where x can be (1..2 depending on device used)
* @retval 			-None
*/
uint8_t MCAL_I2C_SlaveReceiveData (I2C_TypeDef* I2Cx)
{
	return (uint8_t) I2Cx->DR;
}







