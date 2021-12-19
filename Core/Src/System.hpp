/*
 * System.hpp
 *
 *  Created on: 23.10.2020
 *      Author: SHON
 */

#ifndef SYSTEM_HPP_
#define SYSTEM_HPP_

#include <stm32wlxx.h>


class SYSTEM{
public:

	static void SystemErrorHandler();

	static uint32_t getTick();

	static void setLoopsInTenMs(uint16_t s);

	static uint16_t getLoopsInTenMs();

	static int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);


private:
	static uint16_t loopsInTenMs;

};


#endif /* SYSTEM_HPP_ */
