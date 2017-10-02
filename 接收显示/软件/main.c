#include "stm8s.h"

/*-------------------------------------------------------------------------
 *      Interrupt vector numbers
 *-----------------------------------------------------------------------*/
#define AWU_vector                           0x03
#define CLK_CSS_vector                       0x04
#define CLK_SWITCH_vector                    0x04
#define EXTI0_vector                         0x05
#define EXTI1_vector                         0x06
#define EXTI2_vector                         0x07
#define EXTI3_vector                         0x08
#define EXTI4_vector                         0x09
#define SPI_CRCERR_vector                    0x0C
#define SPI_MODF_vector                      0x0C
#define SPI_OVR_vector                       0x0C
#define SPI_RXNE_vector                      0x0C
#define SPI_TXE_vector                       0x0C
#define SPI_WKUP_vector                      0x0C
#define TIM1_CAPCOM_BIF_vector               0x0D
#define TIM1_CAPCOM_TIF_vector               0x0D
#define TIM1_OVR_UIF_vector                  0x0D
#define TIM1_CAPCOM_CC1IF_vector             0x0E
#define TIM1_CAPCOM_CC2IF_vector             0x0E
#define TIM1_CAPCOM_CC3IF_vector             0x0E
#define TIM1_CAPCOM_CC4IF_vector             0x0E
#define TIM1_CAPCOM_COMIF_vector             0x0E
#define TIM2_OVR_UIF_vector                  0x0F
#define TIM2_CAPCOM_CC1IF_vector             0x10
#define TIM2_CAPCOM_CC2IF_vector             0x10
#define TIM2_CAPCOM_CC3IF_vector             0x10
#define TIM2_CAPCOM_TIF_vector               0x10
#define UART1_T_TC_vector                    0x13
#define UART1_T_TXE_vector                   0x13
#define UART1_R_IDLE_vector                  0x14
#define UART1_R_LBDF_vector                  0x14
#define UART1_R_OR_vector                    0x14
#define UART1_R_PE_vector                    0x14
#define UART1_R_RXNE_vector                  0x14
#define I2C_ADD10_vector                     0x15
#define I2C_ADDR_vector                      0x15
#define I2C_AF_vector                        0x15
#define I2C_ARLO_vector                      0x15
#define I2C_BERR_vector                      0x15
#define I2C_BTF_vector                       0x15
#define I2C_OVR_vector                       0x15
#define I2C_RXNE_vector                      0x15
#define I2C_SB_vector                        0x15
#define I2C_STOPF_vector                     0x15
#define I2C_TXE_vector                       0x15
#define I2C_WUFH_vector                      0x15
#define ADC1_AWDG_vector                     0x18
#define ADC1_AWS0_vector                     0x18
#define ADC1_AWS1_vector                     0x18
#define ADC1_AWS2_vector                     0x18
#define ADC1_AWS3_vector                     0x18
#define ADC1_AWS4_vector                     0x18
#define ADC1_AWS5_vector                     0x18
#define ADC1_AWS6_vector                     0x18
#define ADC1_AWS7_vector                     0x18
#define ADC1_AWS8_vector                     0x18
#define ADC1_AWS9_vector                     0x18
#define ADC1_EOC_vector                      0x18
#define TIM4_OVR_UIF_vector                  0x19
#define FLASH_EOP_vector                     0x1A
#define FLASH_WR_PG_DIS_vector               0x1A


#define 	STB_L()  	GPIOA-> ODR &= ~(1 << 3)
#define 	STB_H()  	GPIOA-> ODR |=(1 << 3)
#define 	CLK_L()  	GPIOB-> ODR &= ~(1 << 4)
#define 	CLK_H()  	GPIOB-> ODR |=(1 << 4)
#define 	DIO_L()  	GPIOB-> ODR &= ~(1 << 5)
#define 	DIO_H() 	GPIOB-> ODR |=(1 << 5)


u8 seg7code[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
				0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
u8 showData[8]={0};
u16 cont = 0;
u8 ledState = 0x01;

void GPIO_Init(void);
void SPI_WriteByteData(u8 data);
void SEG_Display(u8 *pArray,u8 ledState);
void TIME1_Init(void);
void delay1us(unsigned short _us);
void TM1668_Init(void);
void setLEDState(u8 sta);
u8 getKeyVlu(void);

int main(){
	GPIO_Init();	
	TM1668_Init();
	TIME1_Init();
	enableInterrupts();
    
	for(;;){
		showData[7] = cont % 10;
		showData[6] = cont % 1000 % 100 /10;
		showData[5] = cont % 1000 / 100 ;
		showData[4] = cont % 10000 /1000 ;
		showData[3] = getKeyVlu();
		SEG_Display(showData,ledState); 
	}
}

void GPIO_Init(){
	GPIOA-> DDR |=(1 << 3);
	GPIOA-> CR1 |=(1 << 3);
	GPIOA-> CR2 |=(1 << 3);
	STB_H();          //默认高电平

	GPIOB-> DDR |= (1 << 4) | (1 << 5);
	GPIOB-> CR1 |= (1 << 4) | (1 << 5);
	GPIOB-> CR2 |= (1 << 4) | (1 << 5);
	CLK_H();         //默认高电平
}

void delay1us(unsigned short _us){
	char a=0;
	while (_us--) {
		for(a=0;a<1;a++)
			;
	}
}

void SPI_WriteByteData(u8 data){
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

void TM1668_WriteData(u8 adde,u8 data){
	STB_L();
    SPI_WriteByteData(adde);
	SPI_WriteByteData(data);
	STB_H();
}



void TM1668_WtiteCom(u8 com){
	STB_L();
	SPI_WriteByteData(com);	
	STB_H();
}

void TM1668_Init(){
	u8 i;
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
	GPIOB->CR2 &= ~(1 << 5);   		//关中断
	for(i = 0;i < 2;i++){
		temp = 0x00;
		for(j = 0;j < 8;j++){
			CLK_H();
			temp >>= 1;
			CLK_L();
			if((GPIOB->IDR) & (1 << 5)) {
				temp |= 0x80; 
			}	
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

void TIME1_Init(void){
	CLK->PCKENR1 |= 0X80;      		//打开TIM1定时器时钟 

	TIM1->PSCRH = 0x07;        		//选择定时器1分频
	TIM1->PSCRL = 0xcf;   
	
	TIM1->CR1 |= (1<<2);      		//只有溢出才产生中断
	TIM1->IER |= 0x01;       		//允许更新中断
	TIM1->ARRH = 0x00;//0x03;       //填写初始值
	TIM1->ARRL = 0xe8;
	TIM1->CR1 |= 0X01;       		//使能定时器  
}



#pragma vector = TIM1_OVR_UIF_vector
__interrupt void TIM1_Interrupt(void){
	cont ++;
	if(cont % 2 == 0)
		ledState <<= 1;
	if(ledState == 0x20)
		ledState = 0x01;
	TIM1->SR1 &= ~(1<<0);//清除更新中断标志
}
