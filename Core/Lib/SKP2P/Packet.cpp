/*
 * Packet.cpp
 *
 *  Created on: 13 kwi 2023
 *      Author: skocz
 */

#include "Packet.hpp"

namespace SKP2P{

	Packet::~Packet(){
		memset(idx, 0x00, SizePacketMax);

	}
	Packet::Packet(){
		memset(idx, 0x00, SizePacketMax);
	}

	Packet::Header Packet::getHeader(){
		return header;
	}
	Packet::PacketData* Packet::getData(){
		return data;
	}
	uint8_t* Packet::getTotalIdx(){
		return idx;
	}

	uint8_t Packet::getTotalSize(){
		return dataSize + SizeHeader;
	}
	uint8_t Packet::getDataSize(){
		return dataSize;
	}
	uint8_t Packet::getHeaderSize(){
		return SizeHeader;
	}

	uint8_t Packet::getMaxDataSize(){
		return SizeDataMax;
	}

	bool Packet::isAckPacket(){
		return header.flags && FLAG_ACK_PACKET;
	}
	bool Packet::isBroadcastPacket(){
		return header.destinyAddress == ADDRESS_BROADCAST && header.destinyAddress == ADDRESS_BROADCAST;
	}

	bool Packet::isForMe(Address myAddress){
		return header.destinyAddress == myAddress || header.destinyAddress == ADDRESS_BROADCAST;
	}

	void Packet::printInfo(){
		System::logLn("-----------------------------------------------------");
		System::logLn("-------------------- PACKET INFO --------------------");
		System::logLn("Address dest : 0x%08X", header.destinyAddress);
		System::logLn("Address src  : 0x%08X", header.sourceAddress);

		System::logLn("Packet id    : %u", header.id);
		System::logLn("Packet flags : 0x%04X", header.flags);
		System::logLn("-----------------------------------------------------");
		System::logLn("-----------------------------------------------------\n");
	}

	void Packet::printAllHex(){

		char buf[128];
		size_t pos = 0;

		System::logLn("-----------------------------------------------------------");
		System::logLn("-------------------- PACKET HEX HEADER --------------------");
		for (uint8_t q = 0; q < getHeaderSize(); q++){
			pos += sprintf(&buf[pos], "0x%02X ", idx[q]);
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

	PacketTx::~PacketTx(){


	}

	PacketTx::PacketTx() :
			Packet(){

		retransmitionCount = 0;
		ackTime = 0;
		txTime = 0;

		onTxCallback = nullptr;
		onAckCallback = nullptr;
		onFailCallback = nullptr;

		this->header.id=0;
		this->header.sourceAddress = 0;
	}

	PacketTx::PacketTx(uint8_t* dataValues, size_t sizeData, TxConfig packetTxConfig, uint8_t flags) :
			PacketTx(){
		if(sizeData > SizeDataMax)
			return;

		if(sizeData <= SizeDataMax)
			memcpy(data, dataValues, sizeData);

		this->dataSize = sizeData;
		this->txConfig = packetTxConfig;

		this->header.signature = SKM_SIGNATURE;
		this->header.destinyAddress = txConfig.destinyAddress;
		this->header.type = txConfig.type;
		this->header.flags = flags;

	}

	void PacketTx::configSourceAddress(Address address){
		this->header.sourceAddress = address;
	}
	void PacketTx::configId(uint16_t id){
		this->header.id = id;
	}

	bool PacketTx::isAckTimeout(){
		return System::getTick() - txTime > txConfig.ackTimout;
	}

	bool PacketTx::canDoRetransmition(){
		if(retransmitionCount >= txConfig.retransmitionCountMax){
			return false;
		}
		retransmitionCount++;
		return true;
	}


	void PacketTx::doTxed(){
		this->txTime = System::getTick();
		if(onTxCallback != nullptr)
			onTxCallback(this);
	}

	void PacketTx::doFailed(){
		if(onFailCallback != nullptr)
			onFailCallback(this);

	}
	void PacketTx::doAcked(){
		ackTime = System::getTick();
		if(onAckCallback != nullptr)
			onAckCallback(this);
	}

	uint32_t PacketTx::getTxTime(){
		return txTime;
	}
	uint32_t PacketTx::getAckTime(){
		return ackTime;
	}
	uint32_t PacketTx::getInFlightTime(){
		return ackTime - txTime;
	}

	void PacketTx::onFail(PacketCallback onFailCallback){
		this->onFailCallback = onFailCallback;
	}
	void PacketTx::onTx(PacketCallback onSentCallback){
		this->onTxCallback = onSentCallback;
	}
	void PacketTx::onAck(PacketCallback onAckCallback){
		this->onAckCallback = onAckCallback;
	}

	//-----------------------------------------------------------------------------------------------------------------

	PacketRx::~PacketRx(){
	}

	PacketRx::PacketRx() :
			Packet(){
	}

	PacketRx::PacketRx(uint8_t* dataAll, size_t sizeAll, int8_t rssi){
		if(sizeAll <= SizePacketMax)
			memcpy(idx, dataAll, sizeAll);
		dataSize = sizeAll - SizeHeader;
		this->rxTime = System::getTick();
		this->rssi = rssi;
	}

	uint32_t PacketRx::getRxTime(){
		return rxTime;
	}

	bool PacketRx::isValid(){
		return header.signature == SKM_SIGNATURE;
	}

	int32_t PacketRx::getdBm(){
		return rssi/2;//(0-rssi)/2;
	}


}
