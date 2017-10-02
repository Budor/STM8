#ifndef _LED_H
#define _LED_H

#define LED_ON        PB_ODR &= ~(1<<5)//´ò¿ªled
#define LED_OFF       PB_ODR |=  (1<<5)//¹Ø±Õled
void led_init(void);


#endif