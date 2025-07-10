/***************************************************************************/ /**
	@render_pc.h
    @Render frontend
    @Grupo_5
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
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

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
 * @brief functions to create and detroy allegro graphics
 */
void initGraphics(void);
void cleanupGraphics(void);

/**
 * @brief functions to render the game screens
 * @param game copy of the game object
 */
void renderGame(game_t game);
void renderMenu(game_t game);
void renderGameOver(game_t game);


/*******************************************************************************
 ******************************************************************************/

#endif // RENDER_PC_H
