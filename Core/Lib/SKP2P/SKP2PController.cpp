/*
 * Controller.cpp
 *
 *  Created on: 16 lut 2022
 *      Author: skocz
 */

#include "SKP2PController.hpp"

namespace SKP2P{

	Controller::Controller(){
		transmitingPacket = nullptr;
		nextTxTime = 0;
	}

	Controller::Controller(Radio* radio, Packet::Address nodeAddress) :
			Controller(){
		this->radio = radio;
		this->nodeAddress = (nodeAddress == Packet::ADDRESS_BROADCAST) ? nodeAddress - 1 : nodeAddress;
	}

	void Controller::init(){
		radio->registerListner(this);
		generateNextTxRandomTime();
	}

	void Controller::iterate(){

		if(rxQueue.size() > 0){
			PacketRx p = rxQueue.shift();
			progressPacket(&p);

		}

		if(txQueue.size() > 0){
			if(System::getTick() >= nextTxTime){
				PacketTx packet = txQueue.shift();

				bool sended = radio->sendPacket(&packet);
				if(sended){
					SKMLogLn("Packet ID: %u was push to send.", packet.getHeader().id);
					if(transmitingPacket != nullptr)
						delete transmitingPacket;
					transmitingPacket = new PacketTx(packet);
				}else{
					SKMLogLn("At this moment can't push packet ID: %u to send!", packet.getHeader().id);
					txQueue.add(0, packet);
				}
				generateNextTxRandomTime();
			}
		}

		if(txedQueue.size() > 0){
			auto packet = txedQueue.shift();

			if(packet.isAckTimeout()){
				/*if(packet.isBroadcastPacket()){
					SKMLogLn("Broadcast Packet ID: %u has end of life time!", transmitingPacket->getHeader().id);

				}else*/ if(packet.canDoRetransmition()){
					SKMLogLn("Packet ID: %u ack timeouted, push to retransmition!", transmitingPacket->getHeader().id);
					txQueue.add(packet);

				}else{
					SKMLogLn("Packet ID: %u ack timeouted, end of retransmition tries.", transmitingPacket->getHeader().id);
					packet.doFailed();

				}

			}else{
				txedQueue.add(packet);
			}
		}

	}

	void Controller::onRx(Packet::Type packetType, OnRxCallback callback){
		OnRxCallbackInfo i;
		i.type = packetType;
		i.callback = callback;
		onRxCallbackList.add(i);
	}

	void Controller::transmit(PacketTx* packet){
		if(packet->getHeader().id == 0)
			packet->configId(System::getRandomNumber() % 0xFFFF);

		packet->configSourceAddress(nodeAddress);

		txQueue.add(*packet);
		SKMLogLn("Packet ID: %u, added to TXQueue!", packet->getHeader().id);
	}

	void Controller::progressPacket(PacketRx* packet){

		SKMLogLn("Progressing Packet ID: %u", packet->getHeader().id);

		if(packet->isAckPacket()){
			for (int q = 0; q < txedQueue.size(); q++){
				if(txedQueue[q].getHeader().id == packet->getHeader().id){
					txedQueue[q].doAcked();
					//if(!txedQueue[q].isBroadcastPacket())
					txedQueue.remove(q);
				}
			}
			return;

		}else{
			//auto ackPack = generateAckPacket(packet);

			PacketTx::TxConfig txConfig;

			txConfig.type = packet->getHeader().type;
			txConfig.destinyAddress = packet->getHeader().sourceAddress;
			txConfig.ackTimout = 0;
			txConfig.retransmitionCountMax = 0;

			PacketTx ackPack = PacketTx(nullptr, 0, txConfig, Packet::FLAG_ACK_PACKET);
			ackPack.configId(packet->getHeader().id);

			SKMLogLn("Ack generated Packet ID: %u", ackPack.getHeader().id);
			ackPack.printInfo();

			transmit(&ackPack);
		}

		if(packet->getHeader().flags == 0x00){
			for (int q = 0; q < onRxCallbackList.size(); q++){
				onRxCallbackList[q].callback(packet);
			}
		}

	}

	void Controller::onTxRxFail(){

		if(transmitingPacket != nullptr){
			if(transmitingPacket->canDoRetransmition()){
				SKMLogLn("Fail to send packet ID: %u, push to retransmition!", transmitingPacket->getHeader().id);
				txQueue.add(*transmitingPacket);
			}else{
				SKMLogLn("Fail to send packet ID: %u, end of retransmition tries.", transmitingPacket->getHeader().id);
				transmitingPacket->doFailed();
			}
			delete transmitingPacket;
		}
	}

	void Controller::onTxDone(){

		if(transmitingPacket != nullptr){
			transmitingPacket->doTxed();
			if(!transmitingPacket->isAckPacket()){
				txedQueue.add(*transmitingPacket);
			}
			SKMLogLn("Packet ID: %u, succesfull transmited.", transmitingPacket->getHeader().id);
			delete transmitingPacket;
		}
	}

	void Controller::onRxDone(PacketRx* packet){
		if(packet->isValid()){
			if(packet->isForMe(nodeAddress)){
				SKMLogLn("Receive Packet ID: %u", packet->getHeader().id);
				rxQueue.add(*packet);
			}
		}

	}

	void Controller::generateNextTxRandomTime(){
		nextTxTime = System::getTick() + (System::getRandomNumber() % TRANSMIT_RANDOM_TIMEOUT_MAX) + 50;
	}

	int Controller::SKMLogLn(const char* format, ...){

		va_list arg;
		int done = 0;

		va_start(arg, format);
		if(LOG_ENABLE)
			done += System::logLn(format, arg);
		va_end(arg);

		return done;
	}

}
