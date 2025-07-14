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

// Enum for directions of movement
typedef enum
{
	MOVING_LEFT = -2, 
	MOVING_UP, 
	STILL, 
	MOVING_DOWN, 
	MOVING_RIGHT
} 
movingDirections_t;

// Enum for powerup types
typedef enum 
{
	FREEZE_POWERUP,			// blue
	ALIENRETREAT_POWERUP,	// red
	ONEUP_POWERUP,			// green
	REBUILDBARRIERS_POWERUP	// yellow
}
powerUpTypes_t;

// General struct for all entities (used for simplification in all elements )
typedef struct
{
	short int x; // position in x
	short int y; // position in y
	unsigned short int height; // height of the entity
	unsigned short int width; // width of the entity
	unsigned char isAlive; // to save if the entity is alive or not
	unsigned short int explosionTimer; // used to animate the explosion of the entity
} entity_t;

// Ship player struct
typedef struct 
{
	entity_t entity; // general entity struct
	movingDirections_t direction; // to save the moving direction
	unsigned char livesLeft				: 4; // to save the lives left, between 0 an SHIP_MAX_LIVES (set to 5)
	unsigned char canShoot 				: 4; // 1 if yes, 0 if not
	unsigned char invencibilityTicks; // to control invencibility mode after the ship is hit
} ship_t;

// Alien struct
typedef struct
{
	entity_t entity; // general entity struct
	unsigned char alienType; // to save the alien type
	unsigned char isMoving; // 1 if yes, 0 if not
} alien_t;

// Alien formation struct
typedef struct
{
	alien_t alien[ALIENS_ROWS][ALIENS_COLS]; // a matrix of aliens
	movingDirections_t direction; // to save the moving	direction of the matrix
	unsigned char canShoot; // 1 if yes, 0 if not
} alienFormation_t;

// Mothership struct
typedef struct
{
	entity_t entity; // general entity struct
	movingDirections_t direction; // to save the moving direction
} mothership_t;

// Barrier pixel struct
typedef struct
{
	entity_t entity; // only uses the general entity struct
} barrierPixel_t;

// Barrier struct
typedef struct 
{
	barrierPixel_t pixel[BARRIER_HEIGHT][BARRIER_WIDTH]; // a matrix of barrier pixels
} barrier_t;

// Bullet struct
typedef struct
{
	entity_t entity; // general entity struct
	movingDirections_t direction; // to save the moving direction
} bullet_t;

// PowerUp struct
typedef struct
{
	entity_t entity; // general entity struct
	powerUpTypes_t type; // to save the power up type
	int ticksLeft; // to save the time the powerup is active
} powerUp_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/



/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief function to create a ship type entity
 * @return an initialized ship in the standby position
 */
ship_t createShip();

/**
 * @brief function to create an alien formation type entity
 * @param rows amount of alien rows
 * @param columns amount of alien columns
 * @return an initialized alien formation in the standby position
 */
alienFormation_t createAlienFormation(int rows, int columns);

/**
 * @brief function to create a mothership type entity
 * @return an initialized mothership in the standby position
 */
mothership_t createMothership(); 

/**
 * @brief function to create a barrier type entity
 * @return an initialized barrier in the standby position
 */
barrier_t createBarrier();

/**
 * @brief function to create a bullet type entity
 * @param direction negative is upwards, positive is downwards
 * @return an initialized bullet in the standby position
 */
bullet_t createBullet(movingDirections_t direction);

/**
 * @brief function to create a powerUp type entity
 * @param type powerUp type
 * @return an initialized powerUp in the standby position
 */
powerUp_t createPowerUp(powerUpTypes_t type); 

/**
 * @brief function to move an entity in the X axis
 * @param entity is a pointer to the entity
 * @param moveRate int with the amount of pixels to move
 * @return 0 if the move was successful, -1 if there was an error
 */
int moveEntityX(entity_t *entity, int moveRate);

/**
 * @brief function to move an entity in the Y axis
 * @param entity is a pointer to the entity
 * @param moveRate int with the amount of pixels to move
 * @return 0 if the move was successful, -1 if there was an error
 */
int moveEntityY(entity_t *entity, int moveRate);

/**
 * @brief general function to set an entity as alive, and set it in a position
 * @param entity pointer to the entity
 * @param x position x
 * @param y position y
 * @return 0 if the entity was set successfully, -1 if there was an error
*/
int setEntity(entity_t * entity, int x, int y); 

/**
 * @brief function to shoot from an entity
 * @param bullet is a pointer to the bullet to shoot
 * @param shootingEntity is a pointer to the entity that is shooting
 * @return 0 if the shoot was executed, -1 if there was an error
 */
int shootFromEntity(bullet_t *bullet, entity_t *shootingEntity);

/**
 * @brief function to set the barriers position
 * @param barriers array of barriers to set
 * @return 0 if the barriers were set successfully, -1 if there was an error
 */
int setBarriers(barrier_t barriers[BARRIERS]);

/**
 * @brief function to set the aliens position
 * @param aliens pointer to the alien formation to set
 * @return 0 if the aliens were set successfully, -1 if there was an error
 */
int setAliens(alienFormation_t *aliens);

/**
 * @brief function to get the nearest column aligned with the ship's x position
 * @param aliens aliens structure to get the column
 * @param shipX ship's position in the X axis
 * @return the column closest to the ship, or -1 if there are no aliens near
 */
int getNearestColumnAlive(alienFormation_t aliens, short int shipX);

/**
 * @brief function to get the row alive nearest to the ship
 * @param aliens is the aliens structure to get the row from
 * @param column is the column of aliens to check
 * @return the row with an alive alien closest to the ship, or -1 if there are no aliens near
 */
int getNearestRowAlive(alienFormation_t aliens, int column);

/**
 * @brief function to get the first column with an alive alien from left to right
 * @param aliens alien formation to get the column from
 * @return first column with an alive alien starting from 0, or -1 if there are no aliens
 */
int getFirstColumnAlive(alienFormation_t aliens);

/**
 * @brief function to get the last column with an alive alien from left to right
 * @param aliens alien formation to get the row from
 * @return first row with an alive alien starting from 0, or -1 if there are no aliens
 */
int getLastColumnAlive(alienFormation_t aliens);

/**
 * @brief function to get the last row with an alive alien
 * @param aliens alien formation to get the row from
 * @return last row with an alive alien starting from 0, or -1 if there are no aliens
 */
int getLastRowAlive(alienFormation_t aliens);

/*******************************************************************************
 ******************************************************************************/

#endif // ENTITIES_H
