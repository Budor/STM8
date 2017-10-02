#include "timer.h"

void Init_Tim4(void){
	CLK->PCKENR1 |= (1<<4);
	
	TIM4->CR1=0x01;                //开计数器
	TIM4->IER=0x01;                //开更新中断
	TIM4->EGR=0x01;                //手动更新
	TIM4->CNTR=0;                  //设置计数
	TIM4->ARR=0xff;                //设置装载
	TIM4->PSCR=0x07;               //设置128分频      125ms                2ms

	ITC->ISPR6 = 0x00; 
}

void Timer2Init(void){
    CLK->PCKENR1 |= (1<<7); 		//时钟使能

	TIM2->ARRH = (50000>>8) & 0xff;
	TIM2->ARRL = ((50000)>>0) & 0xff;    			//设置装载
	TIM2->PSCR = TIM2_PRESCALER_32;

	TIM2->IER = 0x01;						//开更新中断
    TIM2->CR1 = 0x05;          				//开计数器
	//TIM1->RCR = 20;                 //重复计数
	//TIM1->EGR = 0x01;               //手动更新
	 
}