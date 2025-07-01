/***************************************************************************/ /**
   @game.h
   @Game structs and functions
   @Grupo_5
  ******************************************************************************/

#ifndef GAME_H
#define GAME_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "config.h"
#include "entities.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef enum
{
    INPUT_NONE,
    INPUT_RIGHT,
    INPUT_LEFT,
    INPUT_SHOOT,
    INPUT_PAUSE,
    INPUT_RESUME,
    INPUT_RESTART,
    INPUT_EXIT,
    INPUT_ANY
} input_t;

typedef enum
{
	GAME_ERROR = -1,
    GAME_MENU,
    GAME_RUNNING,
    GAME_PAUSED,
    GAME_END
} gameStatus_t;

typedef struct 
{
	ship_t ship;
    alienFormation_t aliens;
    bullet_t alienBullet;
    bullet_t shipBullet;
    barrier_t barriers[BARRIERS];
    mothership_t mothership;

    gameStatus_t status;
    int score;
    int currentLevel;
    int tickCounter;
    int aliensRemaining;

} game_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: extern unsigned int anio_actual;+

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief completar descripcion
 * @param param1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
 * @return Descripcion valor que devuelve
 */
// +ej: char lcd_goto (int fil, int col);+

/**
 * @brief functions to modify the state of the game
 * @param game pointer to the game information
 */
void gameInit(game_t *game);
void gameReset(game_t *game);
void gamePause(game_t *game);
void gameResume(game_t *game);
void gameEnd(game_t *game);

/**
 * @brief function to update the score
 * @param game pointer to the game information, points the number of points to add
 */
void incrementScore(game_t *game, int points);

/**
 * @brief function to reset level information when a level is completed
 * @param game pointer to the game information
 */
void resetLevel(game_t *game);

/**
 * @brief function to set the level configuration
 * @param game pointer to the game information
 */
void startLevel(game_t *game);

/**
 * @brief general function to update the game's objects information
 * @param game pointer to the game information
*/
void manageInput(game_t * game, input_t input);

/*******************************************************************************
 ******************************************************************************/

#endif // GAME_H
