/*
 * Stm32F103C6_SPI_Driver.h
 *
 *  Created on: Mar 11, 2022
 *      Author: moustafa
 */

#ifndef INC_STM32F103C6_SPI_DRIVER_H_
#define INC_STM32F103C6_SPI_DRIVER_H_

//Includes
#include "stm32f103x6.h"
#include "Stm32F103C6_GPIO_Driver.h"

struct S_IRQ_SRC
{
	uint8_t TXE:1; 	//TX buffer empty interrupt
	uint8_t RXNE:1; //RX buffer not empty interrupt
	uint8_t ERRI:1; //Error interrupt
	uint8_t Reserved:5;
};

//Configuration Structure
typedef struct
{
	uint16_t	Device_Mode;								// Specifies the SPI mode
															// This parameter must be set based on @ref SPI_Device_Mode_define

	uint16_t	Communication_Mode;							// Specifies the SPI bidirectional mode state
															// This parameter must be set based on @ref SPI_Communication_Mode_define

	uint16_t	Frame_Format;								// Specifies LSB or MSB
															// This parameter must be set based on @ref SPI_Frame_Format_define

	uint16_t	DataSize;									// This parameter must be set based on @ref SPI_DataSize_define

	uint16_t	CLKPolarity;								// This parameter must be set based on @ref SPI_CLKPolarity_define

	uint16_t	CLKPhase;									// This parameter must be set based on @ref SPI_CLKPhase_define

	uint16_t	NSS;										// Specifies whether the NSS signal is managed by hardware or by software using SII bit enable
															// This parameter must be set based on @ref SPI_NSS_define

	uint16_t	SPI_BAUDRATEPRESCALER;						/* Specifies the baud rate prescaler value which will be used to configure the transmit and
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 receive SCK clock.
	 	 	 	 	 	 	 	 	 	 	 				// This parameter must be set based on @ref SPI_BAUDRATEPRESCALER_define
	 	 	 	 	 	 	 	 	 	 	 				//@Note the communication clock is derived from the master clock. The slave clock doesn't
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 need to be set.
	 	 	 	 	 	 	 	 	 	 	 				//Take care you have to configure RCC to enter the correct clock to APB2 >> SPI1*/

	uint16_t	IRQ_Enable;									// This parameter must be set based on @ref SPI_IRQ_Enable_define

	void (* P_IRQ_Callback)(struct S_IRQ_SRC irq_src);		// Set the C function() which will be called once IRQ Happen

}SPI_Config;

//=======================================================================================
//							Macros Configuration References
//=======================================================================================

//@ref SPI_Device_Mode_define
#define SPI_Device_Mode_SLAVE								(0x00000000U)
#define SPI_Device_Mode_MASTER								(0x1U << 2) 	//CR1.MSTR : 1: Master configuration

//@ref SPI_Communication_Mode_define
#define SPI_DIRECTION_2LINES								(0x00000000U)
#define SPI_DIRECTION_2LINES_RXONLY							(0x1U << 10)	 //CR1 Bit 10 RXONLY: Receive only
#define SPI_DIRECTION_1LINE_recieve_only					(0x1U << 15)	//CR1 Bit 15 BIDIMODE: Bidirectional data mode enable
#define SPI_DIRECTION_1LINE_transmit_only					((0x1U << 15) | (0x1U << 14))	//CR1 Bit 15 BIDIMODE: Bidirectional data mode enable & Bit 14 BIDIOE: Output enable in bidirectional mode

//@ref SPI_Frame_Format_define
#define SPI_Frame_Format_MSB_transmitted_first				(0x00000000U)
#define SPI_Frame_Format_LSB_transmitted_first				(0x1U << 7)		//CR1 Bit 7 LSBFIRST: Frame format

//@ref SPI_DataSize_define
#define SPI_DataSize_8BIT									(0x00000000U)
#define SPI_DataSize_16BIT									(0x1U << 11) 	//CR1 Bit 11 DFF: Data frame format

//@ref SPI_CLKPolarity_define
#define SPI_CLKPolarity_LOW_when_idle						(0x00000000U)
#define SPI_CLKPolarity_HIGH_when_idle						(0x1U << 1)		//CR1 Bit1 CPOL: Clock polarity

//@ref SPI_CLKPhase_define
#define SPI_CLKPhase_1EDGE_first_data_capture_edge			(0x00000000U)
#define SPI_CLKPhase_2EDGE_first_data_capture_edge			(0x1U << 0)		//CR1 Bit 0 CPHA: Clock phase

//@ref SPI_NSS_define
//Hardware
#define SPI_NSS_Hard_Slave									(0x00000000U)
#define SPI_NSS_Hard_Master_SS_output_enable				(0x1U << 2)		//CR2 Bit 2 SSOE: SS output enable
#define SPI_NSS_Hard_Master_SS_output_disable				~(0x1U << 2)	//CR2 Bit 2 SSOE: SS output disable
//Software (Master or slave)
#define SPI_NSS_Soft_NSSInternalSoft_Reset					((0x1U << 9))	//CR1
#define SPI_NSS_Soft_NSSInternalSoft_Set					((0x1U << 9) | (0x1U << 8))

//@ref SPI_BAUDRATEPRESCALER_define
#define SPI_BAUDRATEPRESCALER_2								(0x00000000U)	//CR1 Bits 5:3 BR[2:0]: Baud rate control
#define SPI_BAUDRATEPRESCALER_4								(0b001U << 3)
#define SPI_BAUDRATEPRESCALER_8								(0b010U << 3)
#define SPI_BAUDRATEPRESCALER_16							(0b011U << 3)
#define SPI_BAUDRATEPRESCALER_32							(0b100U << 3)
#define SPI_BAUDRATEPRESCALER_64							(0b101U << 3)
#define SPI_BAUDRATEPRESCALER_128							(0b110U << 3)
#define SPI_BAUDRATEPRESCALER_256							(0b111U << 3)

//@ref SPI_IRQ_Enable_define
#define SPI_IRQ_Enable_NONE									(uint32_t)(0)
#define SPI_IRQ_Enable_TXEIE								(uint32_t)(1<<7) //CR2 Bit 7 TXEIE: Tx buffer empty interrupt enable
#define SPI_IRQ_Enable_RXNEIE								(uint32_t)(1<<6) //CR2 Bit 6 RXNEIE: RX buffer not empty interrupt enable
#define SPI_IRQ_Enable_ERRIE								(uint32_t)(1<<5) //CR2 Bit 5 ERRIE: Error interrupt enable


enum PollingMechanism{
	Pollingenable,
	Pollingdisable
};
/*
 * =======================================================================================
 * 								APIs Supported by "MCAL GPIO DRIVER"
 * =======================================================================================
 */
void MCAL_SPI_Init (SPI_TypeDef *SPIx, SPI_Config* SPI_Config);
void MCAL_SPI_DeInit (SPI_TypeDef *SPIx);

void MCAL_SPI_GPIO_Set_Pins (SPI_TypeDef *SPIx);

void MCAL_SPI_SendData (SPI_TypeDef* SPIx, uint16_t* pTxBuffer, enum PollingMechanism PollingEn);
void MCAL_SPI_ReceiveData (SPI_TypeDef* SPIx, uint16_t* pTxBuffer, enum PollingMechanism PollingEn);
void MCAL_SPI_TX_RX (SPI_TypeDef* SPIx, uint16_t* pTxBuffer, enum PollingMechanism PollingEn);

#endif /* INC_STM32F103C6_SPI_DRIVER_H_ */
