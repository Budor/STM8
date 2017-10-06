#include "delay.h"

void delay1us(unsigned short _us){
  char a=0;
  while (_us--) {
   for(a=0;a<1;a++)
	   ;
  }
}

void delay1ms(unsigned short _ms)
{  
      while (_ms--){
          delay1us(1000);
      }
}
