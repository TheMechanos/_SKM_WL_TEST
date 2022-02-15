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
	this->lastLoopTick=0;
	this->loopsInTenMs=0;
}


void App::run(){
	init();
	while(1){
		loopsInTenMs++;
		loop();

		if(lastLoopTick < System::getTick()-10){
			loop10ms();
			System::setLoopsInTenMs(loopsInTenMs);
			loopsInTenMs=0;
			lastLoopTick = System::getTick();
		}
	}

}


void App::init(){
	dev->init();

	dev->led[0].ON();

}


void App::loop(){

	if(uint8_t q = dev->button[0].isMultiClick()){

		SKMPacket pac;
		pac.init(2);
		pac.dataIdx[0] = q;

		pac.header.address.source = 10;
		pac.header.address.destiny = 10;
		pac.header.address.sender=10;
		pac.header.packet.hops=10;
		pac.header.packet.cryptoKey=10;
		pac.header.packet.type=10;
		pac.header.packet.id=10;
		pac.header.device.type=10;
		pac.header.device.info=10;

		dev->sx.transmitPacket(&pac, 100);

		dev->led[2].timeON(300);
	}

	if(dev->button[1].isPressed()){
		dev->led[2].ON();
		SKMPacket pac;
		uint8_t q = dev->sx.receivePacket(&pac, 5000);
		if(q){
			dev->led[0].blinkNumber(pac.dataIdx[0], 500);
		}
		dev->led[2].OFF();

	}



}



void App::loop10ms(){
	dev->iterateNonCritical();
}

