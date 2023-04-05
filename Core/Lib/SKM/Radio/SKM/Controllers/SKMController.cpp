/*
 * SKMController.cpp
 *
 *  Created on: 16 lut 2022
 *      Author: skocz
 */

#include <Radio/SKM/Controllers/SKMController.hpp>

SKMController::SKMController(){

}

SKMController::SKMController(SKMRadio* radio){
	this->radio = radio;

	this->actualSendingPacketIndex = -1;

	SKMController();
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

void SKMController::progressPacket(SKMPacket* packet){

	if(onRxFunction){
		onRxFunction(packet);
	}

}


void SKMController::init(){
	enable();
	radio->registerListner(this);

	packetStatus.setStorage(arrayPacketStatus, SKM_PACKET_STATUS_QUEUE_SIZE, 0);
	txQueue.setStorage(arrayTxQueue, SKM_PACKET_TX_QUEUE_SIZE, 0);
	rxQueue.setStorage(arrayRxQueue, SKM_PACKET_TX_QUEUE_SIZE, 0);

}

void SKMController::iterate(){
	if(!isEnabled())return;


	if(txQueue.size() > 0){

		auto st = radio->getState();
		if(st == SKMRadio::State::Rx || st == SKMRadio::State::Idle){ //Jeśli radio może nadawać

			for (uint8_t q = 0; q < txQueue.size(); q++){ //Po wszystkich pakietach z kolejki
				SKMPacketStatus* stat = getPacketStatus(&txQueue[q]);

				if(stat != 0){ //Jeśli status pakietu istnieje
					if(stat->status == SKMPacketStatus::Status::InTxQueue){//Jeśli pakiet czeka na nadanie

						stat->status = SKMPacketStatus::Status::Transmitting;
						actualSendingPacketIndex = q;

						radio->sendPacket(&txQueue[q]);

						System::logLn("Packet id: %d writted to send!", txQueue[q].header.id);

						break;//Wyjdz z pętli
					}
				}
			}
		}

	}

	if(rxQueue.size() > 0){
		SKMPacket p = rxQueue.back();
		rxQueue.pop_back();

		progressPacket(&p);

		System::logLn("Received packet ID: %d, was procedded!",p.header.id, p.getDataSize());

	}
}

SKMPacketStatus* SKMController::getPacketStatus(SKMPacket* source){
	for(uint8_t q=0;q<packetStatus.size();q++){
		if(packetStatus[q].id == source->header.id){
			return &packetStatus[q];
		}
	}
	return 0;
}

void SKMController::send(SKMPacket* packet){
	if(!enabled)return;

	//Jesli status pełny
	if(packetStatus.full()){
		packetStatus.remove(0);
		System::logLn("Status Queue full, delete last item!");
	}
	//Dodaj status do listy

	SKMPacketStatus stat(packet->header.id, SKMPacketStatus::Direction::Transmitting);
	stat.status = SKMPacketStatus::Status::InTxQueue;
	packetStatus.push_back(stat);
	System::logLn("Packet ID: %d, added to TXQueue!",packet->header.id);

	//Dodaj do kolejki wysyłkowej
	txQueue.push_back(*packet);
	System::logLn("TXQueue size: %d",txQueue.size());


}

void SKMController::onTxRxFail(){
	if(!enabled)return;

	if(actualSendingPacketIndex >= 0){

		SKMPacketStatus* status = getPacketStatus(&txQueue[actualSendingPacketIndex]);
		if(status){
			status->status = SKMPacketStatus::Status::TxFail;
			System::logLn("Fail to send packet ID: %d",status->id);

			txQueue.remove(actualSendingPacketIndex);
		}

	}
	actualSendingPacketIndex = -1;

	radio->setRx();
}

void SKMController::onTxDone(){
	if(!enabled)return;

	if(actualSendingPacketIndex >= 0){
		SKMPacketStatus* status = getPacketStatus(&txQueue[actualSendingPacketIndex]);
		if(status){
			status->status = SKMPacketStatus::Status::Transmitted;
			status->sendedTime = System::getTick();
			System::logLn("Packet ID: %d succesfull transmitted at time: %d",status->id, status->sendedTime);

			txQueue.remove(actualSendingPacketIndex);
		}

	}
	actualSendingPacketIndex = -1;
	//dodać sortowan
	radio->setRx();
}

void SKMController::onRxDone(){
	if(!enabled)return;

	SKMPacket pack;
	radio->importAvalaiblePacket(&pack);
	rxQueue.push_back(pack);

	System::logLn("Receive Packet ID: %d",pack.header.id);

	radio->setRx();
}

