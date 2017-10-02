#include "main.h"
#include "led.h"

//PB5
void led_init(void)
{
  //PX_DDR    0：输入 1：输出
  PB_DDR |= (1<<5);
  //PX_CR1    0: 浮空输入|开漏输出  1：上拉输入|推挽输出
  PB_CR1 |= (1<<5);
  //PX_CR2    0：输入禁止中断|输出速度2MHZ  1：输入使能中断|输出速度10MHZ
  PB_CR2 |= (1<<5);
  //PX_ODR    数据输出状态寄存器
  PB_ODR |= (1<<5);//默认关闭led
}

