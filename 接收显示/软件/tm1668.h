#ifndef __TM1668_H
#define __TM1668_H


#define SPI_STB_On() GPIOA->ODR &=~(1 << 3)
#define SPI_STB_Off() GPIOA->ODR |=(1 << 3);
#define TM1668_WriteData(data) spiLoopSendReceive(data);
#define TM1668_SendData(pucDataBuf) spiSendData(pucDataBuf,14);

void TM1668_Init(void);

#endif