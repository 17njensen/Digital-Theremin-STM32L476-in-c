#include "stm32l476xx.h"
#include "sensor.h"

int note = 0;
int vol = 2;
/*
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
This code is for a digital Theremin device. The following device will have multiple inputs to the STM32L476 discovery board, 
including at least two ultrasonic distance sensors, and a speaker/buzzer depending on the need. This device will use one distance 
sensor to determine either volume or timbre. The other sensor will detect the users hand and bend the pitch of the output to 
different musical notes. 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

*/
//Pin layout
		//Sensor 1 will use PA0 (trig), PA1, 5V, and GND                                                                  
		//Sensor 2 will use PE10, PE11, 5V and GND
		//..... (for more sensors)
		//Speaker output will use PB6
int main(void){
	double time = 0;
	double time2 = 0;
	float dist = 0;
	float dist2 = 0;
	initialize();
	TIM2_Init();
	//TIM3_Init();
	DAC_Channel2_Init();

	
	//loop indefinitely
	while(1){
		sendtrigger1(); 							//outputs 10us signal from sensor1
		delay(400); 							 							//delay around 10us (number determined on scope)
		time = receivetrigger1(); 							//receives signal and translates into how long it took the signal to come back
		sendtrigger2();
		delay(400);
		time2 = receivetrigger2();							//volume trigger
		dist = distance(time); 				  //calculate distance that hand is from sensor1
		note = determinenote(dist); 					//determine the note depending on the distance
		dist2=distance(time2);
		determinevolume(dist2);
	//Timer driving frequency = 80 MHz/(1 + PSC) = 80 MHz/(1+799) = 10kHz
	//Trigger frequency = 10kHz/(1 + ARR) = 1kHz - 100Hz
	switch((int)note)
	{
		case 0: //261.63
			TIM2->ARR = 75;//.4438;					//20kHz/(1+ARR) = 261.63Hz
			TIM2->CCR1 = 38;//.2219;					//duty ratio = 50%
			break;
		case 1: //293.665
			TIM2->ARR = 67;//.1048;					//20kHz/(1+ARR) = 293.665Hz
			TIM2->CCR1 = 34;//.0524;								//duty ratio = 50%
			break;
		case 2: //329.628
			TIM2->ARR = 59;//.6745;					
			TIM2->CCR1 = 30;//.3373;					//duty ratio = 50%
			break;
		case 3: //349.228
			TIM2->ARR = 56;//.2692;			
			TIM2->CCR1 = 28;//.6346;				//duty ratio = 50%
			break;
		case 4: //391.995
			TIM2->ARR = 50;//.0211;					
			TIM2->CCR1 = 25;//.51055;				//duty ratio = 50%			
			break;
		case 5: //440
			TIM2->ARR = 44;//.4545;					
			TIM2->CCR1 = 22;//.7273;					//duty ratio = 50%
			break;
		case 6: //493.883
			TIM2->ARR = 39;//.4954;					
			TIM2->CCR1 = 20;//.2477;					//duty ratio = 50%
			break;
		case 7: //523.251
			TIM2->ARR = 37;//.2226;					
			TIM2->CCR1 = 19;//.1113;					//duty ratio = 50%
			break;
		default: //default is 261.63 (middle c)
			TIM2->ARR = 75;//.4438;					//20kHz/(1+ARR) = 261.63Hz
			TIM2->CCR1 = 38;//.2219;					//duty ratio = 50%n
			break;
	}
	TIM2->CNT = 0;
	delay(20000);
	}
}
