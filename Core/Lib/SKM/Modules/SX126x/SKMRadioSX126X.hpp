/*
 * SX126xController.hpp
 *
 *  Created on: 18 gru 2021
 *      Author: skocz
 */

#ifndef LIB_SEBIXFRAMEWORK_RADIO_MODULES_SX126X_SKMRADIOSX126X_HPP_
#define LIB_SEBIXFRAMEWORK_RADIO_MODULES_SX126X_SKMRADIOSX126X_HPP_

#include "SX126x.hpp"
#include <System.hpp>


#include <Software/CircuralContainers/CircularQueue.h>

#include <SKM/Packet.hpp>
#include <SKM/SKMRadio.hpp>

class SKMRadioSX126X : public SX126x, public SKMRadio{
public:

	SKMRadioSX126X()=default;
	SKMRadioSX126X(SKM_SX126x_Interface* interface);

	virtual void init();
	virtual void iterate();

	virtual void setSleep();
	virtual void setIdle();
	virtual void setRx();
	void setModeTxContinousWave();

	//virtual State getState();

	virtual bool sendPacket(SKMPacketTx* packet);


	void irqTxCpltCallback();
	void irqRxCpltCallback();
	void irqPreambleDetectedCallback();
	void irqSyncWordValidCallback();
	void irqCRCErrorCallback();
	void irqRxTxTimeoutCallback();


private:
	constexpr static const uint32_t RECEIVING_TIMEOUT = 10;
	constexpr static const uint32_t TRANSMITING_TIMEOUT = 10;

	bool transmit(uint8_t* buffer, uint8_t size, uint32_t timeout);
	SKMPacketRx* importAvalaiblePacket();

	bool isSleep;


	bool isReceiving;
	uint32_t receivingStartTime;

};


/*
void dbgON();
void dbgOFF();
*/
#endif /* LIB_SEBIXFRAMEWORK_RADIO_MODULES_SX126X_SKMRADIOSX126X_HPP_ */
