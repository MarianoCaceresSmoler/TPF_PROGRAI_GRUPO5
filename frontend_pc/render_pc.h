/***************************************************************************//**
  @file 	render_pc.h
  @brief 	Render functions for allegro display
  @author 	Grupo_5
 ******************************************************************************/

#ifndef RENDER_PC_H
#define RENDER_PC_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <allegro5/allegro.h>

#include "../backend/entities.h"
#include "../backend/game.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief function to create and detroy allegro graphics
 * @return 0 if success, -1 if error
 */
int initGraphics(void);

/**
 * @brief function to detroy allegro graphics
 */
void cleanupGraphics(void);

/**
 * @brief function to get the allegro event queue
 * @return the allegro event queue
 */
ALLEGRO_EVENT_QUEUE *getEventQueue(void);

/**
 * @brief function to render the game while playing
 * @param game copy of the game object
 */
void renderGame(game_t game);

/**
 * @brief function to render the game menu
 * @param game copy of the game object
 */
void renderMenu(game_t game);

/**
 * @brief function to render the game over screen
 * @param game copy of the game object
 */
void renderGameOver(game_t game);

/*******************************************************************************
 ******************************************************************************/

#endif // RENDER_PC_H
