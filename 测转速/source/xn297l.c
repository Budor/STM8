#include "stm8s.h"
#include "gpio.h"
#include "delay.h"
#include "spi.h"
#include "xn297l.h"

u8 ucStatus;
u8 vucChannal = 0x4E;
u8 vuctTID[5] = {0x55,0x55,0x55,0x55,0x55};
u8 vuctRID[5] = {0x55,0x55,0x55,0x55,0x55};
u8 vucAck;
u8 vucRetry = 0x0A;
u8 vucBack;
u8 vucPower = 0;
u8 vucSpeed = 0x00;
u8 vucBaud;
const u8 TX_ADDRESS_DEF[5] = {0x55,0x55,0x55,0x55,0x55};    		//RF 地址：接收端和发送端需一致
const u8 cucPower[13] = {RF13dBm,RF10dBm,RF8dBm,RF7dBm,RF5dBm,RF4dBm,RF2dBm,RF0dBm,\
                	         RF_3dBm,RF_6dBm,RF_10dBm,RF_18dBm,RF_30dBm};
const u8 cucSpeed[3] = {DR_250K, DR_1M, DR_2M};

/*
** 写入寄存器数值
*/
void rfWriteReg( u8 ucRegAddr,  u8 ucData){
    spiCSOn();
    spiLoopSendReceive(ucRegAddr);
    spiLoopSendReceive(ucData);
    spiCSOff();
}
/*
** 读取寄存器数值
*/
u8 rfReadReg(u8 ucRegAddr){
    u8 ucData;  
    spiCSOn();
    spiLoopSendReceive(ucRegAddr);
    ucData = spiLoopSendReceive(0);
    spiCSOff();
    return ucData;
}

/*
** 写入寄存器N个数值
*/
void rfWriteNRegister(u8 ucRegAddr, u8* pusData, u8 ucCnt){
    u8 i; 
    spiCSOn();
    spiLoopSendReceive(ucRegAddr);
    for (i = 0; i < ucCnt; i++) {
        spiLoopSendReceive(pusData[i]);
    }
    spiCSOff();
}
/*
** 读取寄存器N个数值
*/
void rfReadNRegister(u8 ucRegAddr, u8* pusData, u8 ucCnt){
    u8 i; 
    spiCSOn();
    spiLoopSendReceive(ucRegAddr);
    for (i = 0; i < ucCnt; i++) {
        pusData[i] = spiLoopSendReceive(0);
    }
    spiCSOff();
}

u8 rfSendNData(u8 *ucData, u8 ucLength){
    u8 Status_Temp;
    rfWriteNRegister(W_TX_PAYLOAD, ucData, ucLength);                         //write data to txfifo        
    rfCE_High();                                                              //rf entery tx mode start send data 
    delay1us(10);                                                             //keep ce high at least 16us                                                              //rf entery stb3
    while(rfIrqIsHigh());                                                     //waite irq pin low 
    Status_Temp = rfReadReg(STATUS) & 0x70;                                   //读取发送完成后的status
    rfWriteReg(W_REGISTER + STATUS, Status_Temp);                             //清除Status
    rfWriteReg(FLUSH_TX,0);                                                   //清 FIFO
    return Status_Temp;
}

u8 rfReceivData(u8 *pucData){
    u8 ucLengthRecive = 0;
    if(rfIrqIsHigh()){
      return 0;                                                                //若IRQ PIN为高，则没有接收到数据
    }
    rfCE_Low(); 
    ucLengthRecive = rfReadReg(R_RX_PL_WID);
    rfReadNRegister(R_RX_PAYLOAD, pucData, ucLengthRecive);                    //将接收到的数据读出到ucPayload，且清除rxfifo
    rfWriteReg(FLUSH_RX, 0);	
    rfWriteReg(W_REGISTER + STATUS, 0x70);                                     //清除Status
    rfCE_High();                                                               //继续开始接收
    return ucLengthRecive;
}

void clearTXRXFifo(void){
    rfCE_Low(); 
    rfWriteReg(FLUSH_RX, 0);
    rfWriteReg(FLUSH_TX, 0);
    rfCE_High();
}

void enterSendMode(void){
    rfCE_Low();
    rfWriteReg(W_REGISTER + CONFIG,  0X8E );							// 将RF设置成RX模式
    rfCE_High();											// Set CE pin high 开始接收数据
    //delay1ms(2);
}

void enterPowerDownMode(void){
    rfCE_Low();
    rfWriteReg(W_REGISTER + CONFIG,  0X02 );
    delay1ms(2);
    rfWriteReg(W_REGISTER + CONFIG,  0X00 );
    delay1ms(2);
}

void enterReceiveMode(void){
    rfCE_Low();
    rfWriteReg(W_REGISTER + CONFIG,  0X8F );							// 将RF设置成RX模式
    rfCE_High();											// Set CE pin high 开始接收数据
    //delay1ms(2);
}
/*
 ** 初始化XN297L各寄存器
*/
void xn297lInit(void){
    const u8  BB_cal_data[3][5]    = {{0x12,0xec,0x6f,0xa1,0x46},{0x0A, 0x6D, 0x67, 0x9C, 0x46},{0x0A, 0x6D, 0x67, 0x9C, 0x46}}; 
    const u8  RF_cal_data[3]    = {0xf6,0x37,0x5d};
    const u8  RF_cal2_data[3][6]   = {{0xd5,0x21,0xeb,0x2c,0x5a,0x40},{0x45, 0x21, 0xEF, 0xAC, 0x5A, 0x50},{0x45, 0x21, 0xEF, 0xAC, 0x5A, 0x50}};
    const u8  Dem_cal_data[3][1]   = {{0x1f},{0x01},{0x01}};   
    const u8  Dem_cal2_data[3]  = {0x0b,0xdf,0x02};  
    //delay1ms(10);
    rfCE_Low();
    //delay1ms(10);
    rfWriteReg(RST_FSPI, 0x5A);								//Software Reset    
    rfWriteReg(RST_FSPI, 0XA5);
    delay1ms(10);
    
    rfWriteReg(W_REGISTER + CONFIG, 0x02);	
    delay1ms(2);    
    rfWriteReg(W_REGISTER + CONFIG, 0x82);	
    delay1ms(2);
    rfReadReg(R_REGISTER + CONFIG);

    rfWriteReg(FLUSH_TX, 0);											// CLEAR TXFIFO		    			 
    rfWriteReg(FLUSH_RX, 0);											// CLEAR  RXFIFO
    rfWriteReg(W_REGISTER + STATUS, 0x70);								// CLEAR  STATUS	
    rfWriteReg(W_REGISTER + EN_RXADDR, 0x01);							// Enable Pipe0
    rfWriteReg(W_REGISTER + SETUP_AW,  0x03);							// address witdth is 5 bytes
    rfWriteReg(W_REGISTER + RF_CH,     vucChannal);                             // 2478M HZ
    //rfWriteReg(W_REGISTER + RX_PW_P0,  PAYLOAD_WIDTH);						// 8 bytes
    rfWriteNRegister(W_REGISTER + TX_ADDR,   (u8*)&vuctTID[0], 5);	            // Writes TX_Address to PN006
    rfWriteNRegister(W_REGISTER + RX_ADDR_P0,(u8*)&vuctRID[0], 5);				// RX_Addr0 same as TX_Adr for Auto.Ack   
    
    rfWriteNRegister(W_REGISTER + BB_CAL,    (u8*)&BB_cal_data[vucSpeed][0],  5);
    rfWriteNRegister(W_REGISTER + RF_CAL2,   (u8*)&RF_cal2_data[vucSpeed][0], 3);
    rfWriteNRegister(W_REGISTER + DEM_CAL,   (u8*)&Dem_cal_data[vucSpeed][0], 1);
    rfWriteNRegister(W_REGISTER + RF_CAL,    (u8*)&RF_cal_data[0],  6);
    rfWriteNRegister(W_REGISTER + DEM_CAL2,  (u8*)&Dem_cal2_data[0],3);
    
    
    rfWriteReg(W_REGISTER + DYNPD, 0x01);				
    
    rfWriteReg(W_REGISTER + RF_SETUP,  (cucPower[vucPower]|cucSpeed[vucSpeed]));	//DBM  	 
    rfWriteReg(W_REGISTER + SETUP_RETR, vucRetry);									//  3 retrans... 	
    
    rfWriteReg(W_REGISTER + EN_AA,   0x01);
    rfWriteReg(W_REGISTER + FEATURE, 0x04);
    //rfWriteReg(W_REGISTER + FEATURE, 0x44);  						// 测试 EM_PA功能
    if (vucAck == 0x00) {
        //rfWriteReg(W_REGISTER + FEATURE, 0x04);
    } else {
        //rfWriteReg(W_REGISTER + FEATURE, 0x06);
    }
    
    rfWriteReg(W_REGISTER + CONFIG,  0X8F );	
    rfCE_High();
}

void rfTestMode( u8 ucChannel_Set){
    u8 BB_cal_data[]    = {0x0A,0x6D,0x67,0x9C,0x46}; 
    u8 RF_cal_data[]    = {0xF6,0x37,0x5D};
    u8 RF_cal2_data[]   = {0x45,0x21,0xEF,0xAC,0x5A,0x50};
    u8 Dem_cal_data[]   = {0xE1}; 								
    u8 Dem_cal2_data[]  = {0x0B,0xDF,0x02};
	
    xn297lInit();
    delay1ms(100);
    rfCE_Low();
    rfWriteReg(W_REGISTER + CONFIG,  0X8E);							// 将RF设置成TX模式
    rfReadReg(R_REGISTER + CONFIG);
    rfWriteReg(W_REGISTER + RF_SETUP,  (cucPower[vucPower]|cucSpeed[vucSpeed]));	     			//dbm
    rfWriteReg(W_REGISTER + RF_CH, ucChannel_Set);						//单载波频点	   
    rfWriteNRegister(W_REGISTER + BB_CAL,    BB_cal_data,  sizeof(BB_cal_data));
    rfWriteNRegister(W_REGISTER + RF_CAL2,   RF_cal2_data, sizeof(RF_cal2_data));
    rfWriteNRegister(W_REGISTER + DEM_CAL,   Dem_cal_data, sizeof(Dem_cal_data));
    rfWriteNRegister(W_REGISTER + RF_CAL,    RF_cal_data,  sizeof(RF_cal_data));
    rfWriteNRegister(W_REGISTER + DEM_CAL2,  Dem_cal2_data,sizeof(Dem_cal2_data));
    delay1us(500);	
    rfReadReg(R_REGISTER + RF_CH);
    //CE_HIGH;
}
