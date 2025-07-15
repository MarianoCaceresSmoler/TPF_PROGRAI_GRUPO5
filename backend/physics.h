/***************************************************************************//**
  @file 	  physics.h
  @brief 	  Collisions and interactions between entities
  @author 	Grupo_5
 ******************************************************************************/

#ifndef PHYSICS_H
#define PHYSICS_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "game.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief general function to handle all colisions
 * @param game pointer to the game information
 * @return the points acumulated if there was a collision with an alien, -1 if there was an error
*/
int handleCollisions(game_t *game); 

/*******************************************************************************
 ******************************************************************************/

#endif // PHYSICS_H