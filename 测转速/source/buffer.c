#include "config.h"
#include "buffer.h"
#include "uart.h"

u8 uartRevBuffer[8][32] = {0};
u8 uartSendBuffer[256] = {0};

u8 uartRevBufferAddr = 0; 				// 每个FIFO的数据地址
u8 uartRevBufferReadAddr = 0;
u8 uartRevBufferWriteAddr = 0;

u8 uartSendBufferCnt = 0; 				// 串口发送FIFO数据个数
u8 uartSendBufferReadAddr = 0;
u8 uartSendBufferWriteAddr = 0;

u8 uartRevBufferIdleBits[8] = {0};


void bufferInit(void) {					// 清空BUFFER
    u8 i, j;
    for (i = 0; i < 8; i++) {
        uartRevBufferIdleBits[i] = 0;
        for (j = 0; j < 32; j++) {
            uartRevBuffer[i][j] = 0;
        }
    }
    for (i = 0; i < 255; i++) {
        uartSendBuffer[i] = 0;
    }
    uartRevBufferAddr = 0;
    uartRevBufferReadAddr = 0;
    uartRevBufferWriteAddr = 0;
    uartSendBufferCnt = 0;
    uartSendBufferReadAddr = 0;
    uartSendBufferWriteAddr = 0;
}

void uartRevBufferStatusSet(void){
    //disableInterrupts();
    uartRevBufferWriteAddr++;
    if (uartRevBufferWriteAddr > 7) {
        uartRevBufferWriteAddr = 0;
    }
    uartRevBufferAddr = 0;
    //enableInterrupts();
}

void uartWriteBuffer(u8 ucData){				// 写FIFO数据
    uartRevBuffer[uartRevBufferWriteAddr][uartRevBufferAddr++] = ucData;
    if (uartRevBufferAddr > 31) {
        uartRevBufferIdleBits[uartRevBufferWriteAddr] = 32;
        uartRevBufferStatusSet();
    }
}

u8* ltReadBuffer(void){
    if (uartRevBufferReadAddr != uartRevBufferWriteAddr) { 
        if (uartRevBufferIdleBits[uartRevBufferReadAddr] != 0) {
            return uartRevBuffer[uartRevBufferReadAddr]; 		// 返回读取的FIFO首地址
        }
    }
    return NULL;
}


void ltReadBufferOK(void){				// 读取成功后清除标志
    uartRevBufferIdleBits[uartRevBufferReadAddr] = 0;
    uartRevBufferReadAddr++;
    if (uartRevBufferReadAddr > 7) {
        uartRevBufferReadAddr = 0;
    }
}

__ramfunc void ltWriteBuffer(u8* ucData, u8 ucLength){
    u8 i;
    for (i = 0; i < ucLength; i++) {
        uartSendBuffer[uartSendBufferWriteAddr++] = ucData[i];
    }
    if ((0xFF - uartSendBufferCnt) > ucLength) {
        uartSendBufferCnt += ucLength;
    } else {
        uartSendBufferCnt = 0xFF;
    }
}

__ramfunc void uartReadBuffer(void){
    if (uartSendBufferCnt > 0) {
        UARTPORT->DR = uartSendBuffer[uartSendBufferReadAddr++]; // 
        uartSendBufferCnt--;
    } else {
        uartSendBufferReadAddr = 0;
        uartSendBufferWriteAddr = 0;
    }
}

