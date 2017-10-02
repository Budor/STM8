/*
  定时器1每1s中断一次，改变LED状态
*/
#include "main.h"
#include "led.h"

void init_time1(void)
{
  //打开TIM1定时器时钟
  CLK_PCKENR1 |= 0X80;
  //选择定时器1分频
  TIM1_PSCRH = 0x07;
  TIM1_PSCRL = 0xcf;
  //只有溢出才产生中断
  TIM1_CR1 |= (1<<2);
  //允许更新中断
  TIM1_IER |= 0x01; 
  //填写初始值
  TIM1_ARRH = 0x03;
  TIM1_ARRL = 0xe8;
  //使能定时器
  TIM1_CR1 |= 0X01; 
  asm("rim");   //打开全局中断  
}

void main(void)
{
  led_init();    
  init_time1(); //配置定时器中断
  while(1);
}
