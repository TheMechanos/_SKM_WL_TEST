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
	//POPRAWKI GRAFICZNE
}


void App::loop(){
/*
	uint8_t buf[10];
	if(dev->button[0].isPressed()){
		dev->led[2].ON();
		while(1){
			HAL_Delay(500);
			dev->led[0].toggle();
			dev->sx.transmit(buf, 1, 100);

		}

	}
	if(dev->button[1].isPressed()){
		dev->led[2].ON();
		while(1){
			if(dev->sx.receive(buffer, 1, 1000)){
				dev->led[0].toggle();
			}

		}

	}

*/

	if(uint8_t q = dev->button[0].isMultiClick()){
		uint8_t buf[1];
		buf[0] = q;
		dev->sx.transmit(buf, 1, 100);
		dev->led[2].timeON(300);

	}

	uint8_t buffer[10];
	if(dev->button[1].isPressed()){
		dev->led[2].ON();
		uint8_t q=dev->sx.receive(buffer, 1, 5000);
		if(q==1){
			dev->led[0].blinkNumber(buffer[0], 500);
		}
		dev->led[2].OFF();

	}



}



void App::loop10ms(){
	dev->iterateNonCritical();
}

