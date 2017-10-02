#include "gpio.h"

void GPIOInit(void){
	GPIOA -> DDR = 0;
    GPIOA -> CR1 = 0;
    GPIOA -> CR2 = 0;
  
	GPIOA->DDR &= ~(u8)(1<<3);      //����ģʽ
	GPIOA->CR1 |= (u8)(1<<3);		//��������
	GPIOA->CR2 |= (u8)(1<<3); 		//�������ж�

	EXTI->CR1 |= (u8)(1<<1);		//���ô���ģʽ  :�½��ش���
	//EXTI->CR2 |= 0X04;
	
	/******����Motot��GPIO******/
//	GPIOC->DDR |= (u8)(1<<1);       //���ģʽ
//	GPIOC->CR1 |= (u8)(1<<1);		//�������
//	GPIOC->CR2 |= (u8)(1<<1);      	//�������ٶ�Ϊ10MHz
    

  
  GPIOB -> DDR = 0;
  GPIOB -> CR1 = 0;
  GPIOB -> CR2 = 0;
  
  GPIOC -> DDR = (1<<5)|(1<<6);
  GPIOC -> CR1 = (1<<3)|(1<<4); 
  GPIOC -> CR2 = (1<<3)|(1<<5)|(1<<6);
  
  GPIOD -> DDR = (1<<2)|(1<<3)|(1<<5);
  GPIOD -> CR1 = (1<<1)|(1<<2)|(1<<3)|(1<<6); 	// RXD��������
  GPIOD -> CR2 = (1<<5); 						// TXD�������
  GPIOD -> ODR |= (1<<2)|(1<<5); 
    
}



