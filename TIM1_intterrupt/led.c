#include "main.h"
#include "led.h"

//PB5
void led_init(void)
{
  //PX_DDR    0������ 1�����
  PB_DDR |= (1<<5);
  //PX_CR1    0: ��������|��©���  1����������|�������
  PB_CR1 |= (1<<5);
  //PX_CR2    0�������ֹ�ж�|����ٶ�2MHZ  1������ʹ���ж�|����ٶ�10MHZ
  PB_CR2 |= (1<<5);
  //PX_ODR    �������״̬�Ĵ���
  PB_ODR |= (1<<5);//Ĭ�Ϲر�led
}

