/*
 * SKMRadioSX126X.cpp
 *
 *  Created on: 18 gru 2021
 *      Author: skocz
 */

#include "SKMRadioSX126X.hpp"

SKMRadioSX126X::SKMRadioSX126X(SKM_SX126x_Interface* interface) :
		SX126x(interface){

	txTimeout = 100;

	isSleep = false;

}

void SKMRadioSX126X::init(){
	SX126x::init();
}

void SKMRadioSX126X::configTxTimeout(uint16_t msTxTimeout){
	this->txTimeout = msTxTimeout;
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
	setModeRxCont();
	isSleep = false;
}

void SKMRadioSX126X::setModeTxContinousWave(){
	SX126x::setModeTxContinousWave();
	isSleep = false;
}

/*
 SKMRadioSX126X::State SKMRadioSX126X::getState(){//Sleep, Busy, Idle, Rx, Tx
 SX126x::Status st = getStatus();

 if(isSleep){
 return State::Sleep;
 }

 if(st.mode == SX126x::Status::Mode::RX){
 return State::Rx;
 }
 if(st.mode == SX126x::Status::Mode::TX){
 return State::Tx;
 }
 if(st.mode == SX126x::Status::Mode::STDBY_RC || st.mode == SX126x::Status::Mode::STDBY_XOSC){
 return State::Idle;
 }
 return State::Idle;
 }**/

bool SKMRadioSX126X::sendPacket(SKMPacketTx* packet){
	return transmitPacket(packet, txTimeout);
}

SKMPacketRx* SKMRadioSX126X::importAvalaiblePacket(){
	if(getStatus().radio == Status::Radio::DataAvailable){

		PacketStatus packetStatus = getPacketStatus();
		if(packetStatus.GFSK.Status.crcError == false){

			RxBufferState rxb = getRxBufferStatus();

			uint8_t buf[rxb.payloadLengthRx];
			readBuffer(rxb.rxStartBufferPointer, buf, rxb.payloadLengthRx);

			SKMPacketRx *packet = new SKMPacketRx(buf, rxb.payloadLengthRx);

			return packet;
		}
	}
	return nullptr;
}

void SKMRadioSX126X::iterate(){

}

bool SKMRadioSX126X::transmitPacket(SKMPacketTx* packet, uint32_t timeout){
	return transmit(packet->getTotalIdx(), packet->getTotalSize(), timeout);
}

uint8_t SKMRadioSX126X::receivePacket(SKMPacketRx* packet, uint32_t timeout){
	/*uint8_t data[255];
	 uint8_t len = receive(data, packet->getPacketMaxLength(), timeout);

	 packet->setTotalSize(len);

	 for(uint8_t q=0;q<len;q++){
	 packet->idx[q] = data[q];
	 }
	 return len - SKMPacket::SizeHeader;*/
}

bool SKMRadioSX126X::transmit(uint8_t* buffer, uint8_t size, uint32_t timeout){

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

uint8_t SKMRadioSX126X::receive(uint8_t* buffer, uint8_t size, uint32_t timeout){

	if(conf->packetParams.GFSK.packetLength == PacketParams::PacketLength::Const){
		setPacketSize(size);
	}else{
		setConfigPacketSize();
	}

	setModeRx(timeout * 1000);

	uint32_t endTick = System::getTick() + timeout;
	while(1){
		if(System::getTick() > endTick){
			return 0;
		}

		Status s = getStatus();

		if(s.radio == Status::Radio::DataAvailable){
			PacketStatus packetStatus = getPacketStatus();

			if(packetStatus.GFSK.Status.crcError == false){
				break;
			}else{
				return 0;
			}

		}

	}
	RxBufferState rxb = getRxBufferStatus();

	if(rxb.payloadLengthRx > size){
		return 0;
	}

	readBuffer(rxb.rxStartBufferPointer, buffer, rxb.payloadLengthRx);

	return rxb.payloadLengthRx;

}

void SKMRadioSX126X::irqTxCpltCallback(){
	listner->onTxDone();
	setRx();
}

void SKMRadioSX126X::irqRxCpltCallback(){
	listner->onRxDone();
	setRx();
}

void SKMRadioSX126X::irqPreambleDetectedCallback(){
}

void SKMRadioSX126X::irqSyncWordValidCallback(){
}

void SKMRadioSX126X::irqCRCErrorCallback(){
	listner->onTxRxFail();
	setRx();
}

void SKMRadioSX126X::irqRxTxTimeoutCallback(){
	listner->onTxRxFail();
	setRx();
}
