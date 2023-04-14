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

class SKMListner{
public:
	virtual void onTxDone()=0;
	virtual void onRxDone()=0;
	virtual void onTxRxFail()=0;
};

class SKMRadio{
public:
	virtual void init()=0;
	virtual void iterate(){};

	virtual bool sendPacket(SKMPacketTx* packet)=0;
	virtual SKMPacketRx* importAvalaiblePacket()=0;

	virtual void registerListner(SKMListner* listner){
		this->listner = listner;
	}

protected:
	SKMListner* listner = nullptr;


};



#endif /* LIB_SEBIXFRAMEWORK_RADIO_SKM_SKMRADIO_HPP_ */
