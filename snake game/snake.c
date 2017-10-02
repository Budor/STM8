#include "snake.h"

void snake_init(Snake *snk){
    u8 i;
    for(i = 0;i < INITSIZE;i++){
        snk -> note[i].x = i;
        snk -> note[i].y = 0;
    }
    snk -> sizeS = INITSIZE;
    snk -> dir = R;
	snk ->life = TRUE;
}

void snake_move(Snake *snk){
    u8 i ;
	for(i = 0;i < snk -> sizeS -1;i++){
		snk -> note[i].x = snk -> note[i + 1].x;
		snk -> note[i].y = snk -> note[i + 1].y;
	}
	switch(snk -> dir){
	case R:
		snk -> note[i].x ++;
		break;
	case L:
		snk -> note[i].x --;
		break;
	case U:
		snk -> note[i].y --;
		break;
	case D:
		snk -> note[i].y ++;
		break;
	default: break;
	}
	if( snk -> note[snk -> sizeS - 1].x > 63 ||  snk -> note[snk -> sizeS - 1].y > 31){
		   snk->life = FALSE;
	}else{
		show(snk);
	}
}

void game_over(){
    clear_screen();
    point(31,15);
}

void show(Snake *snk){
	u8 i,j; 
    clear_screen();
	for(i = 0;i < 64;i++){
		for(j = 0;j < snk->sizeS;j++){
			if(i == food.x)
				point(food.x,food.y);
			if(i == snk ->note[j].x)
				point(snk -> note[j].x,snk -> note[j].y);
		}
		for(j = 0;j < 8;j++){
			status[j] = (u8)0;
		}
	}   
}
void createFood(){
	u8 i;
	bool flagF = TRUE;
	do{
	  srand((u16)&i + num);
	  food.x = rand() % 64;
	  food.y = rand() % 32;
	  for(i = 0;i < snake.sizeS;i++){
		  if(snake.note[i].x == food.x && snake.note[i].y == food.y) 
			  break;
		  else 
			  flagF = FALSE;
	  }
	}while(flagF);
}

bool isEatFood(){
	return (snake.note[snake.sizeS - 1].x == food.x && snake.note[snake.sizeS - 1].y == food.y) ? TRUE : FALSE;  
}

bool isAfoul(){
	u8 i;
	for(i = 0;i <snake.sizeS - 3;i++){
		if(snake.note[snake.sizeS -1].x == snake.note[i].x && snake.note[snake.sizeS -1].y == snake.note[i].y){
			return TRUE;
		}
	}
	return FALSE;
}

void addNote(){
	snake.note[snake.sizeS].x = snake.note[snake.sizeS - 1].x;
	snake.note[snake.sizeS].y = snake.note[snake.sizeS - 1].y;
    snake.sizeS++;
}