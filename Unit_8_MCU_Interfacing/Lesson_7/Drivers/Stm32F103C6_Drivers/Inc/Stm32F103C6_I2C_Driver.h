/*
 * Stm32F103C6_I2C_Driver.h
 *
 *  Created on: Mar 11, 2022
 *      Author: moustafa
 */

#ifndef INC_STM32F103C6_I2C_DRIVER_H_
#define INC_STM32F103C6_I2C_DRIVER_H_

//Includes
#include "stm32f103x6.h"
#include "Stm32F103C6_GPIO_Driver.h"
#include "Stm32F103C6_RCC_Driver.h"

struct S_I2C_Slave_Device_Address
{
	uint16_t							Enable_Dual_ADD_ack; 				// 1-Enable 0-Disable
	uint16_t 							Primary_slave_addres;
	uint16_t							Secondary_slave_addres;
	uint16_t							Addressing_Slave_mode;				//@ref I2C_Addressing_Slave_mode_define
};

typedef enum
{
	I2C_EV_STOP,
	I2C_ERROR_AF,
	I2C_EV_ADDR_Matched,
	I2C_EV_DATA_REQ,	//The APP layer should send the data (I2C_SlaveSendData)
	I2C_EV_DATA_RCV		// The APP layer should read the data (I2C_SlaveReceiveData)
}Slave_State;

//Configuration Structure
typedef struct
{
	uint32_t							ClockSpeed;							// Specifies the clock frequency
																			// This parameter must be set based on @ref I2C_SCLK_define

	uint32_t							StretchMode;						//@ref I2C_StretchMode_define

	uint32_t 							Mode;								// Specifies the I2C mode.
																			//@ref I2C_ Mode_define
	struct S_I2C_Slave_Device_Address	I2C_Slave_Address;

	uint32_t							ACK_Control;						//@ref I2C_ACK_define

	uint32_t							General_Call_Address_Detection;		//@ref I2C_ENGC_define

	void(* P_Slave_Event_CallBack)(Slave_State state);
}I2C_InintTypeDef;

//=======================================================================================
//							Macros Configuration References
//=======================================================================================

//@ref I2C_SCLK_define
// Standard Speed (up to 100 KHz)
// Fast Speed (up to 400 KHz)
// To configure clock before enable the Peripheral
// --I2C_CR2.FREQ[5:0]: Peripheral clock frequency
//• Configure the clock control registers
//		Thigh = CCR * TPCLK1
//		SM or FM
//• Configure the rise time register I2C_TRISE
// --
#define I2C_SCLK_SM_50K								(0x50000U)
#define I2C_SCLK_SM_100K							(100000U)
#define I2C_SCLK_FM_200K							(200000U) // Fast mode not supported yet
#define I2C_SCLK_FM_400K							(400000U) // Fast mode not supported yet


//@ref I2C_StretchMode_define
// I2C_CR1 Bit 7
#define I2C_StretchMode_Enable 						0x00000000U
#define I2C_StretchMode_Disable 					I2C_CR1_NOSTRETCH


//@ref I2C_ Mode_define
//I2C_CR1 Bit1
#define I2C_Mode_I2C								0
#define I2C_Mode_SMBus								I2C_CR1_SMBUS


//@ref I2C_Addressing_Slave_mode_define
//I2C_OAR1 Bit15
#define I2C_Addressing_Slave_mode_7Bit				0x00000000U
#define I2C_Addressing_Slave_mode_10Bit				(uint16_t)(1<<15)


//@ref I2C_ACK_define
//I2C_CR1 Bit10
#define I2C_ACK_Enable								(I2C_CR1_ACK)
#define I2C_ACK_Disable								((uint16_t)0x0000)

//@ref I2C_ENGC_define
//I2C_CR1 Bit6
#define I2C_ENGC_Enable								I2C_CR1_ENGC
#define I2C_ENGC_Disable							0x00000000U

// =======================================================================================

#define I2C_EVENT_MASTER_BYTE_TRANSMITTING			((uint32_t) 0x00070080) // TRA, BUSY, MSL, TXE flags

typedef enum
{
	With_Stop,
	Without_Stop
} Stop_Condition;

typedef enum
{
	Start,
	repeated_start
} Repeated_Start;

typedef enum
{
	DISABLE = 0,
	ENABLE = 1
} FunctionalState;

typedef enum
{
	RESET = 0,
	SET = 1
} FlagStatus;

typedef enum
{
	I2C_FLAG_BUSY,
	EV5,
	EV6,
	EV7,
	EV8,
	EV8_1,
	MASTER_BYTE_TRANSMITTING = ((uint32_t) 0x00070080)
} Status;

typedef enum
{
	I2C_Direction_Transmitter = 0,
	I2C_Direction_Reciever = 1
} I2C_Direction;
/*
 * =======================================================================================
 * 								APIs Supported by "MCAL GPIO DRIVER"
 * =======================================================================================
 */

void MCAL_I2C_Init(I2C_TypeDef* I2Cx, I2C_InintTypeDef* I2C_InitStruct);
void MCAL_I2C_DeInit(I2C_TypeDef* I2Cx);

void MCAL_I2C_GPIO_Set_Pins(I2C_TypeDef* I2Cx);

void MCAL_I2C_Master_TX (I2C_TypeDef* I2Cx, uint16_t devAddr, uint8_t* dataOut, uint32_t dataLen, Stop_Condition Stop, Repeated_Start start);
void MCAL_I2C_Master_RX (I2C_TypeDef* I2Cx, uint16_t devAddr, uint8_t* dataOut, uint32_t dataLen, Stop_Condition Stop, Repeated_Start start);

void MCAL_I2C_SlaveSendData (I2C_TypeDef* I2Cx, uint8_t data);
uint8_t MCAL_I2C_SlaveReceiveData (I2C_TypeDef* I2Cx);

//=================================================================
//						Generic APIs
//=================================================================
void I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState, Repeated_Start start);

void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState);

FlagStatus I2C_GetFlagStatus(I2C_TypeDef* I2Cx, Status flag);

void I2C_SendAddress(I2C_TypeDef* I2Cx, uint16_t Address, I2C_Direction  Direction);

void I2C_AcknowledgeConfig(I2C_TypeDef* I2Cx, FunctionalState state);





#endif /* INC_STM32F103C6_I2C_DRIVER_H_ */
