#ifndef _jlx12864_
#define _jlx12864_

#include "stm8s_gpio.h"
#include "stm8s_spi.h"

#define     LCD_CS      GPIO_PIN_0
#define     LCD_RES     GPIO_PIN_3
#define     LCD_RS      GPIO_PIN_2
#define     LCD_SDA     GPIO_PIN_1
#define     LCD_SCK     GPIO_PIN_5
#define     BACK_LED    GPIO_PIN_4

#define     SPI_MISO    GPIO_PIN_7
#define     SPI_MOSI    GPIO_PIN_6
#define     SPI_SCK     GPIO_PIN_5
#define     SPI_CSN     GPIO_PIN_4

#define     LCD_CS_SetL()   GPIO_WriteLow(GPIOD,LCD_CS)
#define     LCD_CS_SetH()   GPIO_WriteHigh(GPIOD,LCD_CS) 
#define     LCD_RES_SetL()  GPIO_WriteLow(GPIOC,LCD_RES)
#define     LCD_RES_SetH()  GPIO_WriteHigh(GPIOC,LCD_RES)
#define     LCD_RS_SetL()   GPIO_WriteLow(GPIOC,LCD_RS)
#define     LCD_RS_SetH()   GPIO_WriteHigh(GPIOC,LCD_RS)
#define     LCD_SDA_SetL()  GPIO_WriteLow(GPIOC,LCD_SDA)
#define     LCD_SDA_SetH()  GPIO_WriteHigh(GPIOC,LCD_SDA)
#define     LCD_SCK_SetL()  GPIO_WriteLow(GPIOE,LCD_SCK)
#define     LCD_SCK_SetH()  GPIO_WriteHigh(GPIOE,LCD_SCK)
#define     BACK_LED_Off()   GPIO_WriteLow(GPIOD,BACK_LED)
#define     BACK_LED_On()  GPIO_WriteHigh(GPIOD,BACK_LED)

#define     write_Command(cm)       write_DtOrCm(0,cm);
#define     write_Data(data)        write_DtOrCm(1,data);


void JLX12864_Init(void);
extern void delay_us(u16);
extern void delay(u16);
void write_DtOrCm(bool flag,u8 idata);
void clear_screen(void);
void point(u8 x,u8 y);
#endif
