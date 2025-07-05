/***************************************************************************//**
  @file     +Nombre del archivo (ej: template.c)+
  @brief    +Descripcion del archivo+
  @author   +Nombre del autor (ej: Salvador Allende)+
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "entities.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/



/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/**
 * @brief function to create a barrierPixel type entity
 * @return returns an initialized barrierPixel in the standby position
 */
static barrierPixel_t createBarrierPixel();

/**
 * @brief function to create an alien type entity
 * @return returns an initialized alien in the standby position
 */
static alien_t createAlien(int type);


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static int temperaturas_actuales[4];+


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

ship_t createShip()
{
	ship_t ship = {{STANDBY_POSITION, STANDBY_POSITION, SHIP_HEIGHT, SHIP_WIDTH, 1}, STILL, SHIP_LIVES, 1};
	return ship;
}

alienFormation_t createAlienFormation(int rows, int columns)
{
	alienFormation_t enemies;
	int i, j;
	for(i = 0; i < rows; i++)
	{
		for(j = 0; j < columns; j++)
		{
			(enemies.alien[i][j]) = createAlien(i);
		}
	}
	enemies.direction = MOVING_RIGHT;
	enemies.canShoot = 0;
	return enemies;
}

mothership_t createMothership()
{
	mothership_t mothership = {{STANDBY_POSITION, STANDBY_POSITION, MOTHERSHIP_HEIGHT, MOTHERSHIP_WIDTH, 0}, STILL};
	return mothership;
}

barrier_t createBarrier()
{
	barrier_t barrier;
	int i, j;
	for(i = 0; i < BARRIER_HEIGHT; i++)
	{
		for(j = 0; j < BARRIER_WIDTH; j++)
		{
			barrier.pixel[i][j] = createBarrierPixel();
		}
	}
	return barrier;
}

bullet_t createBullet(movingDirections_t direction)
{
	bullet_t bullet = {{STANDBY_POSITION, STANDBY_POSITION, BULLET_HEIGHT, BULLET_WIDTH, 0}, (direction > 0? 1: -1)};
	return bullet;
}

powerUp_t createPowerUp(powerUpTypes_t type)
{
	powerUp_t powerUp = {{STANDBY_POSITION, STANDBY_POSITION, POWERUP_HEIGHT, POWERUP_WIDTH, 0}, type};
	return powerUp;
}

void moveEntityX(entity_t *entity, int moveRate)
{
	entity->x += moveRate;
}

void moveEntityY(entity_t *entity, int moveRate)
{
	entity->y += moveRate;
}

void setEntity(entity_t * entity, int x, int y)
{
	entity->x = x;
	entity->y = y;
	entity->isAlive = 1;
	entity->explosionTimer = 0;
}

void shootFromEntity(bullet_t *bullet, entity_t *shootingEntity)
{
	bullet->entity.x = shootingEntity->x + shootingEntity->width / 2;
	bullet->entity.y = shootingEntity->y + (bullet->direction > 0? shootingEntity->height + bullet->entity.height/2: -bullet->entity.height/2);
	bullet->entity.isAlive = 1;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static alien_t createAlien(int type)
{
	alien_t alien = {{STANDBY_POSITION, STANDBY_POSITION, ALIEN_HEIGHT, ALIEN_WIDTH, 1}, ALIEN_MOVE_RATE, type};
	return alien;
}

static barrierPixel_t createBarrierPixel()
{
	barrierPixel_t pixel = {{STANDBY_POSITION, STANDBY_POSITION, BARRIER_PIXEL_HEIGHT, BARRIER_PIXEL_WIDTH, 1}};
	return pixel;
}

/******************************************************************************/