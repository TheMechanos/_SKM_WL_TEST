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
#include <Software/CircuralContainers/CircularQueue.h>
#include <Software/CircuralContainers/CircularDeque.h>

#include <Software/Vector/Vector.h>

#include <LinkedList.hpp>

#include <random>

#define DEBUG_SKM_CONTROLLER

#include <Modules/SX126x/SKMRadioSX126X.hpp>


class SKMController : public SKMListner{
public:
	constexpr static const bool LOG_ENABLE = true;

	typedef std::function<void(SKMPacketRx* packet)> SKMOnRxCallback;

	struct SKMOnRxCallbackInfo{
		SKMOnRxCallback callback;
		SKMPacket::Type type;
	};

	SKMController();
	SKMController(SKMRadio* radio, SKMPacket::Address nodeAddress);


	void init();
	void iterate();

	void enable();
	void disable();
	bool isEnabled();

	void onRx(SKMPacket::Type packetType, SKMOnRxCallback callback);

	void send(SKMPacketTx* packet);

	virtual void onTxDone();
	virtual void onRxDone();
	virtual void onTxRxFail();

private:

	void progressPacket(SKMPacketRx* packet);

	static int SKMLogLn(const char *format, ...);

	SKMRadio* radio;

	LinkedList<SKMPacketTx> txQueue;
	SKMPacketTx transmitingPacket;
	LinkedList<SKMPacketTx> txedQueue;
	LinkedList<SKMPacketRx> rxQueue;

	LinkedList<SKMOnRxCallbackInfo> onRxCallbackList;

	SKMPacket::Address nodeAddress;

	bool transmiting;


	bool enabled;



};

#endif /* LIB_SEBIXFRAMEWORK_RADIO_SKM_CONTROLLERS_SKMCONTROLLER_HPP_ */
