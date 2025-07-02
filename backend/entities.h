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

// CAMBIOS QUE FALTAN (02/07/2025)
//
// DEFINIR SI HACE FALTA USAR X e Y COMO PARAMETRO O SE PUEDE INICIALIZAR EN
// UNA UBICACION ESTANDAR QUE NO SE USE PARA NADA (POSICION NULA)
//
// CAMBIAR TODAS LAS FUNCIONES MOVE POR UNA FUNCION MOVEENTITY GENERAL
// (PUEDE SER UNA SOLA O UNA VERTICAL Y UNA HORIZONTAL)
//
// CAMBIAR LAS FUNCIONES SHOOT POR UNA GENERAL QUE SIRVA PARA TODAS LAS ENTIDADES
//
// FALTA UN ENUM CON LOS TIPOS DE POWERUP
//
// FALTA PONER MOVERATE EN LAS FUNCIONES MOVESHIP


/**
 * @brief function to create a ship type entity
 * @param x int position in x axis
 * @param y int position in y axis
 */
ship_t createShip(int x, int y);

/**
 * @brief function to create an alienformation type entity
 * @param x int position in x axis
 * @param y int position in y axis
 */
alienFormation_t createEnemies(int x, int y);

/**
 * @brief function to create a mothership type entity
 * @param x int position in x axis
 * @param y int position in y axis
 */
mothership_t createMothership(int x, int y); 

/**
 * @brief function to create a barrier type entity
 * @param x int position in x axis
 * @param y int position in y axis
 */
barrier_t createBarrier(int x, int y);

/**
 * @brief function to create a bullet type entity
 * @param x int position in x axis
 * @param y int position in y axis
 * @param direction -1 is upwards, 1 is downwards
 */
bullet_t createBullet(int x, int y, movingDirections_t direction); // 1 is down, -1 is up

/**
 * @brief function to create a powerUp type entity
 * @param x int position in x axis
 * @param y int position in y axis
 * @param type powerUp type 
 */
powerUp_t createPowerUp(int x, int y, int type); // 0 is freeze, 1 is double shot, etc 


/**
 * @brief function to move a ship to the left
 * @param ship is a pointer to the ship type entity
 */
void moveShipLeft(ship_t * ship);

/**
 * @brief function to move a ship to the right
 * @param ship is a pointer to the ship type entity
 */
void moveShipRight(ship_t * ship);

/**
 * @brief function to move aliens to the left
 * @param enemies is a pointer to the alienformation type entity
 * @param moveRate is the amount of pixels to move
 */
void moveEnemiesLeft(alienFormation_t * enemies, int moveRate);

/**
 * @brief function to move aliens to the right
 * @param enemies is a pointer to the alienformation type entity
 * @param moveRate is the amount of pixels to move
 */
void moveEnemiesRight(alienFormation_t * enemies, int moveRate);

/**
 * @brief function to move aliens downwards
 * @param enemies is a pointer to the alienformation type entity
 * @param moveRate is the amount of pixels to move
 */
void moveEnemiesDown(alienFormation_t * enemies, int moveRate);

/**
 * @brief function to move mothership to the left
 * @param enemies is a pointer to the mothership type entity
 * @param moveRate is the amount of pixels to move
 */
void moveMothershipLeft(mothership_t *mothership, int moveRate);
void moveMothershipRight(mothership_t *mothership, int moveRate); // FALTA DEFINIRLA
void moveBullet(bullet_t * bullet, int moveRate);


/* "shoot" functions make a bullet go out in front of an entity
 * bullet is a pointer to the bullet_t entity to shoot
*/

/**
 * @brief function to make a bullet go out in front of an entity
 * @param ship is a pointer to the ship type entity
 * @param bullet is a pointer to the bullet type entity
 */
void shipShoot(ship_t * ship, bullet_t * bullet);

/**
 * @brief function to make a bullet go out in front of an entity
 * @param alien is a pointer to the alien type entity
 * @param bullet is a pointer to the bullet type entity
 */
void alienShoot(alien_t * alien, bullet_t * bullet);

/**
 * @brief general function to set an entity as alive, and set it in a position
 * @param entity pointer to the entity
 * @param x position x
 * @param y position y
*/
void setEntity(entity_t * entity, int x, int y); 

/*******************************************************************************
 ******************************************************************************/

#endif // ENTITIES_H
