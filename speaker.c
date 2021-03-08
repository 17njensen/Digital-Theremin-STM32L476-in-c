#include "stm32l476xx.h"
#include "sensor.h"
#include "math.h"
#include "stdbool.h"

extern double note;
extern double vol;

int buzzing = 0;


void playnote(int note){ //Plays a note for about 1/2 second use note values 0-4
	int duration = 300;
	while(duration > 0){
		duration--;
		if ((GPIOB->ODR & 0x00000040) != 0){
			GPIOB->ODR &= 0xFFFFFFBF;
		}else{
			GPIOB->ODR |= 0x00000040;
		}
		int freq = 5000 - 1000*note;
		while(freq > 0){
			freq--;
		}
	}
}
