/****************************************Copyright (c)**************************************************
**                               Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                                 http://www.embedtools.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name:         
** Last modified Date: 
** Last Version:       v1.00
** Description:        
** 
**------------------------------------------------------------------------------------------------------
** Created By:         
** Created date:       
** Version:            v1.00
** Descriptions:
**
**------------------------------------------------------------------------------------------------------
** Modified by:        
** Modified date:       
** Version:             
** Description:        
**
********************************************************************************************************/
#ifndef __SPI_H
#define __SPI_H

#include "stm8s.h"

void SPIInit(void);
void spiSendData(u8* pucDataBuf, u8 ucCnt);
void spiGetData(u8* pucDataBuf, u8 ucCnt);
u8 spiLoopSendReceive(u8 ucData);

#endif
