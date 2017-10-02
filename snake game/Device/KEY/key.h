#ifndef _KEY_
	#define _KEY_
    
#include "stm8s_tim4.h"
#include "stm8s_gpio.h"
#include "snake.h"

#define     KEY_U       GPIO_PIN_7
#define     KEY_L       GPIO_PIN_3
#define     KEY_D       GPIO_PIN_4
#define     KEY_R       GPIO_PIN_2
#define     KEY_OK      GPIO_PIN_3

extern Snake snake;

void KEY_init(void);
void KEY_U_event(void);
void KEY_D_event(void);
void KEY_R_event(void);
void KEY_L_event(void);

#endif