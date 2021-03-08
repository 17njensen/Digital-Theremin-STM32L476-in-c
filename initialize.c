#include "stm32l476xx.h"
#include "sensor.h"
void initialize(){
	//Enable clocks
	RCC -> CR |= RCC_CR_HSION;
	while(((RCC -> CR) & RCC_CR_HSIRDY) == 0){
		//wait for hsi to be ready
	}
		//Sensor 1 will use PA0, PA1, 5V, and GND
		//Sensor 2 will use PE10, PE11, 5V and GND
		//..... (for more sensors)
		//Speaker output will use PB6
	//enable GPIO port A 														- Sensor 1 initialize
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	GPIOA->MODER &= 0xFFFFF3F0;
	//Set PA0 as output, PA1 as input
	GPIOA->MODER |= 0x00000001;
	//For DAC
	// Now set PA5 to output mode (01)
	GPIOA->MODER |= 0x00000C00;
	
	//Enable GPIO port E 														- Sensor 2 initialize
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	GPIOE->MODER &= 0xFF0FFFFF;
	//Set PE10 as output and PE11 as input
	GPIOE->MODER |= 0x00100000;
	
	//Enable GPIO port B 														- Speaker initialize
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	GPIOB->MODER &= 0xFFFFCFFF;
	//Set PB6 as output
	GPIOB->MODER |= 0x00001000;
}
