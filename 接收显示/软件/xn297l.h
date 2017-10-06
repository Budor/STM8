#ifndef __XN297L_H
#define __XN297L_H
///////////////////////////////////////////////////////////////////////////////
//                  �����û�Ӧ�ã����²��ֿ�����Ҫ�޸�                       //
///////////////////////////////////////////////////////////////////////////////
/******************** Control I/O *****************************/
#define paOff()                        noFunction()
#define paTXOn()					   noFunction()
#define paRXOn()                       noFunction()
#define rfCE_Low()                     GPIOD->ODR &= ~(1 << 3)
#define rfCE_High()                    GPIOD->ODR |= (1 << 3)

#define spiCSOn()                      GPIOD->ODR &= ~(1 << 2)               
#define spiCSOff()                     GPIOD->ODR |= (1 << 2)

#define rfIrqIsHigh()                  (GPIOC->IDR & (1 << 4)) 
////////////////////////////////////////////////////////////////////////////////
//                    ����Ϊ�������֣��������޸�                              //
////////////////////////////////////////////////////////////////////////////////
#define         TRANS_ENHANCE_MODE    1                                         //����������ǿ��
#define         TRANS_BURST_MODE      2                                         //����������ͨ��

#define         RF13dBm                         0x3F                            // 13dBm 
#define         RF10dBm                         0X0F                            // 10dBm 
#define         RF8dBm                          0x15                            // 8dbm      
#define         RF7dBm                          0x07                            // 7dbm   
#define         RF5dBm                          0x2c                            // 5dbm   
#define         RF4dBm                          0x06                            // 4dbm   
#define         RF2dBm                          0x05                            // 2dbm  
#define         RF0dBm                          0X0B                            // 0dBm  
#define         RF_3dBm                         0x04                            // -3dBm     
#define         RF_6dBm                         0x0A                            // -6dBm 
#define         RF_10dBm                        0x02                            // -10dBm 
#define         RF_18dBm                        0x01                            // -18dBm 
#define         RF_30dBm                        0x00                            // -30dBm 


/********************SPI  REGISTER  ********************/
#define		R_REGISTER			0x00                            //SPI read RF data
#define		W_REGISTER			0x20                            //SPI write RF data
#define		R_RX_PAYLOAD		        0x61                            //Read RX Payload
#define		W_TX_PAYLOAD		        0xA0                            //Write TX Payload
#define		FLUSH_TX			0xE1                            //Flush RX FIFO
#define		FLUSH_RX			0xE2                            //Flush TX FIFO
#define		REUSE_TX_PL			0xE3                            //Reuse TX Payload
#define		ACTIVATE			0x50                            //ACTIVATE
#define		DEACTIVATE			0x50                            //DEACTIVATE
#define		R_RX_PL_WID			0x60                            //Read width of RX data 
#define		W_ACK_PAYLOAD		        0xA8                            //Data with ACK
#define		W_TX_PAYLOAD_NOACK	        0xB0                            //TX Payload no ACK Request
#define		CE_FSPI_ON	                0xFD                            // CE HIGH
#define		CE_FSPI_OFF	                0xFC                            // CE LOW
#define		RST_FSPI	                0x53                            // RESET
#define		NOP_N				0xFF
             
/******************CONTROL  REGISTER*******************/
#define		CONFIG                          0x00            
#define		EN_AA				0x01
#define		EN_RXADDR			0x02
#define		SETUP_AW			0x03
#define		SETUP_RETR			0x04
#define		RF_CH				0x05
#define		RF_SETUP			0x06
#define		STATUS				0x07
#define		OBSERVE_TX			0x08
#define		RPD			        0x09
#define		RX_ADDR_P0			0x0A
#define		RX_ADDR_P1			0x0B
#define		RX_ADDR_P2			0x0C
#define		RX_ADDR_P3			0x0D
#define		RX_ADDR_P4			0x0E
#define		RX_ADDR_P5			0x0F
#define		TX_ADDR				0x10
#define		RX_PW_P0			0x11
#define		RX_PW_P1			0x12
#define		RX_PW_P2			0x13
#define		RX_PW_P3			0x14
#define		RX_PW_P4			0x15
#define		RX_PW_P5			0x16
#define		FIFO_STATUS			0x17
#define		DEM_CAL				0x19
#define         RF_CAL2				0x1A
#define         DEM_CAL2			0x1B
#define		DYNPD				0x1C
#define		FEATURE				0x1D	
#define		RF_CAL				0x1E
#define		BB_CAL				0x1F

/////////////////////////////////////////////////////////////////////////////////////
/*************************CONTROL CMD****************************************/
#define         DR_1M                          0X00				//ͨ������ 1Mbps
#define         DR_2M                          0X40				//ͨ������ 2Mbps
#define         DR_250K                        0XC0				//ͨ������ 2Mbps

#define STATUS_STB1                    0x02
#define STATUS_STB3                    0x03
#define STATUS_RX                      0x04
#define STATUS_TX                      0x05



/******************Parameter define **********************/
        

#define         DEFAULT_CHANNEL                100				//��ʼ��ʱ��Ƶ�ʣ� 2478 MHz           
#define         PAYLOAD_WIDTH                  32				//Payload��ȣ�     8bytes				
#define         TRANSMIT_TYPE                 TRANS_ENHANCE_MODE 		//ʹ����ǿ��ģʽ           TRANS_BURST_MODE    TRANS_ENHANCE_MODE
#define         DATA_RATE                      DR_250K                            //ͨ������2Mbps            DR_2M    DR_1M
#define         RF_POWER                      (RF13dBm |DATA_RATE)		//���书��13dBm 


#define FREQ_MIN                       0
#define FREQ_MAX                       0x7F
/****************************************end of file********************************************/
extern u8 ucStatus;
extern u8 vucChannal;
extern u8 vuctTID[5];
extern u8 vuctRID[5];
extern u8 vucRetry;
extern u8 vucBack;
extern u8 vucAck;
extern u8 vucPower;
extern u8 vucSpeed;
extern u8 vucBaud;

void xn297lInit(void);
void clearTXRXFifo(void);
void enterSendMode(void);
void enterReceiveMode(void);
u8 rfReceivData(u8 *pucData);
u8 rfSendNData(u8 *ucData, u8 ucLength);
void enterPowerDownMode(void);
void rfTestMode( u8 ucChannel_Set);

#endif


