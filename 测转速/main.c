#include "gpio.h"
#include "timer.h"
#include "xn297l.h"
#include "spi.h"

extern u16 frequency;
extern u8 flag;

u8 freStr[6] = {0,0,0,0,0,0};
void sysClk_Init(void);

int main( void ){	
    CLK->CKDIVR = 0x00;
    GPIOInit();
    CLK->PCKENR1 |= (1<<1); 
	SPIInit();
	xn297lInit();
	Timer2Init();
	sysClk_Init();
	__enable_interrupt();
	
	
	for(;;){
		if(flag){
            flag = 0;
			freStr[2] =  0xAA;
			freStr[3] = (u8)(frequency  >> 8);
			freStr[4] = (u8)(frequency  >> 0);
			freStr[5] =  0x55;
			enterSendMode();
			rfSendNData(freStr, 6);			
		}	
	}
}

void sysClk_Init(){
  	CLK_HSECmd(ENABLE);
	while (CLK_GetFlagStatus(CLK_FLAG_HSERDY) == RESET); 
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO,CLK_SOURCE_HSE, DISABLE,CLK_CURRENTCLOCKSTATE_DISABLE);
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
	while (CLK_GetSYSCLKSource() != CLK_SOURCE_HSE);
}
