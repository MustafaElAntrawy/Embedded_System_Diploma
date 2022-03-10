 /*
 * Stm32F103C6_GPIO_Driver.c
 *
 *  Created on: Dec 20, 2021
 *      Author: Antrawy
 */
#include "Stm32F103C6_GPIO_Driver.h"


uint8_t Get_CRLH_Position(uint16_t PinNumber){
	switch(PinNumber){
	case GPIO_PIN_0:
		return 0;
		break;
	case GPIO_PIN_1:
		return 4;
		break;
	case GPIO_PIN_2:
		return 8;
		break;
	case GPIO_PIN_3:
		return 12;
		break;
	case GPIO_PIN_4:
		return 16;
		break;
	case GPIO_PIN_5:
		return 20;
		break;
	case GPIO_PIN_6:
		return 24;
		break;
	case GPIO_PIN_7:
		return 28;
		break;
	case GPIO_PIN_8:
		return 0;
		break;
	case GPIO_PIN_9:
		return 4;
		break;
	case GPIO_PIN_10:
		return 8;
		break;
	case GPIO_PIN_11:
		return 12;
		break;
	case GPIO_PIN_12:
		return 16;
		break;
	case GPIO_PIN_13:
		return 20;
		break;
	case GPIO_PIN_14:
		return 24;
		break;
	case GPIO_PIN_15:
		return 28;
		break;
	}
	return 0;
}


/**================================================================
* @Fn				-MCAL_GPIO_Init
* @brief			-Initializes the GPIOx PINy according to the specified parameters in PinConfig
* @param [in] 		-GPIOx: where x can be (A..E depending on device used) to select GPIO peripheral
* @param [in] 		-PinConfig pointer to a GPIO_PinConfig_t structure that contains
* 					the configuration infrmation for the specified GPIO PIN.
* @retval 			-None
* Note				-Stm32F103C6 MCU has GPIO A, B, C, D, E Modules
* 					But LQFP48 Package jas only GPIO A, B, PART of C/D exported as external PINs from the Module
*/

void MCAL_GPIO_Init (GPIO_TypeDef *GPIOx, GPIO_PinConfig_t *PinConfig){
	//Port configuration low (GPIO_CRL) Configure PINS from 0 >> 7
	//Port configuration high (GPIO_CRH) Configure PINS from 8 >> 15
	volatile uint32_t *configRegister = NULL;
	uint8_t PIN_Config = 0;

	configRegister = (PinConfig->GPIO_PinNumber < GPIO_PIN_8)? &GPIOx->CRL : &GPIOx->CRH;

	//Clear CNF[1:0] MODE[1:0]
	(*configRegister) &= ~(0xF << Get_CRLH_Position(PinConfig->GPIO_PinNumber));

	//If Pin is output
	if ((PinConfig->GPIO_MODE == GPIO_MODE_OUTPUT_AF_OD) || (PinConfig->GPIO_MODE == GPIO_MODE_OUTPUT_AF_PP) || (PinConfig->GPIO_MODE == GPIO_MODE_OUTPUT_OD) || (PinConfig->GPIO_MODE == GPIO_MODE_OUTPUT_PP)){
		//Set CNF[1:0] MODE[1:0]
		PIN_Config = ((((PinConfig->GPIO_MODE - 4) << 2) | (PinConfig->GPIO_Output_Speed)) & 0x0F);
	}
	//If Pin is input
	else{
		if ((PinConfig->GPIO_MODE == GPIO_MODE_INPUT_FLO) || (PinConfig->GPIO_MODE == GPIO_MODE_ANALOG)){
			PIN_Config = ( ( (PinConfig->GPIO_MODE << 2) | 0x0 ) & 0x0F);
		}
		else if (PinConfig->GPIO_MODE == GPIO_MODE_AF_INPUT){
			PIN_Config = ( ( (GPIO_MODE_INPUT_FLO << 2) | 0x0 ) & 0x0F);
		}
		//PU or PD input
		else{
			PIN_Config = ( ( (GPIO_MODE_INPUT_PU << 2) | 0x0 ) & 0x0F);
			if(PinConfig->GPIO_MODE == GPIO_MODE_INPUT_PU){
				//Table 20, Port bit configuration table PxODR 1
				GPIOx->ODR |= PinConfig->GPIO_PinNumber;
			}else{
				//Table 20, Port bit configuration table PxODR 0
				GPIOx->ODR &= ~(PinConfig->GPIO_PinNumber);
			}
		}
	}
	(*configRegister) |= ((PIN_Config) << Get_CRLH_Position(PinConfig->GPIO_PinNumber));

}

/**================================================================
* @Fn				-MCAL_GPIO_DeInit
* @brief			-Reset all the GPIO Registers
* @param [in] 		-GPIOx: where x can be (A..E depending on device used) to select GPIO peripheral
* @retval 			-None
* Note				-None
*/
void MCAL_GPIO_DeInit(GPIO_TypeDef *GPIOx){
//	GPIOx->CRL = 0x44444444;
//	GPIOx->CRH = 0x44444444;
//	//GPIOx->IDR = ; Read only
//	GPIOx->ODR = 0x00000000;
//	GPIOx->BSRR = 0x00000000;
//	GPIOx->BRR = 0x00000000;
//	GPIOx->LCKR = 0x00000000;
//OR==============================
	//Using reset controller >>Set and cleared by software.
	if(GPIOx == GPIOA)
	{
		RCC->APB2RSTR |= (1<<2);
		RCC->APB2RSTR &= ~(1<<2);
	}
	else if (GPIOx == GPIOB)
	{
		RCC->APB2RSTR |= (1<<3);
		RCC->APB2RSTR &= ~(1<<3);
	}
	else if (GPIOx == GPIOC)
	{
		RCC->APB2RSTR |= (1<<4);
		RCC->APB2RSTR &= ~(1<<4);
	}
	else if (GPIOx == GPIOD){
		RCC->APB2RSTR |= (1<<5);
		RCC->APB2RSTR &= ~(1<<5);
	}
	else if (GPIOx == GPIOE)
	{
		RCC->APB2RSTR |= (1<<6);
		RCC->APB2RSTR &= ~(1<<6);
	}

}

/**================================================================
* @Fn				-MCAL_GPIO_ReadPin
* @brief			-Read Specific PIN
* @param [in] 		-GPIOx: where x can be (A..E depending on device used) to select GPIO peripheral
* @param [in] 		-PinNumber: Set Pin Number according to @ref GPIO_PINS_define
* @retval 			-The input pin value (two values based on @ref GPIO_PIN_state)
* Note				-None
*/
uint8_t MCAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t PinNumber){

	if (((GPIOx->IDR) & PinNumber) != (uint32_t)GPIO_PIN_RESET)
	{
		return GPIO_PIN_SET;
	}
	else
	{
		return GPIO_PIN_RESET;
	}
}

/**================================================================
* @Fn				-MCAL_GPIO_ReadPort
* @brief			-Read Specific PORT
* @param [in] 		-GPIOx: where x can be (A..E depending on device used) to select GPIO peripheral
* @retval 			-The input port value
* Note				-None
*/
uint16_t MCAL_GPIO_ReadPort(GPIO_TypeDef *GPIOx){
	return (uint16_t)GPIOx->IDR;
}

/**================================================================
* @Fn				-MCAL_GPIO_WritePin
* @brief			-Write on Specific pin
* @param [in] 		-GPIOx: where x can be (A..E depending on device used) to select GPIO peripheral
* @param [in] 		-PinNumber: specific the port bit to read @ref GPIO_PINS_define
* @param [in] 		-Value: Pin value
* @retval 			-None
* Note				-None
*/
void MCAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t PinNumber, uint8_t Value){
	if (Value != GPIO_PIN_RESET)
	{
		//GPIOx->ODR |= PinNumber;
		//OR by using BSRR register
		GPIOx->BSRR = (uint32_t)PinNumber;
	}
	else
	{
		GPIOx->BRR = (uint32_t)PinNumber;
	}

}

/**================================================================
* @Fn				-MCAL_GPIO_WritePort
* @brief			-Write on Specific PORT
* @param [in] 		-GPIOx: where x can be (A..E depending on device used) to select GPIO peripheral
* @param [in] 		-Value: Port value
* @retval 			-None
* Note				-None
*/
void MCAL_GPIO_WritePort(GPIO_TypeDef *GPIOx, uint16_t Value){
	GPIOx->ODR = (uint32_t)Value;
}

/**================================================================
* @Fn				-MCAL_GPIO_TogglePin
* @brief			-Toggle Specific pin
* @param [in] 		-GPIOx: where x can be (A..E depending on device used) to select GPIO peripheral
* @param [in] 		-PinNumber: specific the port bit to read @ref GPIO_PINS_define
* @retval 			-None
* Note				-None
*/
void MCAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t PinNumber){
	GPIOx->ODR ^= (PinNumber);

}

/**================================================================
* @Fn				-MCAL_GPIO_LockPin
* @brief			-The locking mechanism allows the IO configuration to be frozen
* @param [in] 		-GPIOx: where x can be (A..E depending on device used) to select GPIO peripheral
* @param [in] 		-PinNumber: specific the port bit to read @ref GPIO_RETURN_LOCK
* @retval 			-OK if pin configure is locked Or ERROR if pin is not locked (OK & ERROR are defined @ref GPIO_RETURN_LOCK)
* Note				-None
*/
uint8_t MCAL_GPIO_LockPin(GPIO_TypeDef *GPIOx, uint16_t PinNumber){

	//Set LCKK[16]
	volatile uint32_t temp = 1<<16;
	//Set the LCKy
	temp |= PinNumber;

	//Write 1
	GPIOx->LCKR = temp;
	//Write 0
	GPIOx->LCKR = PinNumber;
	//Write 1
	GPIOx->LCKR = temp;
	//Read 0
	temp = GPIOx->LCKR;
	//Read 1 (this read is optional but confirms that the lock is active)
	if((uint32_t)GPIOx->LCKR & 1<<16){
		return GPIO_RETURN_LOCK_Enabled;
	}
	else{
		return GPIO_RETURN_LOCK_ERROR;
	}

}



