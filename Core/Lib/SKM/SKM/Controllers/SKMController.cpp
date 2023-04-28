/*
 * SKMController.cpp
 *
 *  Created on: 16 lut 2022
 *      Author: skocz
 */

#include "SKMController.hpp"

SKMController::SKMController(){
	transmitingPacket = nullptr;
	nextTxTime = 0;

}

SKMController::SKMController(SKMRadio* radio, SKMPacket::Address nodeAddress) :
		SKMController(){
	this->radio = radio;
	this->nodeAddress = (nodeAddress == SKMPacket::ADDRESS_BROADCAST) ? nodeAddress - 1 : nodeAddress;
}

void SKMController::init(){
	radio->registerListner(this);
	generateNextTxRandomTime();
}

void SKMController::iterate(){

	if(rxQueue.size() > 0){
		SKMPacketRx p = rxQueue.shift();
		progressPacket(&p);
	}

	if(txQueue.size() > 0){
		if(System::getTick() >= nextTxTime){
			SKMPacketTx packet = txQueue.shift();
			bool prepared = preparePacketToSend(&packet);
			if(prepared){
				bool sended = radio->sendPacket(&packet);
				if(sended){
					SKMLogLn("Packet ID: %d was push to send.", packet.getHeader().id);
					if(transmitingPacket != nullptr)
						delete transmitingPacket;
					transmitingPacket = new SKMPacketTx(packet);
				}else{
					SKMLogLn("At this moment can't push packet ID: %d to send!", packet.getHeader().id);
					txQueue.add(0, packet);
				}
				generateNextTxRandomTime();
			}else{
				SKMLogLn("At this moment packet ID: %d is not ready to send!", packet.getHeader().id);
				txQueue.add(0, packet);
			}

		}
	}

	if(txedQueue.size() > 0){
		auto packet = txedQueue.shift();

		if(packet.isAckTimeout()){
			if(packet.isBroadcastPacket()){
				SKMLogLn("Broadcast Packet ID: %d has end of life time!", transmitingPacket->getHeader().id);

			}else if(packet.canDoRetransmition()){
				SKMLogLn("Packet ID: %d ack timeouted, push to retransmition!", transmitingPacket->getHeader().id);
				txQueue.add(packet);

			}else{
				SKMLogLn("Packet ID: %d ack timeouted, end of retransmition tries.", transmitingPacket->getHeader().id);
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

bool SKMController::preparePacketToSend(SKMPacketTx* packet){

	/*for (int q = 0; q < routing.size(); q++){
		if(routing[q].destiny == packet->header.destinyAddress){
			SKMLogLn("Have valid route to 0x%08X", packet->header.destinyAddress);
		}else{
			SKMPacketTx::TxConfig txConfig;

			txConfig.type = 0;
			txConfig.ackTimout = 0;
			txConfig.retransmitionCountMax = 3;
			txConfig.destinyAddress = packet->header.destinyAddress;

			SKMPacketTx ackPack = SKMPacketTx(nullptr, 0, txConfig);

			ackPack.header.sourceAddress = rxPacket->header.destinyAddress;
			ackPack.header.id = rxPacket->header.id;
			ackPack.header.flags |= SKMPacket::FLAG_ACK_PACKET;

			return ackPack;
		}
	}*/

	packet->header.senderAddress = nodeAddress;
	packet->header.sourceAddress = nodeAddress;


	return true;

}
void SKMController::transmit(SKMPacketTx* packet){
	packet->header.id = System::getRandomNumber() % 0xFFFF;
	packet->header.senderAddress = nodeAddress;

	txQueue.add(*packet);
	SKMLogLn("Packet ID: %d, added to TXQueue!", packet->getHeader().id);
}

void SKMController::progressPacket(SKMPacketRx* packet){

	if(packet->isForMe(nodeAddress)){
		if(packet->isAckPacket()){
			for (int q = 0; q < txedQueue.size(); q++){
				if(txedQueue[q].getHeader().id == packet->getHeader().id){
					txedQueue[q].doAcked();
					if(!txedQueue[q].isBroadcastPacket())
						txedQueue.remove(q);
				}
			}
			return;

		}else{
			auto ackPack = generateAckPacket(packet);
			transmit(&ackPack);
		}

		if(packet->getHeader().flags == 0x00){
			for (int q = 0; q < onRxCallbackList.size(); q++){
				onRxCallbackList[q].callback(packet);
			}
		}
	}

}

void SKMController::onTxRxFail(){

	if(transmitingPacket != nullptr){
		if(transmitingPacket->canDoRetransmition()){
			SKMLogLn("Fail to send packet ID: %d, push to retransmition!", transmitingPacket->getHeader().id);
			txQueue.add(*transmitingPacket);
		}else{
			SKMLogLn("Fail to send packet ID: %d, end of retransmition tries.", transmitingPacket->getHeader().id);
			transmitingPacket->doFailed();
		}
		delete transmitingPacket;
	}
}

void SKMController::onTxDone(){

	if(transmitingPacket != nullptr){
		transmitingPacket->doTxed();
		if(!transmitingPacket->isAckPacket()){
			txedQueue.add(*transmitingPacket);
		}
		SKMLogLn("Packet ID: %d, succesfull transmited.", transmitingPacket->getHeader().id);
		delete transmitingPacket;
	}
}

void SKMController::onRxDone(SKMPacketRx* packet){

	if(packet->isValid()){
		rxQueue.add(*packet);

		if(rxedQueue.size()>RXED_QUEUE_MAX_LENGTH){
			rxedQueue.shift();
		}
		rxedQueue.add(*packet);
	}

}

void SKMController::generateNextTxRandomTime(){
	nextTxTime = System::getTick() + (System::getRandomNumber() % TRANSMIT_RANDOM_TIMEOUT_MAX);
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

SKMPacketTx SKMController::generateAckPacket(SKMPacketRx* rxPacket){
	SKMPacketTx::TxConfig txConfig;

	txConfig.type = rxPacket->header.type;
	txConfig.ackTimout = 0;
	txConfig.retransmitionCountMax = 0;
	txConfig.destinyAddress = rxPacket->header.sourceAddress;

	SKMPacketTx ackPack = SKMPacketTx(nullptr, 0, txConfig);

	ackPack.header.sourceAddress = rxPacket->header.destinyAddress;
	ackPack.header.id = rxPacket->header.id;
	ackPack.header.flags |= SKMPacket::FLAG_ACK_PACKET;

	return ackPack;
}

