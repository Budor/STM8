/*
  ��ʱ��1ÿ1s�ж�һ�Σ��ı�LED״̬
*/
#include "main.h"
#include "led.h"

void init_time1(void)
{
  //��TIM1��ʱ��ʱ��
  CLK_PCKENR1 |= 0X80;
  //ѡ��ʱ��1��Ƶ
  TIM1_PSCRH = 0x07;
  TIM1_PSCRL = 0xcf;
  //ֻ������Ų����ж�
  TIM1_CR1 |= (1<<2);
  //��������ж�
  TIM1_IER |= 0x01; 
  //��д��ʼֵ
  TIM1_ARRH = 0x03;
  TIM1_ARRL = 0xe8;
  //ʹ�ܶ�ʱ��
  TIM1_CR1 |= 0X01; 
  asm("rim");   //��ȫ���ж�  
}

void main(void)
{
  led_init();    
  init_time1(); //���ö�ʱ���ж�
  while(1);
}
