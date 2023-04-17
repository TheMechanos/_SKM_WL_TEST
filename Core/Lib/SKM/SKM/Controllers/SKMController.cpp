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
	radio->registerListner(this);

}

void SKMController::enable(){
	enabled = true;
	//////////////////////radio->setRx();
}
void SKMController::disable(){
	enabled = false;
	///////////////////////////radio->setIdle();
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
				packet.doFailed();
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

	if(packet->getHeader().destinyAddress == nodeAddress){ //it is packet to me
		if(packet->getHeader().flags && SKMPacket::FLAG_ACK_PACKET){
			//ack packet
			for (int q = 0; q < txedQueue.size(); q++){
				if(txedQueue[q].getHeader().id == packet->getHeader().id){
					txedQueue.remove(q).doAcked();
				}
			}
			return;

		}else{
			//send ack to sender
			SKMPacketTx::TxConfig a;
			a.destinyAddress = packet->getHeader().sourceAddress;
			a.type = packet->getHeader().type;
			a.retransmitionCountMax = 0;

			SKMPacketTx ackPack = SKMPacketTx(nullptr, 0, a);

			SKMPacket::Header cfg;
			cfg.senderAddress = nodeAddress;
			cfg.sourceAddress = nodeAddress;
			cfg.id = packet->getHeader().id;
			cfg.flags = SKMPacket::FLAG_ACK_PACKET;
			cfg.cryptoKey = 0;

			ackPack.config(cfg);

			txQueue.add(ackPack);
		}


		for (int q = 0; q < onRxCallbackList.size(); q++){
				onRxCallbackList[q].callback(packet);
			}

	}


}

SKMPacketTx SKMController::getAckPacket(SKMPacketRx* rxPacket){

}

void SKMController::onTxRxFail(){

	SKMRadioSX126X* rad = static_cast<SKMRadioSX126X*>(radio);

	auto staus = rad->getStatus();


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
			transmitingPacket.doFailed();
		}
		transmiting = false;
	}

}

void SKMController::onTxDone(){
	if(!enabled)
		return;

	if(transmiting){
		transmiting = false;
		transmitingPacket.doTxed();
		txedQueue.add(transmitingPacket);
#ifdef DEBUG_SKM_CONTROLLER
		System::logLn("Packet ID: %d, succesfull transmited.", transmitingPacket.getHeader().id);
#endif
	}

}

void SKMController::onRxDone(){
	if(!enabled)
		return;

	SKMPacketRx *pack = radio->importAvalaiblePacket();

	if(pack != nullptr){

		pack->printInfo();
		pack->printAllHex();

		if(pack->isValid()){
			rxQueue.add(*pack);
#ifdef DEBUG_SKM_CONTROLLER
				System::logLn("Receive Packet ID: %d", pack->getHeader().id);
			#endif
		}
		delete pack;
	}

}

