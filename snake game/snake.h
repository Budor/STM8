#ifndef _SNAKE_
    #define _SNAKE_
    
#include "stm8s.h"
#include "stdlib.h"
#include "jlx12864.h"

#define  INITSIZE    4
#define  MAXSIZE    50

typedef struct note{
    u8 x;
    u8 y;
}Note;

typedef enum direction{R,L,U,D}Dir;

typedef struct snake{
    u8 sizeS;
    Note note[MAXSIZE];
    Dir dir;
	bool life;
}Snake;

extern Snake snake;
extern u8 status[8];
extern Note food;
extern u32 num;

void createFood(void);
void show(Snake *snk);
void snake_init(Snake *snk);
void snake_move(Snake *snk);
void game_over(void);
bool isEatFood(void);
bool isAfoul(void);
void addNote(void);

#endif