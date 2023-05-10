/*
 * SX126xController.hpp
 *
 *  Created on: 18 gru 2021
 *      Author: skocz
 */

#ifndef LIB_SEBIXFRAMEWORK_RADIO_MODULES_SX126X_SKMRADIOSX126X_HPP_
#define LIB_SEBIXFRAMEWORK_RADIO_MODULES_SX126X_SKMRADIOSX126X_HPP_

#include "../../Packet.hpp"
#include "../../Utility/SKP2PRadio.hpp"
#include <System.hpp>
#include "SX126x.hpp"

namespace SKP2P{
	class RadioSX126X : public SX126x, public Radio{
	public:

		RadioSX126X()=default;
		RadioSX126X(SX126x_Interface* interface);

		virtual void init();
		virtual void iterate();

		virtual void setSleep();
		virtual void setIdle();
		virtual void setRx();
		void setModeTxContinousWave();

		//virtual State getState();

		virtual bool sendPacket(PacketTx* packet);


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
		PacketRx* importAvalaiblePacket();

		bool isSleep;


		bool isReceiving;
		uint32_t receivingStartTime;

	};
}


#endif /* LIB_SEBIXFRAMEWORK_RADIO_MODULES_SX126X_SKMRADIOSX126X_HPP_ */
