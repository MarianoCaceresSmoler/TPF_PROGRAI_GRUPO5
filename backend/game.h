/***************************************************************************//**
  @file     +Nombre del archivo (ej: template.h)+
  @brief    +Descripcion del archivo+
  @author   +Nombre del autor (ej: Salvador Allende)+
 ******************************************************************************/

#ifndef GAME_H
#define GAME_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/



/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef enum {STILL, MOVE_RIGHT, MOVE_LEFT, SHOOT, QUIT, MOVE_UP, MOVE_DOWN} input_t;

typedef enum {GAME_ERROR = -1, GAME_MENU, GAME_RUNNING, GAME_PAUSED, GAME_END} gameStatus_t;

typedef struct 
{
    gameStatus_t status;
    int score;
    int currentLevel;
    int tickCounter;
} game_t;


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: extern unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief TODO: completar descripcion
 * @param param1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
 * @return Descripcion valor que devuelve
*/
// +ej: char lcd_goto (int fil, int col);+

void gameInit(game_t * game);
void gameReset(game_t * game);
void gamePause(game_t * game);
void gameResume(game_t * game);
void gameEnd(game_t * game);

void nextLevel(game_t * game);

void gameUpdate(input_t input, game_t game);

/*******************************************************************************
 ******************************************************************************/

#endif // GAME_H
