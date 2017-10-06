#include "stm8s.h"
#include "tm1668.h"
#include "xn297l.h"
#include "spi.h"


u8 showData[8]={1,2,3,4,5,6,7,8};
u8 cont = 0;
u8 ledState = 0x01;


int main(){
	SPIInit();
    xn297lInit();
	TM1668_Init();
	
	enterReceiveMode();   
	while(1){        
		rfReceivData(&cont);
		showData[7] = cont % 10;
		showData[6] = cont % 1000 % 100 / 10;
		showData[5] = cont % 1000 / 100;
		showData[4] = cont /1000;
		showData[3] = getKeyVlu();
		SEG_Display(showData,ledState); 
	}
}



