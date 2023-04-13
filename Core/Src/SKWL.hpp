/*
 * SK13.hpp
 *
 *  Created on: Sep 13, 2021
 *      Author: skocz
 */

#ifndef SRC_SKWL_HPP_
#define SRC_SKWL_HPP_


#include <stm32wlxx.h>

#include <BasicTypes/Key/Button.hpp>
#include <BasicTypes/Key/ButtonClick.hpp>

#include <BasicTypes/IO/Input.hpp>
#include <BasicTypes/IO/Output.hpp>


#include <Peripheral/GPIO/PIN.hpp>
#include <Peripheral/GPIO/IO_Pin.hpp>

#include <System.hpp>

#include <Interfaces/SubGhz/SubGhz.hpp>
#include <Modules/SX126x/Config/SXExampleConfig.hpp>
#include <Modules/SX126x/Interfaces/HAL_STM32WL.hpp>
#include <Modules/SX126x/SKMRadioSX126X.hpp>
#include <SKM/Controllers/SKMController.hpp>

#include <Software/CircuralContainers/CircularQueue.h>



class SKWL{

public:
	SKWL();

	void init();

	void iterateCritical();
	void iterateNonCritical();

	CircularQueue<char, 2048> logBuffor;

	static SKWL* getInstance();

public:
	static SKWL INSTANCE;
	void SystemClock_Config();

	void initUart();
	UART_HandleTypeDef hlpuart1;

	PIN pinLed[3];
	PIN pinSw[3];

	PIN pinRfSwTx;
	PIN pinRfSwRx;

	OUTPUT_ADVENCED led[3];
	BUTTON_CLICK button[3];

	SKMRadioSX126X sxRadio;
	SKMController radio;
	SKM_SX126x_STM32WL_HAL_Interface radioInterface;




};




#endif /* SRC_SKWL_HPP_ */
