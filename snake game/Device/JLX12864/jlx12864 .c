#include "jlx12864.h"

void JLX12864_Init(){
    GPIO_Init(GPIOD,LCD_CS | BACK_LED,GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(GPIOC,LCD_RES | LCD_RS | LCD_SDA,GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(GPIOE,LCD_SCK,GPIO_MODE_OUT_PP_LOW_FAST);
    
    LCD_RES_SetL();
    delay((u16)2);
    LCD_RES_SetH();
    delay((u16)2);
    write_Command(0xe2);   
    write_Command(0x2c);   
    write_Command(0x2e);
    write_Command(0x2f);
    
    write_Command(0x24);
    write_Command(0x81);
    write_Command(0x15);
    write_Command(0xa2);
    write_Command(0xc8);
    write_Command(0xa0);        //0xA0：常规：列地址从左到右，0xA1：反转：列地址从右到左
    write_Command(0x40);
    write_Command(0xaf);
}

/**
    写数据或命令
*/
void write_DtOrCm(bool flag,u8 idata){
	u8 i;
    LCD_CS_SetL();
    if(flag)
        LCD_RS_SetH(); 
    else
        LCD_RS_SetL();
    for(i = 0;i < 8;i++){
        LCD_SCK_SetL();
        if(idata & 0x80)
            LCD_SDA_SetH();
        else
            LCD_SDA_SetL();
        LCD_SCK_SetH();
        idata <<= 1;
    }
    LCD_CS_SetH();
}

void lcd_address(u8 page,u8 column){
    write_Command(0xb0 + page);
    write_Command(0x10 +((column >> 4) & 0x0f));
    write_Command(column & 0x0f);
}

static u8 status;
void point(u8 x,u8 y){
    u8 page,column;
    if(!((x <64) && (y < 32)))
        return;
    if(!(y % 4))
        status = 0;
    page = y / 4;
    column = x * 2;
    lcd_address(page,column);
    switch(y % 4){
        case 0:
            status |= 0x03;
            write_Data(status);
            write_Data(status);
            break;
        case 1:
            status |= 0x0c;
            write_Data(status);
            write_Data(status);
            break;
        case 2:
            status |= 0x30;
            write_Data(status);
            write_Data(status);
            break;
        case 3:
            status |= 0xc0;
            write_Data(status);
            write_Data(status);
            break;
         default:
            break;   
    }
    
    
    
}

void clear_screen(){
    u8 i,j;
    status = 0;
    for(i = 0;i<8;i++){
        lcd_address(i,0);
        for(j = 0;j < 128;j++){
            write_Data(0x00);
        }
    }
    LCD_CS_SetH();
}