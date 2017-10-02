#include "jlx12864.h"
#include "stm8s_clk.h"
#include "snake.h"
#include "key.h"
#include "stm8s_it.h"
#include "iostm8s103.h"

Snake snake;
Note food;

void delay_us (u16 nCount);
void delay(u16 time);
void sysClk_Init(void);


int main(){
    sysClk_Init();
    JLX12864_Init();
    KEY_init(); 
    snake_init(&snake);
	createFood();
	enableInterrupts();
	
    show(&snake);
    delay(600);
    while(snake.life){
        snake_move(&snake);
		if(isEatFood()){
			createFood();
			addNote();
		}
		if(isAfoul())
			snake.life = FALSE;
        delay(300);	
	}
	game_over();
	while(1);
}

void delay_us(u16 nCount){
  while (nCount){
    nCount--;
  }
}

void delay(u16 time){
    u16 i,j;
    for(i = 0;i < 1000;i++)
        for(j = 0;j < time;j++)
            ;
}

void sysClk_Init(){
  	CLK_HSECmd(ENABLE);
	while (CLK_GetFlagStatus(CLK_FLAG_HSERDY) == RESET); 
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO,CLK_SOURCE_HSE, DISABLE,CLK_CURRENTCLOCKSTATE_DISABLE);
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
	while (CLK_GetSYSCLKSource() != CLK_SOURCE_HSE);
}


#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
