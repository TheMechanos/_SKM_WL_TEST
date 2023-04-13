/*
 * SKMController.cpp
 *
 *  Created on: 16 lut 2022
 *      Author: skocz
 */

#include "SKMController.hpp"

SKMController::SKMController(){
	transmiting = false;

}

SKMController::SKMController(SKMRadio* radio, SKMPacket::Address nodeAddress) :
		SKMController(){
	this->radio = radio;
	this->nodeAddress = nodeAddress;

}

void SKMController::enable(){
	enabled = true;
	radio->setRx();
}
void SKMController::disable(){
	enabled = false;
	radio->setIdle();
}
bool SKMController::isEnabled(){
	return enabled;
}

void SKMController::init(){
	enable();
	radio->registerListner(this);

}

void SKMController::iterate(){
	if(!isEnabled())
		return;

	if(txQueue.size() > 0){

		SKMPacketTx packet = txQueue.shift();
		bool sended = radio->sendPacket(&packet);
		if(sended){
#ifdef DEBUG_SKM_CONTROLLER
			System::logLn("Packet ID: %d was push to send.", packet.getHeader().id);
#endif
			transmitingPacket = packet;
			transmiting = true;
		}else{
#ifdef DEBUG_SKM_CONTROLLER
			System::logLn("At this moment can't push packet ID: %d to send!", packet.getHeader().id);
#endif
			txQueue.add(0, packet);
		}
	}

	if(txedQueue.size() > 0){
		auto packet = txedQueue.shift();

		if(packet.isAckTimeout()){

			if(packet.canDoRetransmition()){
#ifdef DEBUG_SKM_CONTROLLER
				System::logLn("Packet ID: %d ack timeouted, push to retransmition!", transmitingPacket.getHeader().id);
#endif
				txQueue.add(packet);

			}else{
#ifdef DEBUG_SKM_CONTROLLER
				System::logLn("Packet ID: %d ack timeouted, end of retransmition tries.", transmitingPacket.getHeader().id);
#endif
				packet.runOnFail();
			}

		}else{
			txedQueue.add(packet);
		}

	}

	if(rxQueue.size() > 0){
		SKMPacketRx p = rxQueue.shift();

		progressPacket(&p);

	}

}

void SKMController::onRx(SKMPacket::Type packetType, SKMOnRxCallback callback){
	SKMOnRxCallbackInfo i;
	i.type = packetType;
	i.callback = callback;
	onRxCallbackList.add(i);
}

void SKMController::send(SKMPacketTx* packet){
	if(!enabled)
		return;

	SKMPacket::Header cfg;
	cfg.senderAddress = nodeAddress;
	cfg.sourceAddress = nodeAddress;
	cfg.id = rand() % 0xFFFF;
	cfg.flags = 0x00;
	cfg.cryptoKey = 0;

	packet->config(cfg);

	txQueue.add(*packet);
#ifdef DEBUG_SKM_CONTROLLER
	System::logLn("Packet ID: %d, added to TXQueue!", packet->getHeader().id);
#endif
}

void SKMController::progressPacket(SKMPacketRx* packet){
	for (int q = 0; q < onRxCallbackList.size(); q++){
		onRxCallbackList[q].callback(packet);
	}
}

void SKMController::onTxRxFail(){
	if(!enabled)
		return;

	if(transmiting){
		if(transmitingPacket.canDoRetransmition()){
#ifdef DEBUG_SKM_CONTROLLER
			System::logLn("Fail to send packet ID: %d, push to retransmition!", transmitingPacket.getHeader().id);
#endif
			txQueue.add(transmitingPacket);

		}else{
#ifdef DEBUG_SKM_CONTROLLER
			System::logLn("Fail to send packet ID: %d, end of retransmition tries.", transmitingPacket.getHeader().id);
#endif
			transmitingPacket.runOnFail();
		}
		transmiting = false;
	}

	radio->setRx();
}

void SKMController::onTxDone(){
	if(!enabled)
		return;

	if(transmiting){
		transmiting = false;
		transmitingPacket.doTx();
		transmitingPacket.runOnSent();
		txedQueue.add(transmitingPacket);
#ifdef DEBUG_SKM_CONTROLLER
		System::logLn("Packet ID: %d, succesfull transmited.", transmitingPacket.getHeader().id);
#endif
	}

	radio->setRx();
}

void SKMController::onRxDone(){
	if(!enabled)
		return;

	SKMPacketRx *pack = radio->importAvalaiblePacket();

	if(pack != nullptr){
		if(pack->isValid()){
			rxQueue.add(*pack);
			#ifdef DEBUG_SKM_CONTROLLER
				System::logLn("Receive Packet ID: %d", pack->getHeader().id);
			#endif
		}
		delete pack;

	}

	radio->setRx();
}

