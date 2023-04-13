/*
 * SKMRadio.hpp
 *
 *  Created on: 16 lut 2022
 *      Author: skocz
 */

#ifndef LIB_SEBIXFRAMEWORK_RADIO_SKM_SKMRADIO_HPP_
#define LIB_SEBIXFRAMEWORK_RADIO_SKM_SKMRADIO_HPP_

#include <System.hpp>
#include "Packet.hpp"

class SKMRadioListner{
public:
	virtual void onTxDone()=0;
	virtual void onRxDone()=0;
	virtual void onTxRxFail()=0;
};

class SKMRadio{
public:
	enum class State {
		Sleep, Idle, Rx, Tx
	};

	virtual void iterate(){};

	virtual void setSleep()=0;
	virtual void setIdle()=0;
	virtual void setRx()=0;

	virtual State getState()=0;


	virtual bool sendPacket(SKMPacketTx* packet)=0;
	virtual SKMPacketRx* importAvalaiblePacket()=0;

	void registerListner(SKMRadioListner* listner){
		this->listner = listner;
	}



protected:
	SKMRadioListner* listner;
};



#endif /* LIB_SEBIXFRAMEWORK_RADIO_SKM_SKMRADIO_HPP_ */
