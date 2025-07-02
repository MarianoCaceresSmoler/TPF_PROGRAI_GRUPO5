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
	// game entities
    ship_t ship;
    alienFormation_t aliens;
    bullet_t alienBullet;
    bullet_t shipBullet;
    barrier_t barriers[BARRIERS];
    mothership_t mothership;
    powerUp_t powerUp[POWERUP_TYPES];

    // game variables and info
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
 * @brief function to set the state of the game before running it
 * @param game pointer to the game information
 */
void gameInit(game_t *game);

/**
 * @brief function to set the entities before a level starts
 * @param game pointer to the game information
 */
void levelInit(game_t *game);

/**
 * @brief function to pause the game, physics and entities
 * @param game pointer to the game information
 */
void gamePause(game_t *game);

/**
 * @brief function to resume a paused game
 * @param game pointer to the game information
 */
void gameResume(game_t *game);

/**
 * @brief function to finish the game
 * @param game pointer to the game information
 */
void gameEnd(game_t *game);

/**
 * @brief function to update the score
 * @param game pointer to the game information, points the number of points to add
 */
void incrementScore(game_t *game, int points);

/**
 * @brief general function to update the game's objects information
 * @param game pointer to the game information
*/
void manageInput(game_t * game, input_t input);

/*******************************************************************************
 ******************************************************************************/

#endif // GAME_H
