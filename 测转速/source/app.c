#include "config.h"
#include "app.h"
#include "buffer.h"
#include "xn297l.h"
#include "uart.h"
#include "dataStruct.h"
#include "timer.h"

u8 systemStatus; 				// 系统状态
static u8 ucDataBufferTemp[40];
static u8 ltSendIndex;

void appCheckLtReceive(void){
    u8 ucLength;
    u8 ucFlagSend = 0;
	
    if ((systemStatus & SYSTEM_STATUS_BUSY) == 0) {
        systemStatus |= SYSTEM_STATUS_BUSY;
        ucLength = rfReceivData(ucDataBufferTemp);
        if (ucLength > 0) {   
			ledOn(); 							// 接收数据指示灯
			if (uartSendBufferCnt == 0) {
				ucFlagSend = 1; 				// 决定是否需要启动发送第一次
			}
			disableInterrupts();				// 避免串口中断
			ltWriteBuffer(ucDataBufferTemp, ucLength); 		// 复制有效数据
			enableInterrupts();
            if (ucFlagSend == 1) {
                uartReadBuffer(); 				// 启动一次发送
            }
        }
        enterReceiveMode();
        systemStatus &= ~SYSTEM_STATUS_BUSY;
        ledOff();
    }
}

void appCheckUartReceive(void){
    u8 * pucData;
    u8 ucSendFlag;
    pucData = ltReadBuffer();
    if ((pucData != NULL) && ((systemStatus & SYSTEM_STATUS_BUSY) == 0)) {
        systemStatus |= SYSTEM_STATUS_BUSY;
        //timer4Stop();
        ucSendFlag = appSendNData(pucData, uartRevBufferIdleBits[uartRevBufferReadAddr]);
        if (ucSendFlag != 0) {
            ltReadBufferOK();
            ltSendIndex++;
        }
        //timer4Run();
        systemStatus &= ~SYSTEM_STATUS_BUSY;
    }
}

u8 appSendNData(u8 *pucData, u8 ucLength){
    u8 ucFlag = 0;

    disableIRQInterrupt();
    enterSendMode();
    if ((rfSendNData(pucData, ucLength) & (1<<5)) != 0) {
        ledOn();
        if (vucBack == 1) {
            UARTPORT->DR = 'S';
        }
        ucFlag = 1;
    } else {
        ucFlag = 2;
        if (vucBack == 1) {
            UARTPORT->DR = 'F';
        }
    }
    enterReceiveMode();
    enableIRQInterrupt();
    ledOff();
    return ucFlag;
}

u8 appReceiveNData(u8 *pucData){
    return 0;
}
// 计算CRC校验值
u8 calcCrcData(u8 *pucData, u8 ucLength){
    u8 i;
    u8 CRC = 0;
    for (i = 0; i < ucLength; i++) {
        CRC ^= pucData[i]; // 进行CRC校验值计算
    }
    return CRC;
}

// 搜索指定字符串  返回字符串位置 　未返回表示未搜索到
u8 appSearchString(u8 *pucSur, u8 *pucDst, u8 ucSurLength, u8 ucDstLength){  
    u8 j;
    u8 ucIndex = 0;
    u8 ucFlag = 0;
    if (ucSurLength >= ucDstLength) {
        for (ucIndex = 0; ucIndex < (ucSurLength - ucDstLength + 1); ucIndex++) {
            ucFlag = 0;
            if (pucSur[ucIndex] == pucDst[0]) { 		// 如果发现匹配字符串
                ucFlag++;
                for (j = 1; j < ucDstLength; j++) {
                    if (pucSur[ucIndex+j] == pucDst[j]) {
                        ucFlag++;
                        if (ucFlag == ucDstLength) {
                            if (ucIndex > 20) {
                                ucIndex = 0xFF;
                            }
                            return ucIndex; // 返回位置
                        }
					}
                }
            }
        }   
    }
    return 0xFF; // 返回0xFF表示字符串不存在    
}

u8 checkData(u8 *pucData, u8 ucLength){
    u8 i = 0;
    u8 ucFlag = 0;
    for (i = 0; i < ucLength; i++) {
        if ((pucData[i] >= '0' && pucData[i] <= '9') ||\
            (pucData[i] >= 'A' && pucData[i] <= 'F')) {
            ucFlag++;    
        }
    }
    if (ucFlag == ucLength){
        return 0;
    } else {
        return 0xFF;
    }
}
u8 asciiToHexHalf(u8 ucData){
    if (ucData >= '0' && ucData <= '9') {
        return (ucData - '0');
    } else if (ucData >= 'A' && ucData <= 'F') {
        return (ucData - 'A' + 10);
    } else {
        return 0xFF;
    }
}

u8 asciiToHex(u8 *pucData){
    return ((asciiToHexHalf(pucData[0])<<4) + asciiToHexHalf(pucData[1]));
}

u8 searchCMD(u8 *pucString, u8 ucLength){		// 寻找CMD命令并执行
    u8 ucResult = RETURN_NO;
    u8 ucSearch;
    u8 ucHex = 0;
    ucSearch = appSearchString(pucString, "AT+BAUD", 32, 7);
    if (ucSearch != 0xFF) {
        if (pucString[ucSearch+7] == '=') {
            if (((pucString[ucSearch+8] > UART_BAUD_MIN) && (pucString[ucSearch+8] < UART_BAUD_MAX)) ||\
                (pucString[ucSearch+8] == UART_BAUD_1200) ||\
                (pucString[ucSearch+8] == UART_BAUD_31250)\
                ) {
                vucBaud = pucString[ucSearch+8];
                saveBaud(vucBaud);
                ucResult = RETURN_YES;
            }
        } else if (pucString[ucSearch+7] == '?') {
            ucResult = RETURN_GET_BAUD;
        }
    } 
    if (ucResult == RETURN_NO){
        ucSearch = appSearchString(pucString, "AT+FREQ", 32, 7);
        if (pucString[ucSearch+7] == '=') {
            if (ucSearch != 0xFF) {
                if (checkData(&pucString[ucSearch+8], 2) == 0) {
                    ucHex = asciiToHex(&pucString[ucSearch+8]);
                    if (ucHex <= FREQ_MAX) {
                        vucChannal = ucHex;
                        saveFreq(vucChannal);
                        ucResult = RETURN_YES;
                    }
                }
            }
        } else if (pucString[ucSearch+7] == '?') {
            ucResult = RETURN_GET_FREQ;
        }
    }
    if (ucResult == RETURN_NO) {
        ucSearch = appSearchString(pucString, "AT+TID", 32, 6);
        if (pucString[ucSearch+6] == '=') {
            if (ucSearch != 0xFF) {
                if (checkData(&pucString[ucSearch+7], 10) == 0) {
                    ucHex = asciiToHex(&pucString[ucSearch+7]);
                    saveTID0(ucHex);
                    ucHex = asciiToHex(&pucString[ucSearch+9]);
                    saveTID1(ucHex);
                    ucHex = asciiToHex(&pucString[ucSearch+11]);
                    saveTID2(ucHex);
                    ucHex = asciiToHex(&pucString[ucSearch+13]);
                    saveTID3(ucHex);
                    ucHex = asciiToHex(&pucString[ucSearch+15]);
                    saveTID4(ucHex);
                    getTxAddr();
                    ucResult = RETURN_YES;
                }
            }
        } else if (pucString[ucSearch+6] == '?') {
            ucResult = RETURN_GET_TID;
        }
    }
    if (ucResult == RETURN_NO) {
        ucSearch = appSearchString(pucString, "AT+RID", 32, 6);
        if (pucString[ucSearch+6] == '=') {
            if (ucSearch != 0xFF) {
                if (checkData(&pucString[ucSearch+7], 10) == 0) {
                    ucHex = asciiToHex(&pucString[ucSearch+7]);
                    saveRID0(ucHex);
                    ucHex = asciiToHex(&pucString[ucSearch+9]);
                    saveRID1(ucHex);
                    ucHex = asciiToHex(&pucString[ucSearch+11]);
                    saveRID2(ucHex);
                    ucHex = asciiToHex(&pucString[ucSearch+13]);
                    saveRID3(ucHex);
                    ucHex = asciiToHex(&pucString[ucSearch+15]);
                    saveRID4(ucHex);
                    getRxAddr();
                    ucResult = RETURN_YES;
                }
            }
        } else if (pucString[ucSearch+6] == '?') {
            ucResult = RETURN_GET_RID;
        }
    }
    
    if (ucResult == RETURN_NO) {
        ucSearch = appSearchString(pucString, "AT+RETRY", 32, 8);
        if (pucString[ucSearch+8] == '=') {
            if (ucSearch != 0xFF) {
                if (checkData(&pucString[ucSearch+9], 2) == 0) {
                    ucHex = asciiToHex(&pucString[ucSearch+9]);
                    // 限制过大的重传数
                    if (ucHex < 2) {
                        ucHex = 0x02;
                    }
                    if (ucHex > 0x0F) {
                        ucHex = 0x0F;
                    }
                    saveRetry(ucHex);
                    vucRetry = getRetry();
                    ucResult = RETURN_YES;
                }
            }
        } else if (pucString[ucSearch+8] == '?') {
            ucResult = RETURN_GET_RETRY;
        }
    }
    
    if (ucResult == RETURN_NO) {
        ucSearch = appSearchString(pucString, "AT+RESET", 32, 8);
        if (ucSearch != 0xFF) {
            #if HW_VER_NOPA_ET==0
            saveBaud(DEFAULT_BAUD);
            saveBack(DEFAULT_BACK);
            saveAck(DEFAULT_ACK);
            saveFreq(DEFAULT_FREQ);
            saveRetry(DEFAULT_RETRY);
            savePower(DEFAULT_POWER);
            saveSpeed(DEFAULT_SPEED);
            saveTID0(DEFAULT_TID0);
            saveTID1(DEFAULT_TID1);
            saveTID2(DEFAULT_TID2);
            saveTID3(DEFAULT_TID3);
            saveTID4(DEFAULT_TID4);
            saveRID0(DEFAULT_RID0);
            saveRID1(DEFAULT_RID1);
            saveRID2(DEFAULT_RID2);
            saveRID3(DEFAULT_RID3);
            saveRID4(DEFAULT_RID4);
            getTxAddr();
            getRxAddr();
            vucChannal = getFreq();
            vucRetry = getRetry();
            vucBack = getBack();
            vucAck = getAck();
            vucPower = getPower();
            vucSpeed = getSpeed();
            vucBaud = getBaud();
            ucResult = 0xFF;
            #else
            saveFlag(0xFF);
            vucChannal = DEFAULT_FREQ;
            vucRetry = DEFAULT_RETRY;
            vucBack = DEFAULT_BACK;
            vucAck = DEFAULT_ACK;
            vucPower = DEFAULT_POWER;
            vucSpeed = DEFAULT_SPEED;
            vucBaud = DEFAULT_BAUD;
            ucResult = 0xFF;
            vuctTID[0] = DEFAULT_TID0;vuctTID[1] = DEFAULT_TID1;vuctTID[2] = DEFAULT_TID2;vuctTID[3] = DEFAULT_TID3;vuctTID[4] = DEFAULT_TID4;
            vuctRID[0] = DEFAULT_RID0;vuctRID[1] = DEFAULT_RID1;vuctRID[2] = DEFAULT_RID2;vuctRID[3] = DEFAULT_RID3;vuctRID[4] = DEFAULT_RID4;
            #endif
            appDisplayReset();
        }
    }
    if (ucResult == RETURN_NO) {
        ucSearch = appSearchString(pucString, "AT+BACK", 32, 7);
        if (pucString[ucSearch+7] == '=') {
            if (ucSearch != 0xFF) {
                if (checkData(&pucString[ucSearch+8], 1) == 0) {
                    ucHex = asciiToHexHalf(pucString[ucSearch+8]);
                    if (ucHex < 2) {
                        saveBack(ucHex);
                        vucBack = getBack();
                        ucResult = RETURN_YES;
                    }
                }
            }
        } else if (pucString[ucSearch+7] == '?') {
            ucResult = RETURN_GET_BACK;
        }
    }
    if (ucResult == RETURN_NO) {
        ucSearch = appSearchString(pucString, "AT+TEST", 32, 7);
        if (ucSearch != 0xFF) {
            ucResult = RETURN_YES;
            appTest();
            ucResult = 0xFF;
        }
    }
    if (ucResult == RETURN_NO) {
        ucSearch = appSearchString(pucString, "AT+INF", 32, 6);
        if (ucSearch != 0xFF) {
            ucResult = 0xFF;
            appDisplayLog();
        }
    }
    if (ucResult == RETURN_NO) {
        ucSearch = appSearchString(pucString, "AT+UPDATE", 32, 9);
        if (ucSearch != 0xFF) {
            ucResult = 0xFF;
            appUpdate();
        }
    }
    if (ucResult == RETURN_NO) {
        ucSearch = appSearchString(pucString, "AT+ACK", 32, 6);
        if (pucString[ucSearch+6] == '=') {
            if (ucSearch != 0xFF) {
                if (checkData(&pucString[ucSearch+7], 1) == 0) {
                    ucHex = asciiToHexHalf(pucString[ucSearch+7]);
                    if (ucHex < 2) {
                        saveAck(ucHex);
                        vucAck = getAck();
                        ucResult = RETURN_YES;
                    }
                }
            }
        } else if (pucString[ucSearch+6] == '?') {
            ucResult = RETURN_GET_ACK;
        }
    }
    if (ucResult == RETURN_NO) {
        ucSearch = appSearchString(pucString, "AT+POWER", 32, 8);
        if (pucString[ucSearch+8] == '=') {
            if (ucSearch != 0xFF) {
                if (checkData(&pucString[ucSearch+9], 2) == 0) {
                    ucHex = asciiToHex(&pucString[ucSearch+9]);
                    // 限制过大的重传数
                    if (ucHex > 12) {
                        ucHex = 12;
                    }
                    savePower(ucHex);
                    vucPower = getPower();
                    ucResult = RETURN_YES;
                }
            }
        } else if (pucString[ucSearch+8] == '?') {
            ucResult = RETURN_GET_POWER;
        }
    }
    if (ucResult == RETURN_NO) {
        ucSearch = appSearchString(pucString, "AT+SPEED", 32, 8);
        if (pucString[ucSearch+8] == '=') {
            if (ucSearch != 0xFF) {
                if (checkData(&pucString[ucSearch+9], 1) == 0) {
                    ucHex = asciiToHexHalf(pucString[ucSearch+9]);
                    // 限制过大的重传数
                    if (ucHex > 2) {
                        ucHex = 2;
                    }
                    saveSpeed(ucHex);
                    vucSpeed  = getSpeed();
                    ucResult = RETURN_YES;
                }
	    }
        } else if (pucString[ucSearch+8] == '?') {
            ucResult = RETURN_GET_SPEED;
        }
    }
    return ucResult;
}

u8 hexToAscii(u8 ucHex){
    if (ucHex < 10) {
        return (ucHex + '0');
    } else if (ucHex < 0x10) {
        return ((ucHex - 0x0A) + 'A');
    } else {
        return 'X';
    }
}

void writeHexToUart(u8 ucHex){
    u8 ucDataTemp[2] = {0};
    ucDataTemp[0] = hexToAscii(ucHex>>4);
    ucDataTemp[1] = hexToAscii(ucHex&0x0F);
    wirteNData(ucDataTemp, 2);
}

void wirteNData(const u8 *pucData, u8 ucNumber){
    ltWriteBuffer((u8*)pucData, ucNumber);
}

void getTxAddr(void){
    u8 i = 0;
    for (i = 0; i < 5; i++) {
        vuctTID[i] = getData(STRUCT_TID0_ADDR + i);
    }
}
void getRxAddr(void){
    u8 i = 0;
    for (i = 0; i < 5; i++) {
        vuctRID[i] = getData(STRUCT_RID0_ADDR + i);
    }
}

void appCheckUartReceiveAtCMD(void){
    u8 * pucData;
    u8 ucResult;
    u8 i;
    u8 ucSendData;
    pucData = ltReadBuffer();
    if (pucData != NULL) {
        ucResult = searchCMD(pucData, 32);   
        if (ucResult == RETURN_NO) {			// 返回设置结果
            wirteNData("N\r\n",3);
            uartReadBuffer(); 					// 启动一次发送
            ucResult = 0xFF;
        } else if (ucResult == RETURN_YES) {
            wirteNData("Y\r\n",3);
            uartReadBuffer();					// 启动一次发送
            ucResult = 0xFF;
        } else if (ucResult == RETURN_GET_BAUD) {
            ucSendData = vucBaud;
            wirteNData("\r\nBAUD = ",9);
            wirteNData(&ucSendData, 1);
            wirteNData("\r\n",2);
            uartReadBuffer(); 					// 启动一次发送
            ucResult = 0xFF;
        } else if (ucResult == RETURN_GET_FREQ) {
            ucSendData = getFreq();
            wirteNData("\r\nFREQ = 0x",11);
        } else if (ucResult == RETURN_GET_TID) {
            wirteNData("\r\nTID = 0x",10);
            writeHexToUart(getTID0());
            writeHexToUart(getTID1());
            writeHexToUart(getTID2());
            writeHexToUart(getTID3());
            ucSendData = getTID4();
        }  else if (ucResult == RETURN_GET_RID) {
            wirteNData("\r\nRID = 0x",10);
            writeHexToUart(getRID0());
            writeHexToUart(getRID1());
            writeHexToUart(getRID2());
            writeHexToUart(getRID3());
            ucSendData = getRID4();
        } else if (ucResult == RETURN_GET_RETRY) {
            wirteNData("\r\nRETRY = 0x",12);
            ucSendData = getRetry();
        } else if (ucResult == RETURN_GET_POWER) {
            wirteNData("\r\nPOWER = 0x",12);
            ucSendData = getPower();
        }  else if (ucResult == RETURN_GET_SPEED) {
            if (vucSpeed == 0) {
                wirteNData("\r\nSPEED = 0(250K)",17);
            } else if (vucSpeed == 1) {
                wirteNData("\r\nSPEED = 1(1M)",15);
            }  else if (vucSpeed == 2) {
                wirteNData("\r\nSPEED = 2(2M)",15);
            } 
            uartReadBuffer(); 					// 启动一次发送
            ucResult = 0xFF;
        } else if (ucResult == RETURN_GET_BACK) {
            if (vucBack == 0) {
                wirteNData("\r\nBACK = 0(OFF)",15);
            } else {
                wirteNData("\r\nBACK = 1(ON)",14);
            }
            uartReadBuffer(); 					// 启动一次发送
            ucResult = 0xFF;
        } else if (ucResult == RETURN_GET_ACK) {
            if (vucAck == 0) {
                wirteNData("\r\nACK = 0(OFF)",14);
            } else {
                wirteNData("\r\nACK = 1(ON)",13);
            }
            uartReadBuffer(); 					// 启动一次发送
            ucResult = 0xFF;
        } else if (ucResult == RETURN_RESET) {
        
        } else if (ucResult == RETURN_TEST) {
        
        }
        if (ucResult != 0xFF) {
            writeHexToUart(ucSendData); 		// 复制有效数据
            wirteNData("\r\n",2);
            uartReadBuffer(); 					// 启动一次发送
        }
        uartRevBufferIdleBits[0] = 0; 
        uartRevBufferAddr = 0; 					// 每个FIFO的数据地址
        uartRevBufferReadAddr = 0;
        uartRevBufferWriteAddr = 0;
        for (i = 0; i < 32; i++) {
            uartRevBuffer[0][i] = 0;
        }
    }
}
         
void appDisplayLog(void){
    const u8 ucLogInf[] = 
#if HW_VER_NOPA==1
// V1.06 2016年6月16日 修改优先级    修改中断由下降沿触发 
// V1.07 2016年7月13日 将串口初始化调到上电闪灯后 
// V1.08 2016年7月16日 当发送端不停的发送数据时   如果接收模块接收到数据  会死机     
// V1.09 2016年8月3日  SETUP_RETR设置错误
// V1.10 2016年8月16日 增加CMD脚复用功能,可以对 arduino产生复位信号
// V1.11 2016年9月10日 再次优化透传性能 降低状态切换延时
        
// V1.12 2016年11月2日 恩田升级 修改波特率固定为38400的问题       
	"\r\nThank You For Choosing GH-xUART\r\n\
	Version:1.11\r\n\
	Data:2016-09-10\r\n\
	http:\\\\www.ghtech.cn\r\n";
#elif HW_VER_NOPA_ET==1
	"\r\nThank You For Choosing GH-xUART-ET\r\n\
	Version:1.12\r\n\
	Data:2016-11-02\r\n\
	http:\\\\www.ghtech.cn\r\n";
#elif  HW_VER_PA_USB==1
	"\r\nThank You For Choosing GH-PA-USB\r\n\
	Version:1.02\r\n\
	Data:2016-06-14\r\n\
	http:\\\\www.ghtech.cn\r\n";
#elif  HW_VER_PA==1
	"\r\nThank You For Choosing GH-PA\r\n\
	Version:1.02\r\n\
	Data:2016-06-14\r\n\
	http:\\\\www.ghtech.cn\r\n";
#endif
		
#ifdef HW_VER_DOWNLOAD
    const u8 ucLogInf2[] = "Upload Support For Arduino\r\n";
#endif
    wirteNData(ucLogInf, sizeof(ucLogInf)-1); 		// 复制有效数据
#ifdef HW_VER_DOWNLOAD
    wirteNData(ucLogInf2, sizeof(ucLogInf2)-1);  	// 复制有效数据
#endif
    uartReadBuffer(); 								// 启动一次发送
}

void appDisplayReset(void){
    const u8 ucLogInf[] = 
#if HW_VER_NOPA==1
	"\r\nModule reseted\r\n\
	\r\nBAUD = 9600bps\r\n\
	FREQ = 2478MHz\r\n\
	TID = 0x5555555555\r\n\
	RID = 0x5555555555\r\n\
	RETRY = 0x0A\r\n\
	BACK = 0(OFF)\r\n\
	ACK = 1(ON)\r\n\
	SPEED = 0(250K)\r\n\
	POWER = 0(+13dBm)\r\n\
	";
#elif HW_VER_NOPA_ET==1
	"\r\nModule reseted\r\n\
	\r\nData not be save\r\n\
	\r\nBAUD = 9600bps\r\n\
	FREQ = 2478MHz\r\n\
	TID = 0x5555555555\r\n\
	RID = 0x5555555555\r\n\
	RETRY = 0x0A\r\n\
	BACK = 0(OFF)\r\n\
	ACK = 1(ON)\r\n\
	SPEED = 0(250K)\r\n\
	POWER = 0x0B(-18dBm)\r\n\
	";
#elif  HW_VER_PA_USB==1
	"\r\nModule reseted\r\n\
	\r\nBAUD = 9600bps\r\n\
	FREQ = 2478MHz\r\n\
	TID = 0x5555555555\r\n\
	RID = 0x5555555555\r\n\
	RETRY = 0x0A\r\n\
	BACK = 0(OFF)\r\n\
	ACK = 1(ON)\r\n\
	SPEED = 0(250K)\r\n\
	POWER = 0(+20dBm)\r\n\
	";
	#elif  HW_VER_PA==1
	"\r\nModule reseted\r\n\
	\r\nBAUD = 9600bps\r\n\
	FREQ = 2478MHz\r\n\
	TID = 0x5555555555\r\n\
	RID = 0x5555555555\r\n\
	RETRY = 0x0A\r\n\
	BACK = 0(OFF)\r\n\
	ACK = 1(ON)\r\n\
	SPEED = 0(250K)\r\n\
	POWER = 0(+20dBm)\r\n\
	";
#endif
    wirteNData(ucLogInf, sizeof(ucLogInf)-1); // 复制有效数据
    uartReadBuffer(); // 启动一次发送   
}

void appUpdate(void){
    const u8 ucLogInf[] = "\r\nThe moudle enters in Update Mode...\r\n";
    wirteNData(ucLogInf, sizeof(ucLogInf)-1); 				// 复制有效数据
    uartReadBuffer(); 										// 启动一次发送  
    saveUpdate();
    Init_Iwdg();
    while(1);   
}


void appTest(void){
    const u8 ucLogInf[] = "\r\nThe moudle enters in Test Mode...\r\n";
    wirteNData(ucLogInf, sizeof(ucLogInf)-1); // 复制有效数据
    uartReadBuffer(); // 启动一次发送   
    delay1ms(150);
    disableInterrupts();
    rfTestMode(vucChannal);
    while(1) {
        resetIWDG();
        ledOn();
        delay1ms(500);
        ledOff();
        delay1ms(500);
    }
}

