#include "gpio.h"

void GPIOInit(void){
	GPIOA -> DDR = 0;
    GPIOA -> CR1 = 0;
    GPIOA -> CR2 = 0;
  
	GPIOA->DDR &= ~(u8)(1<<3);      //输入模式
	GPIOA->CR1 |= (u8)(1<<3);		//上拉输入
	GPIOA->CR2 |= (u8)(1<<3); 		//开引脚中断

	EXTI->CR1 |= (u8)(1<<1);		//设置触发模式  :下降沿触发
	//EXTI->CR2 |= 0X04;
	
	/******配置Motot的GPIO******/
//	GPIOC->DDR |= (u8)(1<<1);       //输出模式
//	GPIOC->CR1 |= (u8)(1<<1);		//推挽输出
//	GPIOC->CR2 |= (u8)(1<<1);      	//输出最大速度为10MHz
    

  
  GPIOB -> DDR = 0;
  GPIOB -> CR1 = 0;
  GPIOB -> CR2 = 0;
  
  GPIOC -> DDR = (1<<5)|(1<<6);
  GPIOC -> CR1 = (1<<3)|(1<<4); 
  GPIOC -> CR2 = (1<<3)|(1<<5)|(1<<6);
  
  GPIOD -> DDR = (1<<2)|(1<<3)|(1<<5);
  GPIOD -> CR1 = (1<<1)|(1<<2)|(1<<3)|(1<<6); 	// RXD上拉输入
  GPIOD -> CR2 = (1<<5); 						// TXD高速输出
  GPIOD -> ODR |= (1<<2)|(1<<5); 
    
}



