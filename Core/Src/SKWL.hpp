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
#include <Radio/Modules/RadioRFSwitch.hpp>
#include <Radio/Modules/SX126x/SX126xController.hpp>
#include <Radio/Modules/SX126x/Config/SXExampleConfig.hpp>






class SKWL{

public:
	SKWL();

	void init();

	void iterateCritical();
	void iterateNonCritical();

	static SKWL* getInstance();

public:
	static SKWL INSTANCE;
	void SystemClock_Config();

	PIN pinLed[3];
	PIN pinSw[3];

	PIN pinRfSwTx;
	PIN pinRfSwRx;

	OUTPUT_ADVENCED led[3];
	BUTTON_CLICK button[3];

	OUTPUT_PIN rfSwTx;
	OUTPUT_PIN rfSwRx;

	SubGhz subghz;
	RadioRFSwitch rfSw;

	SX126xController sx;

};




#endif /* SRC_SKWL_HPP_ */
