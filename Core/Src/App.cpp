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

	dev->radio.onRx(1, [this](SKMPacket* packet){
		System::logLn("odebrano pakiet!");
		packet->printInfo();
		packet->printAllHex();
		dev->led[2].blinkNumber(1, 25, 25);
	});

}

//kontrola id pakietu przy dodawaniu

bool txcw = false;

void App::loop(){
	dev->iterateNonCritical();

	if(uint8_t q = dev->button[0].isMultiClick()){

		uint8_t data[10] = { 0x11, 0x22, 0x33, 0x44 };
		SKMPacketTx::TxConfig cfg;

		cfg.destinyAddress = 0x00000001;
		cfg.type = q;
		cfg.retransmitionCountMax = 3;

		SKMPacketTx pac = SKMPacketTx(data, 10, cfg);

		pac.onAck([](SKMPacketTx* packet){
			System::logLn("OnAck!");
		});
		pac.onFail([](SKMPacketTx* packet){
			System::logLn("OnFail!");
		});
		pac.onSent([](SKMPacketTx* packet){
			System::logLn("OnSent!");
		});

		dev->radio.send(&pac);

		dev->led[2].timeON(300);
	}

	if(dev->button[1].isPressed()){
		if(txcw){
			dev->sxRadio.setRx();
			dev->led[1].OFF();
		}else{
			dev->sxRadio.setModeTxContinousWave();
			dev->led[1].blink(200, 200);
		}
		txcw = !txcw;
	}

}
