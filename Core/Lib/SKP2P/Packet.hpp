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

#include "Utility/LinkedList.hpp"

namespace SKP2P{
	class Packet {
	public:

		typedef uint32_t Address;
		typedef uint16_t Type;

		constexpr static const Address ADDRESS_BROADCAST = 0xFFFFFFFF;

		constexpr static const size_t SizeHeader = 15;
		constexpr static const size_t SizeDataMax = 240;
		constexpr static const size_t SizePacketMax = SizeHeader + SizeDataMax;

		constexpr static const uint16_t SKM_SIGNATURE = 0x534B;

		constexpr static const uint16_t FLAG_ACK_PACKET = (1 << 0);

		struct Header{
			Address destinyAddress;
			Address sourceAddress;
			uint16_t signature;
			Type type;
			uint16_t id;
			uint8_t flags;
		};

	protected:

		Header header = {0};
		uint8_t* data = nullptr;
		size_t dataSize;

		Header encodeHeader(uint8_t* headerData);


	public:
		~Packet();
		Packet(uint8_t* initialData, size_t initialDataSize);

		virtual Header getHeader();

		virtual size_t getData(uint8_t* buffer, size_t maxSize);

		uint8_t getMaxDataSize();
		uint8_t getDataSize();
		uint8_t getHeaderSize();
		uint8_t getTotalSize();

		bool isAckPacket();
		bool isBroadcastPacket();
		bool isForMe(Address myAddress);

		void printInfo();
		void printAllHex();

	};

	class PacketTx : public Packet {
	public:
		typedef std::function<void(PacketTx* instance)> PacketCallback;
		struct TxConfig {
			Address destinyAddress = 0;
			Type type = 0;
			uint16_t retransmitionCountMax = 3;
			uint32_t ackTimout = 250;
		};

	public:
		~PacketTx();
		PacketTx();
		PacketTx(uint8_t* dataValues, size_t sizeData, TxConfig packetConfig, uint8_t flags=0x00);

		void configSourceAddress(Address address);
		void configId(uint16_t id);

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

	class PacketRx : public Packet {
	public:
		~PacketRx();
		PacketRx();
		PacketRx(uint8_t* dataAll, size_t sizeAll, int8_t rssi=0);

		bool isValid();

		uint32_t getRxTime();
		int32_t getdBm();

	private:
		uint32_t rxTime;
		int8_t rssi;

	};
}

#endif /* SRC_SX1262_PACKET_HPP_ */
