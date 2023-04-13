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

void SKMPacket::printInfo(){
	System::logLn("-------------------- PACKET INFO --------------------");
	System::logLn("Address dest : 0x%08X", header.destinyAddress);
	System::logLn("Address src  : 0x%08X", header.sourceAddress);
	System::logLn("Address snd  : 0x%08X", header.senderAddress);

	System::logLn("Packet id    : %u", header.id);
	System::logLn("Packet flags : 0x%04X", header.flags);
	System::logLn("");
}

void SKMPacket::printAllHex(){

	char buf[128];
	size_t pos=0;

	System::logLn("-------------------- PACKET HEX HEADER --------------------");
	for (uint8_t q = 0; q < getHeaderSize(); q++){
		pos += sprintf(&buf[pos], "0x%02X ", header.idx[q]);
	}
	System::logLn("%s", buf);
	pos=0;


	System::logLn("--------------------- PACKET HEX DATA ---------------------");
	for (uint8_t q = 0; q < getDataSize(); q++){
		pos += sprintf(&buf[pos], "0x%02X ", data[q]);

		if(q % 8 == 0 && q > 0){
			pos=0;
			System::logLn("%s", buf);
		}
	}
	if(pos>0)
		System::logLn("%s", buf);

	printf("\n");

}

SKMPacketTx::~SKMPacketTx(){
	onFailList.clear();
	onSentList.clear();
	onAckList.clear();
}

SKMPacketTx::SKMPacketTx() :
		SKMPacket(){

}

SKMPacketTx::SKMPacketTx(uint8_t* dataValues, size_t sizeData, TxConfig packetTxConfig){
	if(sizeData > SizeDataMax)
		return;

	if(sizeData <= SizeDataMax)
		memcpy(data, dataValues, sizeData);

	this->dataSize = sizeData;

	this->txConfig = packetTxConfig;
}

bool SKMPacketTx::isAckTimeout(){
	return System::getTick() - txTime > txConfig.ackTimout;
}

bool SKMPacketTx::canDoRetransmition(){
	if(retransmitionCount + 2 > txConfig.retransmitionCountMax){
		return false;
	}
	retransmitionCount++;
	return true;
}

void SKMPacketTx::config(Header newHeader){
	this->header = newHeader;

	this->header.destinyAddress = txConfig.destinyAddress;
	this->header.type = txConfig.type;
	this->header.signature = SKM_SIGNATURE;
}

void SKMPacketTx::doTx(){
	this->txTime = System::getTick();
}

uint32_t SKMPacketTx::getTxTime(){
	return txTime;
}

void SKMPacketTx::onFail(PacketCallback onFail){
	onFailList.add(onFail);
}
void SKMPacketTx::onSent(PacketCallback onSent){
	onSentList.add(onSent);
}
void SKMPacketTx::onAck(PacketCallback onAck){
	onAckList.add(onAck);
}

void SKMPacketTx::runOnFail(){
	for (int q = 0; q < onFailList.size(); q++)
		onFailList[q](this);
}
void SKMPacketTx::runOnSent(){
	for (int q = 0; q < onSentList.size(); q++)
		onSentList[q](this);
}
void SKMPacketTx::runOnAck(){
	for (int q = 0; q < onAckList.size(); q++)
		onAckList[q](this);
}

SKMPacketRx::~SKMPacketRx(){
}

SKMPacketRx::SKMPacketRx() :
		SKMPacket(){
}

SKMPacketRx::SKMPacketRx(uint8_t* dataAll, size_t sizeAll){
	if(sizeAll <= SizePacketMax)
		memcpy(idx, dataAll, sizeAll);
	dataSize = sizeAll - SizeHeader;
	this->rxTime = System::getTick();
}

uint32_t SKMPacketRx::getRxTime(){
	return rxTime;
}

bool SKMPacketRx::isValid(){
	return header.signature == SKM_SIGNATURE;
}
