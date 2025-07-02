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
	FREEZE_POWERUP,
	DOUBLESHOOT_POWERUP,
	DOUBLEPOINTS_POWERUP,
	REBUILDBARRIERS_POWERUP
}
powerUpTypes_t;

typedef struct
{
	unsigned short int x;
	unsigned short int y;
	unsigned short int height;
	unsigned short int width;
	unsigned char isAlive;
	unsigned short int explosionTimer;
} entity_t;

typedef struct 
{
	entity_t entity;
	movingDirections_t direction;
	unsigned char livesLeft		: 4;
	unsigned char canShoot 		: 4;
} ship_t;

typedef struct
{
	entity_t entity;
	unsigned short int moveRate;
	unsigned char alienType;
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
	char direction;
} bullet_t;

typedef struct
{
	entity_t entity;
	unsigned char powerUpType;
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

/*******************************************************************************
 ******************************************************************************/

#endif // ENTITIES_H
