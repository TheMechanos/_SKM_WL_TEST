/*
 * SKMController.hpp
 *
 *  Created on: 16 lut 2022
 *      Author: skocz
 */

#ifndef LIB_SEBIXFRAMEWORK_RADIO_SKM_CONTROLLERS_SKMCONTROLLER_HPP_
#define LIB_SEBIXFRAMEWORK_RADIO_SKM_CONTROLLERS_SKMCONTROLLER_HPP_

#include <Modules/SX126x/RadioSX126X.hpp>
#include <System.hpp>

#include "Packet.hpp"
#include "Utility/SKP2PRadio.hpp"
#include "Utility/LinkedList.hpp"



namespace SKP2P{

	class Controller : public RadioListner{
	public:
		constexpr static const bool LOG_ENABLE = true;
		constexpr static const uint32_t TRANSMIT_RANDOM_TIMEOUT_MAX = 5;
		constexpr static const uint32_t RXED_QUEUE_MAX_LENGTH = 10;

		typedef std::function<void(PacketRx* packet)> OnRxCallback;

		struct OnRxCallbackInfo{
			OnRxCallback callback;
			Packet::Type type;
		};


		Controller();
		Controller(Radio* radio, Packet::Address nodeAddress);


		void init();
		void iterate();

		void transmit(PacketTx* packet);
		void onRx(Packet::Type packetType, OnRxCallback callback);


		virtual void onTxDone();
		virtual void onRxDone(PacketRx* packet);
		virtual void onTxRxFail();

	protected:
		static int SKMLogLn(const char *format, ...);
		void progressPacket(PacketRx* packet);

	private:
		void generateNextTxRandomTime();

		Radio* radio;
		Packet::Address nodeAddress;


		LinkedList<PacketTx> txQueue;
		PacketTx* transmitingPacket;
		LinkedList<PacketTx> txedQueue;

		LinkedList<PacketRx> rxQueue;

		uint32_t nextTxTime;

		LinkedList<OnRxCallbackInfo> onRxCallbackList;

	};
}

#endif /* LIB_SEBIXFRAMEWORK_RADIO_SKM_CONTROLLERS_SKMCONTROLLER_HPP_ */
