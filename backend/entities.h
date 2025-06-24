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
typedef struct
{
	unsigned short int x;
	unsigned short int y;
	unsigned short int size;
	unsigned char isAlive;
} entity_t;

typedef struct 
{
	entity_t entity;
	unsigned char livesLeft		: 2;
	unsigned char canShoot 		: 1;
	unsigned char movingLeft 	: 1;
	unsigned char movingRight	: 1;
	unsigned char 	: 1;
	unsigned char	: 1;
	unsigned char	: 1;
} ship_t;

typedef struct
{
	entity_t entity;
	unsigned char alienType		: 2;
	unsigned char canShoot		: 1;
	unsigned char isMoving		: 1;
	unsigned char 	: 1;
	unsigned char 	: 1;
	unsigned char 	: 1;
	unsigned char	: 1;
} alien_t;

typedef struct
{
	alien_t alien[ALIENS_FILS][ALIENS_COLS];
	unsigned char direction;
	unsigned char tickRate;
} alienFormation_t;

typedef struct
{
	entity_t entity;
	unsigned char direction : 4;
	unsigned char isMoving : 4;
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
	char direction; // -1 goes upwards, 1 goes downwards
	
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
 * @brief TODO: completar descripcion
 * @param pahipram1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
 * @return Descripcion valor que devuelve
*/
// +ej: char lcd_goto (int fil, int col);+


/* "create" functions initialize entities in a position
 * x and y are the coordinates of the entity
 * 0, 0 is the top left corner
*/

ship_t createShip(int x, int y);
alienFormation_t createEnemies(int x, int y);
barrier_t createBarrier(int x, int y);
bullet_t createBullet(int x, int y);

/* "move" functions move an entity
 * ship/enemies are pointers to the entity to move
*/
void moveShipLeft(ship_t * ship);
void moveShipRight(ship_t * ship);
void moveEnemiesLeft(alienFormation_t * enemies);
void moveEnemiesRight(alienFormation_t * enemies);
void moveEnemiesDown(alienFormation_t * enemies);
void moveMothership(mothership_t * mothership);
void moveBullet(bullet_t * bullet);

/* "shoot" functions make a bullet go out in front of an entity
 * bullet is a pointer to the bullet_t entity to shoot
*/
void shipShoot(ship_t * ship, bullet_t * bullet);
void alienShoot(alien_t * alien, bullet_t * bullet);

/*
 *
*/


/*******************************************************************************
 ******************************************************************************/

#endif // ENTITIES_H
