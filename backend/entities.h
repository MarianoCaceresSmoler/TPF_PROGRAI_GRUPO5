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

typedef enum{
	MOVING_LEFT = -2, 
	MOVING_UP, 
	STILL, 
	MOVING_DOWN, 
	MOVING_RIGHT} 
movingDirections_t;

typedef struct
{
	unsigned short int x;
	unsigned short int y;
	unsigned short int height;
	unsigned short int width;
	unsigned char isAlive;
	unsigned short int explosionTimer;  // > 0 if explosion is active
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

/* "create" functions initialize entities in a position
 * x and y are the coordinates of the entity
 * 0, 0 is the top left corner
*/
ship_t createShip(int x, int y);
alienFormation_t createEnemies(int x, int y);
mothership_t createMothership(int x, int y); 
barrier_t createBarrier(int x, int y);
bullet_t createBullet(int x, int y, int direction); // 1 is down, -1 is up
powerUp_t createPowerUp(int x, int y, int type); // 0 is freeze, 1 is double shot, etc 

/* "move" functions move an entity
 * ship/enemies are pointers to the entity to move
 * moveRate is the amount of units moved
*/
void moveShipLeft(ship_t * ship);
void moveShipRight(ship_t * ship);
void moveEnemiesLeft(alienFormation_t * enemies, int moveRate);
void moveEnemiesRight(alienFormation_t * enemies, int moveRate);
void moveEnemiesDown(alienFormation_t * enemies, int moveRate);
void moveMothership(mothership_t * mothership, int moveRate);
void moveBullet(bullet_t * bullet, int moveRate);
// CAPAZ QUEDAN PRIVADAS Y SOLO SE USAN LAS DE UPDATE


/* "shoot" functions make a bullet go out in front of an entity
 * bullet is a pointer to the bullet_t entity to shoot
*/
void shipShoot(ship_t * ship, bullet_t * bullet);
void alienShoot(alien_t * alien, bullet_t * bullet);

/* "getAlienPoints" returns the amount of points given by the type of alien killed
 * alien is a pointer to the structure of the enemy killed
*/
int getAlienPoints(alien_t alien);

/**
 * @brief general function to set an entity as alive, and set it in a position
 * @param entity pointer to the entity
 * @param x position x
 * @param y position y
*/
void setEntity(entity_t * entity, int x, int y); 


// POSIBLES CAMBIOS
/*
void update_entities(game_t * game) *******VA EN GAME.C (no esta definido game_t)
{
	void update_ship(ship_t *ship, input_t input);
	void update_aliens(alienFormation_t *aliens, int tick);
	void update_bullets(bullet_t bullets[MAX_BULLETS]);
	void update_mothership(mothership_t *mothership, int tick);
	void update_barriers(barriers_t *barrier); ****FALTA VER SI VA ESTA
}

void init_entities(game_t * game) *******VA EN GAME.C (no esta definido game_t)
{
	void init_aliens(alienFormation_t *aliens);
	void init_barriers(barrier_t barriers[BARRIERS]);
	void init_ship(ship_t *ship);
}
*/

/*******************************************************************************
 ******************************************************************************/

#endif // ENTITIES_H
