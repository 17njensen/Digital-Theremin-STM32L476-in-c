#include "stm32l476xx.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "sensor.h"

volatile int v = 0;
signed int sine_table[91];

extern int note; //extern just allows this variable to be global across all files. It's declared in main.c
extern int vol;

void DAC_Channel2_Init(void)
{
	//DAC channel 2: DAC_OUT2 = PA 5
	//enable DAC clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN;
	
	//Disable DAC
	DAC->CR &= ~(DAC_CR_EN1 | DAC_CR_EN2);

	DAC->MCR &= ~(7U<<16);						//mode = 000;
	
	//Enable Trigger for DAC channel 2
	DAC->CR |= DAC_CR_TEN2;
	
	//clear trigger selection bits for channel 2
	DAC->CR &= DAC_CR_TSEL2;				//set to software trigger
	
	//select TIM2_TRGO as the trigger of DAC channel 2 (100)
	DAC->CR |= (DAC_CR_TSEL2_2);
	//DAC->CR |= DAC_CR_EN2;

	Create_Sine_Table(); 
	
	//Enable DAC Channel 2
	DAC->CR |= DAC_CR_EN2;
	
}

void TIM2_Init(void)  //timer for DAC frequency updated every 2ms
{
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;	//Enable Clock of Timer 2
	TIM2->CR1 &= ~TIM_CR1_CMS;				//Clear Edge-Aligned mode bits
	TIM2->CR1 &= ~TIM_CR1_DIR;				//Counting direction: Up-counting;
	
	TIM2->CR2 &= ~TIM_CR2_MMS;				//Clear master mode selection bits
	TIM2->CR2 |= TIM_CR2_MMS_2;				//select 100 = OC1REF as TRGO
	
	//OC1M:  output Compare 1 mode
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	
	//Timer driving frequency = 80 MHz/(1 + PSC) = 80 MHz/(1+799) = 10kHz
	//Trigger frequency = 10kHz/(1 + ARR) = 1kHz - 100Hz
	TIM2->PSC = 2;							//max 65535;
	TIM2->ARR = 10;								//max 65535;
	TIM2->CCR1 = 5;								//Duty ration 50%
	
	TIM2->DIER |= TIM_DIER_UIE;				//Enable update interrupts
	NVIC_EnableIRQ(TIM2_IRQn);				//Enable TIM2 Interrupt in NVIC_EnableIRQ
	NVIC_SetPriority(TIM2_IRQn, 3);			//Set TIM2 interrupt priority to 3
	
	TIM2->CCER |= TIM_CCER_CC1E; 			//OC1 signal is output
	TIM2->CR1 |= TIM_CR1_CEN;				//Enable Timer	
}


void TIM2_IRQHandler(void)							//put next value from table into DAC
{
	v += 20;
	v = v % 360;
	DAC->DHR12R2 = lookup_sine(v)/vol;
	DAC->CR |= DAC_CR_EN2;
	if((TIM2->SR & TIM_SR_UIF) != 0)
	{
		TIM2->SR &= ~TIM_SR_UIF;
	}
}

void Create_Sine_Table(void)
{
    float sf;

    // for 12-bit [0, 4095(0xFFF)];
    for (int i = 0; i <= 90; i++){
        sf = sin(3.14159 * i /180);
        sine_table[i] = (1 + sf) * 2048;
        if(sine_table[i] == 0x1000){
            sine_table[i] = 0xFFF;
				}
    }
    return;
}

uint32_t lookup_sine(int x)
{
    // x is the input in degrees
    x = x % 360; // shift into first period
    if (x < 90) return (sine_table[x]);
    if (x < 180) return (sine_table[180-x]);
    if (x < 270) return ((4096 - sine_table[x-180]));
    return ((4096 - sine_table[360-x]));
}

