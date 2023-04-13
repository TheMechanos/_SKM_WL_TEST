/*
 * SKMRadioInterface.h
 *
 *  Created on: 5 kwi 2023
 *      Author: user
 */

#ifndef LIB_SKM_SKM_SX1262_INTERFACE_HPP_
#define LIB_SKM_SKM_SX1262_INTERFACE_HPP_

#include <System.hpp>

class SKM_SX126x_Interface {
public:

	virtual void init()=0;

	virtual void switchTx()=0;
	virtual void switchRx()=0;
	virtual void switchNone()=0;

	virtual bool execSetCmd(uint8_t command, uint8_t *pBuffer, uint16_t Size)=0;
	virtual bool execGetCmd(uint8_t command, uint8_t *pBuffer, uint16_t Size)=0;

	virtual bool writeBuffer(uint8_t Offset, uint8_t *pBuffer, uint16_t Size)=0;
	virtual bool readBuffer(uint8_t Offset, uint8_t *pBuffer, uint16_t Size)=0;

	virtual bool writeRegisters(uint8_t Address, uint8_t *pBuffer, uint16_t Size)=0;
	virtual bool readRegisters(uint8_t Address, uint8_t *pBuffer, uint16_t Size)=0;
	virtual bool writeRegister(uint8_t Address, uint8_t Value)=0;
	virtual bool readRegister(uint8_t Address, uint8_t *pValue)=0;

};

#endif /* LIB_SKM_SKM_SX1262_INTERFACE_HPP_ */
