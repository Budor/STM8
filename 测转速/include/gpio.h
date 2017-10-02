#include "stm8s_gpio.h"
#ifndef __GPIO_H
  #define __GPIO_H

  #define GPIO2LOW(g,d)                  g->ODR&=~d
  #define GPIO2HIGH(g,d)                 g->ODR|=d
  #define RFCSS2LOW()                    GPIO2LOW(GPIOD,GPIO_PIN_2)
  #define RFCSS2HIGH()                   GPIO2HIGH(GPIOD,GPIO_PIN_2)
  #define RFCE2LOW()                     GPIO2LOW(GPIOD,GPIO_PIN_3)
  #define RFCE2HIGH()                    GPIO2HIGH(GPIOD,GPIO_PIN_3)

  #define RFIRQPORT                      GPIOC
  #define RFIRQBIT                       (u8)(1<<4)

  #define CMDPORT                        GPIOD
  #define CMDBIT                         (u8)(1<<1)


  #define LEDPORT                    GPIOC
  #define LEDBIT                     GPIO_PIN_3
 

  #define ledOn()                        GPIO2HIGH(LEDPORT,LEDBIT)//noFunction()
  #define ledOff()                       GPIO2LOW(LEDPORT,LEDBIT)//noFunction()
  #define ledFlash()                     LEDPORT->ODR ^= LEDBIT//noFunction()

  #define enableIRQInterrupt()           RFIRQPORT->CR2 |= RFIRQBIT
  #define disableIRQInterrupt()          RFIRQPORT->CR2 &= ~RFIRQBIT


  void GPIOInit(void);

#endif
