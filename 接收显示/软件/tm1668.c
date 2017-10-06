#include "tm1668.h"
#include "delay.h"

u8 seg7code[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
				0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

static void GPIO_Config(void);
static void SPI_WriteByteData(u8 data);
static void TM1668_WriteData(u8 adde,u8 data);
static void TM1668_WtiteCom(u8 com);

static void GPIO_Config(){
	GPIOA-> DDR |=(1 << 3);
	GPIOA-> CR1 |=(1 << 3);
	GPIOA-> CR2 |=(1 << 3);
	STB_H();          //默认高电平      

	GPIOB-> DDR |= (1 << 4) | (1 << 5);
	GPIOB-> CR1 |= (1 << 4) | (1 << 5);
	GPIOB-> CR2 |= (1 << 4) | (1 << 5);
	CLK_H();         //默认高电平
}

static void SPI_WriteByteData(u8 data){
	u8 i,temp;
	for(i = 0;i < 8;i++){
		CLK_L();
		temp = data & 0x01;
		if(temp == 0)
			DIO_L();
		else
			DIO_H();
		CLK_H();
		data >>=1;

	}
}

static void TM1668_WriteData(u8 adde,u8 data){
	STB_L();
    SPI_WriteByteData(adde);
	SPI_WriteByteData(data);
	STB_H();
}



static void TM1668_WtiteCom(u8 com){
	STB_L();
	SPI_WriteByteData(com);	
	STB_H();
}

void TM1668_Init(void){
	u8 i;
	GPIO_Config();
	TM1668_WtiteCom(0x03);		  	//7位10段	
    TM1668_WtiteCom(0x44);          //写显示数据  固定地址
	for(i = 0;i < 14;i ++){
		TM1668_WriteData(0xc0 + i ,0x00);
	}
	TM1668_WtiteCom(0x88+7);       //设置显示辉度
	
}

void SEG_Display(u8 *pArray,u8 ledState){
    u8 i,j;
	u8 tempData = 0;
	for(i = 0;i < 8;i++){
		for(j = 0;j < 8;j++){
			if(i == 0)pArray[j] %= 16;    	//防止数组下标越界
			tempData >>=  1;
			if(seg7code[pArray[j]] & (0x01 << i)){
			     tempData |= 0x80; 
			}				 
		}
		TM1668_WriteData(0xc0 + i + i,tempData);
		TM1668_WriteData(0xc1 + i + i,(ledState >> i) & 0x01);
	}
}

u8 getKeyVlu(void){
	u8 i,j;
	u8 temp = 0;
	u8 keyArr[2] = {0};
	STB_L();
	SPI_WriteByteData(0x42);       //读扫键数据
	delay1us(2);
	GPIOB->DDR &= ~(1 << 5);   		//输入模式
	GPIOB->CR2 &= ~(1 << 5);   		//关IO口中断
	for(i = 0;i < 2;i++){
		temp = 0x00;
		for(j = 0;j < 8;j++){		
			temp >>= 1;
			CLK_L();
			if((GPIOB->IDR) & (1 << 5)) {
				temp |= 0x80; 
			}
			CLK_H();
		} 
		delay1us(1);
		keyArr[i] = temp;	
	}
	STB_H();
	GPIOB-> DDR |= (1 << 5);
	GPIOB-> CR2 |= (1 << 5);
	if((keyArr[0] & 0x07) == 0x01)
		return 4;
	if((keyArr[0] & 0x38) == 0x08)
		return 3;
	if((keyArr[1] & 0x07) == 0x01)
		return 2;
	if((keyArr[1] & 0x38) == 0x08)
		return 1;
	return 0;
}