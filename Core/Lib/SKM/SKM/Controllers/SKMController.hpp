/*
 * SKMController.hpp
 *
 *  Created on: 16 lut 2022
 *      Author: skocz
 */

#ifndef LIB_SEBIXFRAMEWORK_RADIO_SKM_CONTROLLERS_SKMCONTROLLER_HPP_
#define LIB_SEBIXFRAMEWORK_RADIO_SKM_CONTROLLERS_SKMCONTROLLER_HPP_

#include <System.hpp>
#include "../Packet.hpp"
#include "../SKMRadio.hpp"

#include <LinkedList.hpp>

//#define DEBUG_SKM_CONTROLLER

#include <Modules/SX126x/SKMRadioSX126X.hpp>


class SKMController : public SKMListner{
public:
	constexpr static const bool LOG_ENABLE = true;
	constexpr static const uint32_t TRANSMIT_RANDOM_TIMEOUT_MAX = 5;
	constexpr static const uint32_t RXED_QUEUE_MAX_LENGTH = 10;

	typedef std::function<void(SKMPacketRx* packet)> SKMOnRxCallback;

	struct SKMOnRxCallbackInfo{
		SKMOnRxCallback callback;
		SKMPacket::Type type;
	};

	struct RoutingInfo {
		SKMPacket::Address destiny;
		SKMPacket::Address nextHop;
		uint8_t hops;
		uint8_t evaluation;
	};


	SKMController();
	SKMController(SKMRadio* radio, SKMPacket::Address nodeAddress);


	void init();
	void iterate();

	void transmit(SKMPacketTx* packet);

	void onRx(SKMPacket::Type packetType, SKMOnRxCallback callback);


	virtual void onTxDone();
	virtual void onRxDone(SKMPacketRx* packet);
	virtual void onTxRxFail();

protected:
	static int SKMLogLn(const char *format, ...);
	void progressPacket(SKMPacketRx* packet);

	bool preparePacketToSend(SKMPacketTx* packet);

private:
	SKMPacketTx generateAckPacket(SKMPacketRx* rxPacket);
	void generateNextTxRandomTime();

	SKMRadio* radio;
	SKMPacket::Address nodeAddress;


	LinkedList<SKMPacketTx> txQueue;
	SKMPacketTx* transmitingPacket;
	LinkedList<SKMPacketTx> txedQueue;

	LinkedList<SKMPacketRx> rxQueue;
	LinkedList<SKMPacketRx> rxedQueue;

	LinkedList<RoutingInfo> routing;


	uint32_t nextTxTime;

	LinkedList<SKMOnRxCallbackInfo> onRxCallbackList;



};

#endif /* LIB_SEBIXFRAMEWORK_RADIO_SKM_CONTROLLERS_SKMCONTROLLER_HPP_ */
