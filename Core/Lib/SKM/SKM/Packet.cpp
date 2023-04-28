/*
 * Packet.cpp
 *
 *  Created on: 13 kwi 2023
 *      Author: skocz
 */

#include "Packet.hpp"

SKMPacket::~SKMPacket(){
	memset(idx, 0x00, SizePacketMax);

}
SKMPacket::SKMPacket(){
	memset(idx, 0x00, SizePacketMax);
}

SKMPacket::Header SKMPacket::getHeader(){
	return header;
}
SKMPacket::PacketData* SKMPacket::getData(){
	return data;
}
uint8_t* SKMPacket::getTotalIdx(){
	return idx;
}

uint8_t SKMPacket::getTotalSize(){
	return dataSize + SizeHeader;
}
uint8_t SKMPacket::getDataSize(){
	return dataSize;
}
uint8_t SKMPacket::getHeaderSize(){
	return SizeHeader;
}

uint8_t SKMPacket::getMaxDataSize(){
	return SizeDataMax;
}

bool SKMPacket::isAckPacket(){
	return header.flags && FLAG_ACK_PACKET;
}
bool SKMPacket::isBroadcastPacket(){
	return header.destinyAddress == ADDRESS_BROADCAST && header.destinyAddress == ADDRESS_BROADCAST;
}

bool SKMPacket::isForMe(Address myAddress){
	return header.destinyAddress == myAddress || header.destinyAddress == ADDRESS_BROADCAST;
}

void SKMPacket::printInfo(){
	System::logLn("-----------------------------------------------------");
	System::logLn("-------------------- PACKET INFO --------------------");
	System::logLn("Address dest : 0x%08X", header.destinyAddress);
	System::logLn("Address src  : 0x%08X", header.sourceAddress);
	System::logLn("Address snd  : 0x%08X", header.senderAddress);

	System::logLn("Packet id    : %u", header.id);
	System::logLn("Packet flags : 0x%04X", header.flags);
	System::logLn("-----------------------------------------------------");
	System::logLn("-----------------------------------------------------\n");
}

void SKMPacket::printAllHex(){

	char buf[128];
	size_t pos = 0;

	System::logLn("-----------------------------------------------------------");
	System::logLn("-------------------- PACKET HEX HEADER --------------------");
	for (uint8_t q = 0; q < getHeaderSize(); q++){
		pos += sprintf(&buf[pos], "0x%02X ", header.idx[q]);
	}
	System::logLn("%s", buf);
	pos = 0;

	System::logLn("--------------------- PACKET HEX DATA ---------------------");
	for (uint8_t q = 0; q < getDataSize(); q++){
		pos += sprintf(&buf[pos], "0x%02X ", data[q]);

		if(q % 8 == 0 && q > 0){
			pos = 0;
			System::logLn("%s", buf);
		}
	}
	if(pos > 0)
		System::logLn("%s", buf);

	System::logLn("-----------------------------------------------------------");
	System::logLn("-----------------------------------------------------------\n");

}

//-----------------------------------------------------------------------------------------------------------------

SKMPacketTx::~SKMPacketTx(){


}

SKMPacketTx::SKMPacketTx() :
		SKMPacket(){

	retransmitionCount = 0;
	ackTime = 0;
	txTime = 0;

	onTxCallback = nullptr;
	onAckCallback = nullptr;
	onFailCallback = nullptr;

}

SKMPacketTx::SKMPacketTx(uint8_t* dataValues, size_t sizeData, TxConfig packetTxConfig) :
		SKMPacketTx(){
	if(sizeData > SizeDataMax)
		return;

	if(sizeData <= SizeDataMax)
		memcpy(data, dataValues, sizeData);

	this->dataSize = sizeData;

	this->txConfig = packetTxConfig;
	this->header.destinyAddress = txConfig.destinyAddress;
	this->header.receiverAddress = txConfig.destinyAddress;
	this->header.type = txConfig.type;
	this->header.signature = SKM_SIGNATURE;
}

bool SKMPacketTx::isAckTimeout(){
	return System::getTick() - txTime > txConfig.ackTimout;
}

bool SKMPacketTx::canDoRetransmition(){
	if(retransmitionCount >= txConfig.retransmitionCountMax){
		return false;
	}
	retransmitionCount++;
	return true;
}


void SKMPacketTx::doTxed(){
	this->txTime = System::getTick();
	if(onTxCallback != nullptr)
		onTxCallback(this);
}

void SKMPacketTx::doFailed(){
	if(onFailCallback != nullptr)
		onFailCallback(this);

}
void SKMPacketTx::doAcked(){
	ackTime = System::getTick();
	if(onAckCallback != nullptr)
		onAckCallback(this);
}

uint32_t SKMPacketTx::getTxTime(){
	return txTime;
}
uint32_t SKMPacketTx::getAckTime(){
	return ackTime;
}
uint32_t SKMPacketTx::getInFlightTime(){
	return ackTime - txTime;
}

void SKMPacketTx::onFail(PacketCallback onFailCallback){
	this->onFailCallback = onFailCallback;
}
void SKMPacketTx::onTx(PacketCallback onSentCallback){
	this->onTxCallback = onSentCallback;
}
void SKMPacketTx::onAck(PacketCallback onAckCallback){
	this->onAckCallback = onAckCallback;
}

//-----------------------------------------------------------------------------------------------------------------

SKMPacketRx::~SKMPacketRx(){
}

SKMPacketRx::SKMPacketRx() :
		SKMPacket(){
}

SKMPacketRx::SKMPacketRx(uint8_t* dataAll, size_t sizeAll, int8_t rssi){
	if(sizeAll <= SizePacketMax)
		memcpy(idx, dataAll, sizeAll);
	dataSize = sizeAll - SizeHeader;
	this->rxTime = System::getTick();
	this->rssi = rssi;
}

uint32_t SKMPacketRx::getRxTime(){
	return rxTime;
}

bool SKMPacketRx::isValid(){
	return header.signature == SKM_SIGNATURE;
}

int32_t SKMPacketRx::getdBm(){
	return rssi/2;//(0-rssi)/2;
}
