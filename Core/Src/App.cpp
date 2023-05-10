/*
 * App.c
 *
 *  Created on: Dec 7, 2021
 *      Author: skocz
 */

#include <App.hpp>

App App::instance;
App* App::getInstance(){
	return &instance;
}

App::App(){
	this->lastLoopTick = 0;
	this->loopsInTenMs = 0;
}

void App::run(){
	init();
	while(1){
		loopsInTenMs++;
		loop();

		if(System::getTick() > lastLoopTick + 10){
			System::setLoopsInTenMs(loopsInTenMs);
			loopsInTenMs = 0;
			lastLoopTick = System::getTick();
		}
	}

}

void App::init(){
	dev->init();

	dev->led[0].ON();

	if(System::getSKID() == 0x0ED15193){
		friendAddress = 0x1C8911DF;
	}else{
		friendAddress = 0x0ED15193;
	}

	dev->radio.onRx(1, [this](SKP2P::PacketRx* packet){
		System::logLn("Odebrano pakiet, moc: %ddBm!", packet->getdBm());
		//packet->printInfo();
		//packet->printAllHex();
		dev->led[0].timeOFF(25);
	});

	System::logLn("\n\nSTART!\n");

}

void App::loop(){
	dev->iterateNonCritical();

	if(uint8_t q = dev->button[0].isMultiClick()){

		uint8_t data[10] = { 0x11, 0x22, 0x33, 0x44 };
		SKP2P::PacketTx::TxConfig cfg;

		cfg.destinyAddress = friendAddress;
		cfg.type = q;
		cfg.retransmitionCountMax = 2;
		cfg.ackTimout = 200;

		SKP2P::PacketTx pac = SKP2P::PacketTx(data, 10, cfg);

		pac.onAck([](SKP2P::PacketTx* packet){
			System::logLn("OnAck!");
			SKWL::getInstance()->led[2].timeON(250);
		});
		pac.onFail([](SKP2P::PacketTx* packet){
			System::logLn("OnFail!");
			SKWL::getInstance()->led[1].timeON(50);
		});
		pac.onTx([](SKP2P::PacketTx* packet){
			System::logLn("OnTx!");
		});

		dev->radio.transmit(&pac);

		dev->led[0].timeOFF(25);
	}

}
