/***************************************************************************//**
  @file 	  render_rpi.h
  @brief    Render functions for the matrix display
  @author 	Grupo_5
 ******************************************************************************/

#ifndef RENDER_RPI_H
#define RENDER_RPI_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

 #include "../backend/game.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

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
