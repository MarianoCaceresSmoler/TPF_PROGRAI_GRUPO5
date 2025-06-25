/***************************************************************************//**
  @physics.h
  @Physics ant interactions
  @Grupo_5
 ******************************************************************************/

#ifndef _PHYSICS_H_
#define _PHYSICS_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "entities.h"
#include "game.h"

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
 * @brief: function to check a collision between two entities
 * @param entityA, entityB the two entities to compare
 * @return 1 if there is a collision, 0 if not
*/
int check_entities_collision(entity_t entityA, entity_t entityB);

/**
 * @brief general function to handle all colisions
 * @param game pointer to the game information
*/
void handle_collisions(game_t *game); 

/**
 * @brief functions to check each possible collision individually
 * @param game pointer to the game information
*/
void check_bullet_hits_aliens(game_t *game);
void check_bullet_hits_player(game_t *game);
void check_bullet_hits_barriers(game_t *game);


/*******************************************************************************
 ******************************************************************************/

#endif // _PHYSICS_H_