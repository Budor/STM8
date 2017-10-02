#include "key.h"

void KEY_init(){
    GPIO_Init(GPIOB,KEY_U | KEY_R | KEY_OK,GPIO_MODE_IN_PU_NO_IT );
    GPIO_Init(GPIOA,KEY_L,GPIO_MODE_IN_PU_NO_IT );
    GPIO_Init(GPIOF,KEY_D,GPIO_MODE_IN_PU_NO_IT );
    
    TIM4_DeInit();
    TIM4_TimeBaseInit(TIM4_PRESCALER_128,20);
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
    TIM4_ARRPreloadConfig(ENABLE);
    TIM4_Cmd(ENABLE);
}

void KEY_U_event(){
    if(snake.dir == D)
        return;
    snake.dir = U;
}

void KEY_D_event(){
    if(snake.dir == U)
        return;
    snake.dir = D;

}

void KEY_R_event(){
    if(snake.dir == L)
        return;
    snake.dir = R;
}

void KEY_L_event(){
    if(snake.dir == R)
        return;
    snake.dir = L;

}