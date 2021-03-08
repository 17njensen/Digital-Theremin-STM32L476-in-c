#include "stm32l476xx.h"
#include "sensor.h"

extern int note;
extern int vol; 

void sendtrigger1(){
	//needs to have PA0 set as high for 10us
	//10us has a period of 100,000Hz
		//so our delay = 16MHz/100kHz = 160
	for(int i = 0; i <= 160; i++){
		GPIOA->ODR |= 0x00000001;
	}
	GPIOA -> ODR &= 0xFFFFFFFE;
}

void sendtrigger2(){
	//needs to have PE10 set as high for 10us
	//10us has a period of 100,000Hz
		//so our delay = 16MHz/100kHz = 160
	for(int i = 0; i <= 160; i++){
		GPIOE->ODR |= 0x00000400;
	}
	GPIOE -> ODR &= 0xFFFFFBFF;
}

void delay(__IO uint32_t nCount){
	while(nCount--);
}

float receivetrigger1(){
	//while receiving, count
	//once done receiving, calculate distance from time the input was high
	float count = 0;
	float time = 0;
	
	GPIOE->ODR |= 0x00000400; //For testing on scope
	
	while((GPIOA->IDR & 0x00000002) != 0){
		count += 1;
		if(count >= 20000){ //if count counts for too long, break
			break;
		}
	}
	
	GPIOE -> ODR &= 0xFFFFFBFF;//For testing on scope
	
	time = count * (6.25 * (10e-8));

	return time;
}


float receivetrigger2(){
			//while receiving, count
	//once done receiving, calculate distance from time the input was high
	float count = 0;
	float time = 0;
	while((GPIOE->IDR & 0x00000800) != 0){
		count += 1;
		if(count >= 20000){
			break;
		}
	}
	
	time = count * (6.25 * (10e-8));

	return time;
}

float distance(float time){
	//Distance (cm) = (Time x SpeedOfSound) / 2
	float speedofsound = 500000;  //Adjusted based on experimental results *note the real speed of sound* 500000 is a good starting value
	float dist = (time * speedofsound) / 2;
	return dist; //returns in cm
}

double determinenote(float dist){
	//Min note is middle C, going up the scale, our highest note is D5
	
	//sensor has a range of 2cm to 400cm, I decided our range should be 3cm to 46cm (or 1.5ft) 
	//increments in 5.4cm from 3cm to 46cm
		if (dist <= 3 || (dist >= 3 && dist <= 8.4)){ //note 0
			//note = 261.63;	//middle C freq in Hz
			note = 0;
		}
		if (dist > 8.4 && dist <= 13.8){ //note 1
			//note = 293.665; //D4 freq
			note = 1;
		}
		if (dist > 13.8 && dist <= 19.2){ //note 2
			//note = 329.628; //E4 freq 
			note = 2;
		}
		if (dist > 19.2 && dist <= 24.6){ //note 3
			//note = 349.228; //F4 freq
			note = 3;
		}
		if (dist > 24.6 && dist <= 30){ //note 4
			//note = 391.995; //G4 freq
			note = 4;
		}
		if (dist > 30 && dist <= 35.4){ //note 5
			//note = 440;			//A4 freq
			note = 5;
		}
		if (dist > 35.4 && dist <= 40.8){ //note 6
			//note = 493.883;	//B4 freq
			note = 6;
		}
		if (dist > 40.8 && dist <= 46.2){ //note 7
			//note = 523.251;	//C5 freq top note of scale
			note = 7;
		}
		if (dist > 46.2){ //note 7 again? //Note 8 so we can play a full scale C to C?
			note = 7; //adjust to be note past our furthest distance from the sensor
		}
	return note;
}

void determinevolume(float dist){
	//The closer to the sensor the louder the speaker is and vice versa
	//int vol = 0;
	
	//sensor has a range of 2cm to 400cm, I decided our range should be 3cm to 46cm (or 1.5ft) 
	//increments in 5.4cm from 3cm to 46cm
		if (dist <= 3 || (dist >= 3 && dist <= 8.4)){
			vol = 5;//2.50; //2.5 times our original volume (sets amplitude to 2.5 for sinewave if possible)
		}
		if (dist > 8.4 && dist <= 13.8){
			vol = 5;//2.25; //2.25 times our original volume
		}
		if (dist > 13.8 && dist <= 19.2){
			vol = 5; //2 times our volume
		}
		if (dist > 19.2 && dist <= 24.6){
			vol = 5;//1.75; //1.75 times our volume
		}
		if (dist > 24.6 && dist <= 30){
			vol = 1;//1.50; //1.5 times our volume
		}
		if (dist > 30 && dist <= 35.4){
			vol = 1;//1.25; //1.25 times our volume
		}
		if (dist > 35.4 && dist <= 40.8){
			vol = 1;//1.00;
		}
		if (dist > 40.8 && dist <= 46.2){
			vol = 1;//0.75;
		}
		if (dist > 46.2){
			vol = 1;//0.50; //half our regular volume, if the amplitude wasnt adjusted
		}
	return;// vol;
}