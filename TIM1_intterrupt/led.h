#ifndef _LED_H
#define _LED_H

#define LED_ON        PB_ODR &= ~(1<<5)//��led
#define LED_OFF       PB_ODR |=  (1<<5)//�ر�led
void led_init(void);


#endif