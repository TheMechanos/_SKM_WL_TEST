/*
 * Packet.hpp
 *
 *  Created on: 15 gru 2021
 *      Author: user
 */

#ifndef SRC_SX1262_PACKET_HPP_
#define SRC_SX1262_PACKET_HPP_

#include <System.hpp>
#include <stdio.h>
#include <string.h>

#include <LinkedList.hpp>

class SKMPacket {
	friend class SKMController;
public:

	typedef uint32_t Address;
	typedef uint16_t Type;

	constexpr static const uint8_t SizeHeader = 26;
	constexpr static const uint8_t SizeDataMax = 220;
	constexpr static const uint8_t SizePacketMax = SizeHeader + SizeDataMax;

	constexpr static const uint16_t SKM_SIGNATURE = 0x534B;

	constexpr static const uint16_t FLAG_ACK_PACKET = (1 << 0);
	constexpr static const uint16_t FLAG_DISCOVERY_PACKET = (1 << 1);
	constexpr static const uint16_t FLAG_DISCOVERY_RESPONSE_PACKET = (1 << 2);

	constexpr static const Address ADDRESS_BROADCAST = 0xFFFFFFFF;



	union Header {
		struct {
			uint16_t signature;

			Address senderAddress;
			Address receiverAddress;

			Address destinyAddress;
			Address sourceAddress;

			Type type;
			uint16_t id;
			uint8_t flags;
			uint8_t hops;
			uint16_t cryptoKey;
		};

		uint8_t idx[SizeHeader];
	};

	typedef uint8_t PacketData;
	typedef PacketData PacketDataArray[SizeDataMax];

protected:
	union {
		struct {
			Header header;
			PacketDataArray data;
		};
		uint8_t idx[SizePacketMax];
	};
	uint8_t dataSize;

public:
	~SKMPacket();
	SKMPacket();

	Header getHeader();
	PacketData* getData();
	uint8_t* getTotalIdx();

	uint8_t getDataSize();
	uint8_t getHeaderSize();
	uint8_t getTotalSize();

	bool isAckPacket();
	bool isBroadcastPacket();
	bool isForMe(Address myAddress);



	uint8_t getMaxDataSize();

	void printInfo();
	void printAllHex();

};

class SKMPacketTx : public SKMPacket {
public:
	typedef std::function<void(SKMPacketTx* instance)> PacketCallback;
	struct TxConfig {
		Address destinyAddress = 0;
		Type type = 0;
		uint16_t retransmitionCountMax = 3;
		uint32_t ackTimout = 250;
	};

public:
	~SKMPacketTx();
	SKMPacketTx();
	SKMPacketTx(uint8_t* dataValues, size_t sizeData, TxConfig packetConfig);

	bool isAckTimeout();
	bool canDoRetransmition();

	void doTxed();
	void doFailed();
	void doAcked();

	uint32_t getTxTime();
	uint32_t getAckTime();
	uint32_t getInFlightTime();

	void onTx(PacketCallback onSent);
	void onFail(PacketCallback onFail);
	void onAck(PacketCallback onAck);

private:
	PacketCallback onFailCallback;
	PacketCallback onTxCallback;
	PacketCallback onAckCallback;

	TxConfig txConfig;

	uint16_t retransmitionCount;

	uint32_t txTime;
	uint32_t ackTime;
};

class SKMPacketRx : public SKMPacket {
public:
	~SKMPacketRx();
	SKMPacketRx();
	SKMPacketRx(uint8_t* dataAll, size_t sizeAll, int8_t rssi=0);

	bool isValid();

	uint32_t getRxTime();

	int32_t getdBm();

private:
	uint32_t rxTime;
	int8_t rssi;

};

#endif /* SRC_SX1262_PACKET_HPP_ */
