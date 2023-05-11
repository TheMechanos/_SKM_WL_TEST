/*
 * SK13.hpp
 *
 *  Created on: Sep 13, 2021
 *      Author: skocz
 */

#ifndef SRC_SKWL_HPP_
#define SRC_SKWL_HPP_


#include <stm32wlxx.h>

#include <System.hpp>

#include <BasicTypes/IO/Input.hpp>
#include <BasicTypes/IO/Output.hpp>
#include <BasicTypes/Key/Button.hpp>
#include <BasicTypes/Key/ButtonClick.hpp>
#include <Software/CircuralContainers/CircularQueue.h>
#include <Interfaces/SubGhz/SubGhz.hpp>
#include <Peripheral/GPIO/IO_Pin.hpp>
#include <Peripheral/GPIO/PIN.hpp>

#include <Modules/SX126x/Config/SXExampleConfig.hpp>
#include <Modules/SX126x/Interfaces/HAL_STM32WL.hpp>
#include <Modules/SX126x/RadioSX126X.hpp>
#include <SKP2PController.hpp>




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
	RNG_HandleTypeDef hrng;

	PIN pinLed[3];
	PIN pinSw[3];

	PIN pinRfSwTx;
	PIN pinRfSwRx;

	OUTPUT_ADVENCED led[3];
	BUTTON_CLICK button[3];

	SKP2P::SX126x_STM32WL_HAL_Interface radioInterface;
	SKP2P::RadioSX126X sxRadio;
	SKP2P::Controller radio;





};




#endif /* SRC_SKWL_HPP_ */
