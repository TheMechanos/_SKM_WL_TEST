/*
 * RadioSX126X.cpp
 *
 *  Created on: 18 gru 2021
 *      Author: skocz
 */

#include "RadioSX126X.hpp"

namespace SKP2P{

	RadioSX126X::RadioSX126X(SX126x_Interface* interface) :
			SX126x(interface){

		isSleep = false;

		isReceiving=false;
		receivingStartTime = 0;

	}


	void RadioSX126X::init(){
		SX126x::init();
	}


	void RadioSX126X::iterate(){

	}


	void RadioSX126X::setSleep(){
		SleepConfig sc;
		sc.RTCWakeUp = false;
		sc.startType = SleepConfig::Start::Warm;
		setModeSleep(sc);
		isSleep = true;

	}

	void RadioSX126X::setIdle(){
		setModeSTDBY(SX126x::STDBY::RC);
		isSleep = false;
	}

	void RadioSX126X::setRx(){
		setConfigPacketSize();

		setModeRxCont();
		isSleep = false;
	}

	void RadioSX126X::setModeTxContinousWave(){
		SX126x::setModeTxContinousWave();
		isSleep = false;
	}

	bool RadioSX126X::sendPacket(PacketTx* packet){
		return transmit(packet->getTotalIdx(), packet->getTotalSize(), TRANSMITING_TIMEOUT);
	}

	PacketRx* RadioSX126X::importAvalaiblePacket(){
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

				PacketRx *packet = new PacketRx(buf, rxb.payloadLengthRx, packetStatus.GFSK.rssiAvg);

				return packet;
			}
		}
		return nullptr;
	}

	bool RadioSX126X::transmit(uint8_t* buffer, uint8_t size, uint32_t timeout){
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


	void RadioSX126X::irqTxCpltCallback(){
		isReceiving = false;
		listner->onTxDone();
		setRx();
	}

	void RadioSX126X::irqRxCpltCallback(){
		isReceiving = false;

		PacketRx *packet = importAvalaiblePacket();
		setRx();
		if(packet !=nullptr){
			listner->onRxDone(packet);
			delete packet;
		}
	}

	void RadioSX126X::irqRxTxTimeoutCallback(){
		isReceiving = false;
		listner->onTxRxFail();
		setRx();
	}

	void RadioSX126X::irqSyncWordValidCallback(){
		isReceiving = true;
		receivingStartTime = System::getTick();
	}

	void RadioSX126X::irqPreambleDetectedCallback(){}
	void RadioSX126X::irqCRCErrorCallback(){}



}
