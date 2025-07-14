/***************************************************************************//**
  @render_rpi.h
  @Render frontend
  @Grupo_5
 ******************************************************************************/

#ifndef RENDER_RPI_H
#define RENDER_RPI_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

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
 * @brief function to init and clear the display
 * @param game pointer to game used in main
 */
int initGraphics(game_t * game);

/**
 * @brief function to cleanup graphics
 */
void cleanupGraphics(void);


/*******************************************************************************
 ******************************************************************************/

#endif // RENDER_RPI_H
