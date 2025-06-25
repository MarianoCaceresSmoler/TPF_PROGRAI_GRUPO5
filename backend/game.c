/***************************************************************************//**
  @file     +Nombre del archivo (ej: template.c)+
  @brief    +Descripcion del archivo+
  @author   +Nombre del autor (ej: Salvador Allende)+
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

// +Incluir el header propio (ej: #include "template.h")+
#include <game.h>
#include <entities.h>
#include <physics.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/



/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static void falta_envido (int);+


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static int temperaturas_actuales[4];+


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void gameInit(game_t * game)
{
	game->status = GAME_RUNNING;
	game->score = 0;
	game->currentLevel = 1;
	game->tickCounter = 0;
	game->aliensRemaining = ALIENS_COLS * ALIENS_ROWS;

}

void gameReset(game_t * game)
{
	gameInit(game);
}

void gamePause(game_t * game)
{
	game->status = GAME_PAUSED;
}

void gameResume(game_t * game)
{
	game->status = GAME_RUNNING;
}

void gameEnd(game_t * game)
{
	game->status = GAME_END;
}

void incrementScore(game_t * game, int points)
{
	game->score += points;
}

void resetLevel(game_t * game)
{
	game->tickCounter = 0;
	game->aliensRemaining = ALIENS_COLS * ALIENS_ROWS;
}

void nextLevel(game_t * game)
{
	game->currentLevel += 1;
	resetLevel(game);
}

void gameUpdate(game_t * game, input_t input)
{
	if(game->status == GAME_RUNNING)
	{
		game->tickCounter++;

		// HACER FUNCIONES Y VER SI HACE FALTA AGREGAR MAS COSAS

		// Update entities 
		update_ship(game->ship, input);
		update_bullets(game->bullets, input);
		update_aliens(game->aliens, game->tickCounter);
		update_mothership(game->mothership);

		// Collisions
		check_collisions(game);

		// Check game state

		if (player_is_dead(game->ship)) {
			gameEnd(game);
			return;
		}

		if (game->aliensRemaining == 0) {
			game->currentLevel++;
			nextLevel(game);
		}

	}

}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



 