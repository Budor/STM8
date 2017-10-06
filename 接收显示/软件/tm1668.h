#ifndef _TM1668_H
#define _TM1668_H

#include "stm8s.h"

#define 	STB_L()  	GPIOA-> ODR &= ~(1 << 3)
#define 	STB_H()  	GPIOA-> ODR |=(1 << 3)
#define 	CLK_L()  	GPIOB-> ODR &= ~(1 << 4)
#define 	CLK_H()  	GPIOB-> ODR |=(1 << 4)
#define 	DIO_L()  	GPIOB-> ODR &= ~(1 << 5)
#define 	DIO_H() 	GPIOB-> ODR |=(1 << 5)

void TM1668_Init(void);
u8 getKeyVlu(void);
void SEG_Display(u8 *pArray,u8 ledState);

#endif 
