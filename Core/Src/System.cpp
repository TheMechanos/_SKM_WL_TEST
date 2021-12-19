/*
 * System.cpp
 *
 *  Created on: 23.10.2020
 *      Author: SHON
 */

#include "System.hpp"

uint16_t SYSTEM::loopsInTenMs;



void SYSTEM::SystemErrorHandler(){
	while(1){


	}
}


uint32_t SYSTEM::getTick(){
	return HAL_GetTick();
}


void SYSTEM::setLoopsInTenMs(uint16_t s){
	loopsInTenMs=s;
}


uint16_t SYSTEM::getLoopsInTenMs(){
	return loopsInTenMs;
}



int32_t SYSTEM::map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
