#include "tm1668.h"
#include "delay.h"
#include "spi.h"

void TM1668_WriteCom(u8 com);
void  TM1668_WriteComData(u8 com,u8 *pdata);

u8 DataBuf[14] ={1,1,1,1,10,1,1,1,1,15,1,1,1,1};




void  TM1668_WriteCom(u8 com){
    SPI_STB_On(); 
	spiLoopSendReceive(com);
	SPI_STB_Off();
	delay1us(5);
}

void  TM1668_WriteComData(u8 com,u8 *pdata){
    SPI_STB_On(); 
	spiLoopSendReceive(com);
	TM1668_SendData(pdata);
	SPI_STB_Off();
	delay1us(5);
}

