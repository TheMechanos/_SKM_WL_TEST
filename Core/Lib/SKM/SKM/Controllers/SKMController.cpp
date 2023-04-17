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

	if(rxQueue.size() > 0){
		SKMPacketRx p = rxQueue.shift();
		progressPacket(&p);

	}


	if(txQueue.size() > 0){
		SKMPacketTx packet = txQueue.shift();
		bool sended = radio->sendPacket(&packet);
		if(sended){

			SKMLogLn("Packet ID: %d was push to send.", packet.getHeader().id);

			transmitingPacket = packet;
			transmiting = true;
		}else{

			SKMLogLn("At this moment can't push packet ID: %d to send!", packet.getHeader().id);

			txQueue.add(0, packet);
		}
	}

	if(txedQueue.size() > 0){
		auto packet = txedQueue.shift();

		if(packet.isAckTimeout()){

			if(packet.canDoRetransmition()){

				SKMLogLn("Packet ID: %d ack timeouted, push to retransmition!", transmitingPacket.getHeader().id);

				txQueue.add(packet);

			}else{

				SKMLogLn("Packet ID: %d ack timeouted, end of retransmition tries.", transmitingPacket.getHeader().id);

				packet.doFailed();
			}

		}else{
			txedQueue.add(packet);
		}

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

	SKMLogLn("Packet ID: %d, added to TXQueue!", packet->getHeader().id);

}

void SKMController::progressPacket(SKMPacketRx* packet){

	if(packet->getHeader().destinyAddress == nodeAddress){ //it is packet to me
		if(packet->isAckPacket()){
			//ack packet
			for (int q = 0; q < txedQueue.size(); q++){
				if(txedQueue[q].getHeader().id == packet->getHeader().id){
					txedQueue.remove(q).doAcked();
				}
			}
			return;

		}else{
			txQueue.add(packet->generateAckPacket());
		}

		for (int q = 0; q < onRxCallbackList.size(); q++){
			onRxCallbackList[q].callback(packet);
		}

	}

}


void SKMController::onTxRxFail(){
	if(!enabled)
		return;

	if(transmiting){
		if(transmitingPacket.canDoRetransmition()){

			SKMLogLn("Fail to send packet ID: %d, push to retransmition!", transmitingPacket.getHeader().id);

			txQueue.add(transmitingPacket);

		}else{

			SKMLogLn("Fail to send packet ID: %d, end of retransmition tries.", transmitingPacket.getHeader().id);

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
		if(!transmitingPacket.isAckPacket()){
			txedQueue.add(transmitingPacket);
		}

		SKMLogLn("Packet ID: %d, succesfull transmited.", transmitingPacket.getHeader().id);

	}

}

void SKMController::onRxDone(){
	if(!enabled)
		return;

	SKMPacketRx *pack = radio->importAvalaiblePacket();

	if(pack != nullptr){
		if(pack->isValid()){
			rxQueue.add(*pack);
			SKMLogLn("Receive Packet ID: %d", pack->getHeader().id);
		}
		delete pack;
	}

}

int SKMController::SKMLogLn(const char* format, ...){
	va_list arg;
	int done = 0;

	va_start(arg, format);
	if(LOG_ENABLE)
		done += System::logLn(format, arg);
	va_end(arg);

	return done;
}

