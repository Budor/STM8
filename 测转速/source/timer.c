#include "timer.h"

void Init_Tim4(void){
	CLK->PCKENR1 |= (1<<4);
	
	TIM4->CR1=0x01;                //��������
	TIM4->IER=0x01;                //�������ж�
	TIM4->EGR=0x01;                //�ֶ�����
	TIM4->CNTR=0;                  //���ü���
	TIM4->ARR=0xff;                //����װ��
	TIM4->PSCR=0x07;               //����128��Ƶ      125ms                2ms

	ITC->ISPR6 = 0x00; 
}

void Timer2Init(void){
    CLK->PCKENR1 |= (1<<7); 		//ʱ��ʹ��

	TIM2->ARRH = (50000>>8) & 0xff;
	TIM2->ARRL = ((50000)>>0) & 0xff;    			//����װ��
	TIM2->PSCR = TIM2_PRESCALER_32;

	TIM2->IER = 0x01;						//�������ж�
    TIM2->CR1 = 0x05;          				//��������
	//TIM1->RCR = 20;                 //�ظ�����
	//TIM1->EGR = 0x01;               //�ֶ�����
	 
}