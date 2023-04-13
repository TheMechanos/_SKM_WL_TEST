/*
 * RadioInterface.hpp
 *
 *  Created on: 5 kwi 2023
 *      Author: user
 */

#ifndef SKM_HAL_STM32_HPP_
#define SKM_HAL_STM32_HPP_

#include "SKM_SX126x_Interface.hpp"

#define SF_SUBGHZ_CRITICAL_START() uint32_t primask_bit= __get_PRIMASK();__disable_irq()
#define SF_SUBGHZ_CRITICAL_END() __set_PRIMASK(primask_bit)

class SKM_SX126x_STM32WL_HAL_Interface : public SKM_SX126x_Interface {
public:
	enum class BaudRatePrescaller {
		BR2 = SUBGHZSPI_BAUDRATEPRESCALER_2,
		BR4 = SUBGHZSPI_BAUDRATEPRESCALER_4,
		BR8 = SUBGHZSPI_BAUDRATEPRESCALER_8,
		BR16 = SUBGHZSPI_BAUDRATEPRESCALER_16,
		BR32 = SUBGHZSPI_BAUDRATEPRESCALER_32,
		BR64 = SUBGHZSPI_BAUDRATEPRESCALER_64,
		BR128 = SUBGHZSPI_BAUDRATEPRESCALER_128,
		BR256 = SUBGHZSPI_BAUDRATEPRESCALER_256,
	};

	SKM_SX126x_STM32WL_HAL_Interface() = default;
	SKM_SX126x_STM32WL_HAL_Interface(BaudRatePrescaller prescaller, uint32_t priority, PIN* txPin, PIN* rxPin){
		this->prescaller = prescaller;
		this->priority = priority;
		this->txPin = txPin;
		this->rxPin = rxPin;
	}

	SUBGHZ_HandleTypeDef* getHandler(){
		return &handler;
	}

	void init(){

		GPIO_InitTypeDef gp;
		gp.Mode = GPIO_MODE_OUTPUT_PP;
		gp.Pull = GPIO_NOPULL;
		gp.Speed = GPIO_SPEED_FREQ_MEDIUM;

		txPin->Init(&gp);
		rxPin->Init(&gp);
		switchNone();

		__HAL_RCC_SUBGHZ_CLK_ENABLE();

		handler.Init.BaudratePrescaler = (uint32_t) prescaller;

		HAL_SUBGHZ_Init(&handler);

		HAL_NVIC_SetPriority(SUBGHZ_Radio_IRQn, priority, 0);
		HAL_NVIC_EnableIRQ(SUBGHZ_Radio_IRQn);

	}

	virtual void switchTx(){
		HAL_GPIO_WritePin(rxPin->getGPIO(), rxPin->getPIN(), GPIO_PIN_RESET);
		HAL_GPIO_WritePin(txPin->getGPIO(), txPin->getPIN(), GPIO_PIN_SET);
	}
	virtual void switchRx(){
		HAL_GPIO_WritePin(txPin->getGPIO(), txPin->getPIN(), GPIO_PIN_RESET);
		HAL_GPIO_WritePin(rxPin->getGPIO(), rxPin->getPIN(), GPIO_PIN_SET);
	}
	virtual void switchNone(){
		HAL_GPIO_WritePin(txPin->getGPIO(), txPin->getPIN(), GPIO_PIN_RESET);
		HAL_GPIO_WritePin(rxPin->getGPIO(), rxPin->getPIN(), GPIO_PIN_RESET);
	}

	virtual bool execSetCmd(uint8_t command, uint8_t* pBuffer, uint16_t Size){
		SF_SUBGHZ_CRITICAL_START();
		HAL_StatusTypeDef a = HAL_SUBGHZ_ExecSetCmd(&handler, (SUBGHZ_RadioSetCmd_t) command, pBuffer, Size);
		SF_SUBGHZ_CRITICAL_END();
		return a == HAL_OK;
	}
	virtual bool execGetCmd(uint8_t command, uint8_t* pBuffer, uint16_t Size){
		SF_SUBGHZ_CRITICAL_START();
		HAL_StatusTypeDef a = HAL_SUBGHZ_ExecGetCmd(&handler, (SUBGHZ_RadioGetCmd_t) command, pBuffer, Size);
		SF_SUBGHZ_CRITICAL_END();
		return a == HAL_OK;
	}

	virtual bool writeBuffer(uint8_t Offset, uint8_t* pBuffer, uint16_t Size){
		SF_SUBGHZ_CRITICAL_START();
		HAL_StatusTypeDef a = HAL_SUBGHZ_WriteBuffer(&handler, Offset, pBuffer, Size);
		SF_SUBGHZ_CRITICAL_END();
		return a == HAL_OK;
	}
	virtual bool readBuffer(uint8_t Offset, uint8_t* pBuffer, uint16_t Size){
		SF_SUBGHZ_CRITICAL_START();
		HAL_StatusTypeDef a = HAL_SUBGHZ_ReadBuffer(&handler, Offset, pBuffer, Size);
		SF_SUBGHZ_CRITICAL_END();
		return a == HAL_OK;
	}

	virtual bool writeRegisters(uint8_t Address, uint8_t* pBuffer, uint16_t Size){
		SF_SUBGHZ_CRITICAL_START();
		HAL_StatusTypeDef a = HAL_SUBGHZ_WriteRegisters(&handler, (uint16_t) Address, pBuffer, Size);
		SF_SUBGHZ_CRITICAL_END();
		return a == HAL_OK;
	}
	virtual bool readRegisters(uint8_t Address, uint8_t* pBuffer, uint16_t Size){
		SF_SUBGHZ_CRITICAL_START();
		HAL_StatusTypeDef a = HAL_SUBGHZ_ReadRegisters(&handler, (uint16_t) Address, pBuffer, Size);
		SF_SUBGHZ_CRITICAL_END();
		return a == HAL_OK;
	}
	virtual bool writeRegister(uint8_t Address, uint8_t Value){
		SF_SUBGHZ_CRITICAL_START();
		HAL_StatusTypeDef a = HAL_SUBGHZ_WriteRegister(&handler, (uint16_t) Address, Value);
		SF_SUBGHZ_CRITICAL_END();
		return a == HAL_OK;
	}
	virtual bool readRegister(uint8_t Address, uint8_t* pValue){
		SF_SUBGHZ_CRITICAL_START();
		HAL_StatusTypeDef a = HAL_SUBGHZ_ReadRegister(&handler, (uint16_t) Address, pValue);
		SF_SUBGHZ_CRITICAL_END();
		return a == HAL_OK;
	}

private:
	SUBGHZ_HandleTypeDef handler = { 0 };
	BaudRatePrescaller prescaller;
	uint32_t priority;
	PIN *txPin;
	PIN *rxPin;

};

#endif /* SRC_RADIOINTERFACE_HPP_ */
