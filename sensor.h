//#ifndef __STM32L476G_DISCOVERY_FINAL
//#define __STM32L476G_DISCOVERY_FINAL
#include <stdint.h>

//sensor.c
void sendtrigger1(void);
void sendtrigger2(void);
//void delay(__IO uint32_t nCount);
float receivetrigger1(void);//sensor 1 receive
float receivetrigger2(void);
float distance(float time);
double determinenote(float dist);
void determinevolume(float dist);

//speaker.c
void playnote(int note);
void sysbuzz(void);
void Create_Sine_Table(void);
uint32_t lookup_sine(int x);
void delay(__IO uint32_t nCount);

//initialize.c
void initialize(void);

//DAC.c
void DAC_Channel2_Init(void);
void TIM3_Init(void);
void TIM2_Init(void);
void TIM3_IRQHandler(void);
void TIM2_IRQHandler(void);
void Create_Sine_Table(void);
uint32_t lookup_sine(int x);
extern volatile int v;

//#endif /* __STM32L476G_DISCOVERY_FINAL */
