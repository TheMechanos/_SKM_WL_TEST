/*
 * SKMRadio.hpp
 *
 *  Created on: 16 lut 2022
 *      Author: skocz
 */

#ifndef LIB_SEBIXFRAMEWORK_RADIO_SKM_SKMRADIO_HPP_
#define LIB_SEBIXFRAMEWORK_RADIO_SKM_SKMRADIO_HPP_

#include "../Packet.hpp"
#include <System.hpp>

namespace SKP2P{

	class RadioListner {
	public:
		virtual void onTxDone()=0;
		virtual void onRxDone(PacketRx* packet)=0;
		virtual void onTxRxFail()=0;
	};

	class Radio {
	public:
		virtual void init()=0;
		virtual void iterate(){};

		virtual bool sendPacket(PacketTx* packet)=0;

		virtual void registerListner(RadioListner* listner){
			this->listner = listner;
		}

	protected:
		RadioListner *listner = nullptr;

	};
}




#endif /* LIB_SEBIXFRAMEWORK_RADIO_SKM_SKMRADIO_HPP_ */
