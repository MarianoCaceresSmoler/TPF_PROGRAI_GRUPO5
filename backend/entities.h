/***************************************************************************//**
  @entities.h
  @Entity definitions and actions
  @Grupo_5
 ******************************************************************************/

#ifndef ENTITIES_H
#define ENTITIES_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "config.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef enum
{
	MOVING_LEFT = -2, 
	MOVING_UP, 
	STILL, 
	MOVING_DOWN, 
	MOVING_RIGHT
} 
movingDirections_t;

typedef enum 
{
	FREEZE_POWERUP,			// celeste
	ALIENRETREAT_POWERUP,	// rojo
	ONEUP_POWERUP,			// verde
	REBUILDBARRIERS_POWERUP	// amarillo
}
powerUpTypes_t;

typedef struct
{
	short int x;
	short int y;
	unsigned short int height;
	unsigned short int width;
	unsigned char isAlive;
	unsigned short int explosionTimer;
} entity_t;

typedef struct 
{
	entity_t entity;
	movingDirections_t direction;
	unsigned char livesLeft				: 4;
	unsigned char canShoot 				: 4;
	unsigned char invencibilityTicks;
} ship_t;

typedef struct
{
	entity_t entity;
	unsigned char alienType;
	unsigned char isMoving;
} alien_t;

typedef struct
{
	alien_t alien[ALIENS_ROWS][ALIENS_COLS];
	movingDirections_t direction;
	unsigned char canShoot;
} alienFormation_t;

typedef struct
{
	entity_t entity;
	movingDirections_t direction;
} mothership_t;

typedef struct
{
	entity_t entity;
} barrierPixel_t;

typedef struct 
{
	barrierPixel_t pixel[BARRIER_HEIGHT][BARRIER_WIDTH];
} barrier_t;

typedef struct
{
	entity_t entity;
	movingDirections_t direction;
} bullet_t;

typedef struct
{
	entity_t entity;
	powerUpTypes_t type;
	int ticksLeft;
} powerUp_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/



/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief function to create a ship type entity
 * @return returns an initialized ship in the standby position
 */
ship_t createShip();

/**
 * @brief function to create an alien formation type entity
 * @param rows amount of alien rows
 * @param columns amount of alien columns
 * @return returns an initialized alien formation in the standby position
 */
alienFormation_t createAlienFormation(int rows, int columns);

/**
 * @brief function to create a mothership type entity
 * @return returns an initialized mothership in the standby position
 */
mothership_t createMothership(); 

/**
 * @brief function to create a barrier type entity
 * @return returns an initialized barrier in the standby position
 */
barrier_t createBarrier();

/**
 * @brief function to create a bullet type entity
 * @param direction negative is upwards, positive is downwards
 * @return returns an initialized bullet in the standby position
 */
bullet_t createBullet(movingDirections_t direction);

/**
 * @brief function to create a powerUp type entity
 * @param type powerUp type
 * @return returns an initializes powerUp in the standby position
 */
powerUp_t createPowerUp(powerUpTypes_t type); 

/**
 * @brief function to move an entity in the X axis
 * @param entity is a pointer to the entity
 * @param moveRate int with the amount of pixels to move
 */
void moveEntityX(entity_t *entity, int moveRate);

/**
 * @brief function to move an entity in the Y axis
 * @param entity is a pointer to the entity
 * @param moveRate int with the amount of pixels to move
 */
void moveEntityY(entity_t *entity, int moveRate);

/**
 * @brief general function to set an entity as alive, and set it in a position
 * @param entity pointer to the entity
 * @param x position x
 * @param y position y
*/
void setEntity(entity_t * entity, int x, int y); 

/**
 * @brief function to move an entity in the Y axis
 * @param bullet is a pointer to the bullet to shoot
 * @param shootingEntity is a pointer to the entity
 */
void shootFromEntity(bullet_t *bullet, entity_t *shootingEntity);

/**
 * @brief function to set the barriers position
 * @param game pointer to the game
 */
void setBarriers(barrier_t barriers[BARRIERS]);

/**
 * @brief function to set the aliens position
 * @param game pointer to the game
 */
void setAliens(alienFormation_t *aliens);

/**
 * @brief function to set the shape of a barrier
 * @param barrier pointer to the barrier type entity
 */
void setBarrierShape(barrier_t *barrier);

/**
 * @brief function to get the column aligned with the ship's x position
 * @param aliens aliens structure to get the column
 * @param shipX ship's position in the X axis
 * @return the column closest to the ship
 */
int getNearestColumnAlive(alienFormation_t aliens, short int shipX);

/**
 * @brief function to get the nearest row alive to the ship
 * @param aliens is the aliens structure to get the row from
 * @param column is the column of aliens to check
 * @return the row with an alive alien closest to the ship
 */
int getNearestRowAlive(alienFormation_t aliens, int column);

/**
 * @brief function to get the first column with an alive alien from left to right
 * @param aliens alien formation to get the column from
 * @return first column with an alive alien starting from 0
 */
int getFirstColumnAlive(alienFormation_t aliens);

/**
 * @brief function to get the first row with an alive alien from left to right
 * @param aliens alien formation to get the row from
 * @return first row with an alive alien starting from 0
 */
int getLastColumnAlive(alienFormation_t aliens);


/*******************************************************************************
 ******************************************************************************/

#endif // ENTITIES_H
