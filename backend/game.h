/***************************************************************************//**
  @file 	game.h
  @brief 	Game structures and functions for status and parameters 
  @author 	Grupo_5
 ******************************************************************************/

#ifndef GAME_H
#define GAME_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "config.h"
#include "entities.h"
#include "scores.h"

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

// Enum to clasify user inputs
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

// Struct to hold user inputs
typedef struct 
{
    char leftKeyPressed        : 1;
    char rightKeyPressed       : 1;
    char upKeyPressed          : 1;
    char shootKeyPressed       : 1;
    char pauseKeyPressed       : 1;
    char resumeKeyPressed      : 1;
    char restartKeyPressed     : 1;
    char exitKeyPressed        : 1;
} inputStatus_t;

// Enum to clasify game states
typedef enum
{
	GAME_ERROR = -1,
    GAME_MENU,
    GAME_LOADING,
    GAME_RUNNING,
    GAME_PAUSED,
    GAME_END
} gameStatus_t;

// General struct to control game information
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
    score_t highScores[MAX_SCORES];

    // game variables and state
    gameStatus_t status;
    char nameTag[MAX_NAME_CHARS + 1];
    int loadingTimer;
    int score;
    int scoreRank;
    int currentLevel;
    int tickCounter;
    int aliensRemaining;
    int activePowerUp[POWERUP_TYPES];
} game_t;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

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
 * @brief function to finish the game and save the score
 * @param game pointer to the game information
 */
void gameEnd(game_t *game);

/**
 * @brief function to restart the game
 * @param game pointer to the game information
 */
void gameReset(game_t *game);

/**
 * @brief general function to update the game's objects information
 * @param game pointer to the game information
 * @param input last input provided by user
*/
void gameUpdate(game_t * game, inputStatus_t input);

/*******************************************************************************
 ******************************************************************************/

#endif // GAME_H
