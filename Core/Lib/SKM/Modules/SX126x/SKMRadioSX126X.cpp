/*
 * SKMRadioSX126X.cpp
 *
 *  Created on: 18 gru 2021
 *      Author: skocz
 */

#include "SKMRadioSX126X.hpp"

SKMRadioSX126X::SKMRadioSX126X(SKM_SX126x_Interface* interface) :
		SX126x(interface){

	isSleep = false;

	isReceiving=false;
	receivingStartTime = 0;

}


void SKMRadioSX126X::init(){
	SX126x::init();
}


void SKMRadioSX126X::iterate(){

}


void SKMRadioSX126X::setSleep(){
	SleepConfig sc;
	sc.RTCWakeUp = false;
	sc.startType = SleepConfig::Start::Warm;
	setModeSleep(sc);
	isSleep = true;

}

void SKMRadioSX126X::setIdle(){
	setModeSTDBY(SX126x::STDBY::RC);
	isSleep = false;
}

void SKMRadioSX126X::setRx(){
	setConfigPacketSize();

	setModeRxCont();
	isSleep = false;
}

void SKMRadioSX126X::setModeTxContinousWave(){
	SX126x::setModeTxContinousWave();
	isSleep = false;
}

bool SKMRadioSX126X::sendPacket(SKMPacketTx* packet){
	return transmit(packet->getTotalIdx(), packet->getTotalSize(), TRANSMITING_TIMEOUT);
}

SKMPacketRx* SKMRadioSX126X::importAvalaiblePacket(){
	if(getStatus().radio == Status::Radio::DataAvailable){

		PacketStatus packetStatus = getPacketStatus();
		if(packetStatus.GFSK.Status.abortError == false &&
				packetStatus.GFSK.Status.addressError == false &&
				packetStatus.GFSK.Status.crcError == false &&
				packetStatus.GFSK.Status.lengthError == false &&
				packetStatus.GFSK.Status.preambleError == false &&
				packetStatus.GFSK.Status.syncError == false){

			RxBufferState rxb = getRxBufferStatus();

			uint8_t buf[rxb.payloadLengthRx];
			readBuffer(rxb.rxStartBufferPointer, buf, rxb.payloadLengthRx);

			SKMPacketRx *packet = new SKMPacketRx(buf, rxb.payloadLengthRx, packetStatus.GFSK.rssiAvg);

			return packet;
		}
	}
	return nullptr;
}

bool SKMRadioSX126X::transmit(uint8_t* buffer, uint8_t size, uint32_t timeout){
	if(isReceiving){
		if(System::getTick() > receivingStartTime + RECEIVING_TIMEOUT)
			isReceiving=false;
		else
			return false;
	}

	auto st = getStatus().mode;
	if(st == Status::Mode::FS || st == Status::Mode::RX || st == Status::Mode::STDBY_RC || st == Status::Mode::STDBY_XOSC){

		setModeSTDBY();

		setPacketSize(size);
		writeBuffer(0, buffer, size);

		setModeTx(timeout * 1000);
		return true;
	}
	return false;
}


void SKMRadioSX126X::irqTxCpltCallback(){
	isReceiving = false;
	listner->onTxDone();
	setRx();
}

void SKMRadioSX126X::irqRxCpltCallback(){
	isReceiving = false;

	SKMPacketRx *packet = importAvalaiblePacket();
	setRx();
	if(packet !=nullptr){
		listner->onRxDone(packet);
		delete packet;
	}
}

void SKMRadioSX126X::irqRxTxTimeoutCallback(){
	isReceiving = false;
	listner->onTxRxFail();
	setRx();
}

void SKMRadioSX126X::irqSyncWordValidCallback(){
	isReceiving = true;
	receivingStartTime = System::getTick();
}

void SKMRadioSX126X::irqPreambleDetectedCallback(){}
void SKMRadioSX126X::irqCRCErrorCallback(){}


/*
void dbgON(){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);

}
void dbgOFF(){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
}
*/
