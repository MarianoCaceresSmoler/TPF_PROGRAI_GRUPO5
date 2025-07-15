/***************************************************************************//**
  @file     +Nombre del archivo (ej: template.c)+
  @brief    +Descripcion del archivo+
  @author   +Nombre del autor (ej: Salvador Allende)+
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdio.h>

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

/**
 * @brief function to set the shape of a barrier
 * @param barrier pointer to the barrier type entity
 * @return 0 if the barrier shape was set successfully, -1 if there was an error

 */
static int setBarrierShape(barrier_t *barrier);


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
	ship_t ship = {{STANDBY_POSITION, STANDBY_POSITION, SHIP_HEIGHT, SHIP_WIDTH, 1}, STILL, SHIP_INITIAL_LIVES, 1};
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
			(enemies.alien[i][j]) = createAlien(i); // creates each individual alien of the formation
		}
	}

	// Initially the aliens move to the right, and cannot shoot
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
			barrier.pixel[i][j] = createBarrierPixel(); // creates each individual pixel of the barrier
		}
	}
	return barrier;
}

bullet_t createBullet(movingDirections_t direction)
{
	bullet_t bullet = {{STANDBY_POSITION, STANDBY_POSITION, BULLET_HEIGHT, BULLET_WIDTH, 0}, direction};
	return bullet;
}

powerUp_t createPowerUp(powerUpTypes_t type)
{
	powerUp_t powerUp = {{STANDBY_POSITION, STANDBY_POSITION, POWERUP_HEIGHT, POWERUP_WIDTH, 0}, type, 0};
	return powerUp;
}

int moveEntityX(entity_t *entity, int moveRate)
{
	if(!entity)
	{
		printf("Error: entity is NULL\n");
		return -1; // returns an error if the entity is NULL
	}

	entity->x += moveRate;
	return 0;
}

int moveEntityY(entity_t *entity, int moveRate)
{
	if(!entity)
	{
		printf("Error: entity is NULL\n");
		return -1; // returns an error if the entity is NULL
	}

	entity->y += moveRate;
	return 0;
}

int setEntity(entity_t * entity, int x, int y)
{
	if(!entity)
	{
		printf("Error: entity is NULL\n");
		return -1; // returns an error if the entity is NULL
	}

	// Sets the entity position, and sets it alive
	entity->x = x;
	entity->y = y;
	entity->isAlive = 1;
	entity->explosionTimer = 0;
	
	return 0;
}

int shootFromEntity(bullet_t *bullet, entity_t *shootingEntity)
{
	if(!bullet || !shootingEntity)
	{
		printf("Error: entity is NULL\n");
		return -1; // returns an error if one or both of the entities are NULL
	}
	
	int yOffset = (bullet->direction > 0? shootingEntity->height + bullet->entity.height / 2: - bullet->entity.height / 2);

	setEntity(&bullet->entity, 
			shootingEntity->x + shootingEntity->width / 2,
			shootingEntity->y + yOffset);

	return 0;
}

int setBarriers(barrier_t barriers[BARRIERS])
{
	int i, j, k;
	// set barriers to initial position
	for (k = 0; k < BARRIERS; k++)
	{
		for (i = 0; i < BARRIER_HEIGHT; i++)
		{
			for (j = 0; j < BARRIER_WIDTH; j++)
			{
				// Sets the pixel entity on screen and gives shape to the barrier
				if(setEntity(&barriers[k].pixel[i][j].entity, BARRIERS_INITIAL_X + k * BARRIERS_SEPARATION + j * BARRIER_PIXEL_WIDTH, BARRIERS_INITIAL_Y + i * BARRIER_PIXEL_HEIGHT))
				{
					printf("Error setting barrier pixel entity\n");
					return -1; // returns an error if there was an error setting a pixel entity
				}
			}
		}

		if(setBarrierShape(&barriers[k])) // gives the shape to / constructs the barrier
		{
			printf("Error setting barrier shape\n");
			return -1; // returns an error if there was an error setting the barrier shape
		}
	}

	return 0;
}

int setAliens(alienFormation_t *aliens)
{
	if(!aliens)
	{
		printf("Error: alien formation is NULL\n");
		return -1; // returns an error if the alien formation is NULL
	}

	int i, j;
	for (i = 0; i < ALIENS_ROWS; i++)
	{
		for (j = 0; j < ALIENS_COLS; j++)
		{
			//Ssets each alien individually
			if(setEntity(&aliens->alien[i][j].entity, ALIENS_INITIAL_X + j * ALIEN_X_SEPARATION, ALIENS_INITIAL_Y + i * ALIEN_Y_SEPARATION))
			{
				printf("Error setting alien entity\n");
				return -1; // returns an error if there was an error setting an alien entity
			} 
		}
	}

	return 0;
}

int getNearestColumnAlive(alienFormation_t aliens, short int shipX)
{
	int i, nearestColumn = -1;

	// Checks from left to right and returns the first matching column
	for (i = 0; i < ALIENS_COLS && nearestColumn == -1; i++)
	{
		if (aliens.alien[0][i].entity.x + ALIEN_WIDTH + ALIEN_X_SEPARATION / 2 >= shipX + SHIP_WIDTH / 2 &&
			aliens.alien[0][i].entity.x - ALIEN_X_SEPARATION / 2 <= shipX + SHIP_WIDTH / 2)
		{
			nearestColumn = i;
		}
	}

	return nearestColumn;
}

int getNearestRowAlive(alienFormation_t aliens, int column)
{
	// Returns the index of the lowest (last) alive alien row in the given column
	// Starts checking from the bottom up

	int row, nearestRow = -1;
	for (row = ALIENS_ROWS - 1; row >= 0 && nearestRow == -1; row--)
	{
		if (aliens.alien[row][column].entity.isAlive)
		{
			nearestRow = row;
		}
	}
	return nearestRow;
}

int getFirstColumnAlive(alienFormation_t aliens)
{
	// Returns the index of the first column (from left to right) that contains at least one alive alien.

	int i, j, firstColumn = -1;
	for (j = 0; j < ALIENS_COLS && firstColumn == -1; j++)
	{
		for (i = 0; i < ALIENS_ROWS && firstColumn == -1; i++)
		{
			if (aliens.alien[i][j].entity.isAlive)
			{
				firstColumn = j;
			}
		}
	}

	return firstColumn;
}

int getLastColumnAlive(alienFormation_t aliens)
{
	// Returns the index of the last column (from right to left) that contains at least one alien alive

	int i, j, lastColumn = -1;
	for (j = ALIENS_COLS - 1; j >= 0 && lastColumn == -1; j--)
	{
		for (i = 0; i < ALIENS_ROWS && lastColumn == -1; i++)
		{
			if (aliens.alien[i][j].entity.isAlive)
			{
				lastColumn = j;
			}
		}
	}

	return lastColumn;
}

int getLastRowAlive(alienFormation_t aliens)
{
	// Returns the index of the last row that contains at least one alien alive
	int i, j, lastRow = -1;

	for (i = ALIENS_ROWS - 1; i >= 0 && lastRow == -1; i--)
	{
		for (j = ALIENS_COLS; j >= 0 && lastRow == -1; j--)
		{
			if (aliens.alien[i][j].entity.isAlive)
			{
				lastRow = i;
			}
		}		
	}

	return lastRow;
	
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static alien_t createAlien(int type)
{
	alien_t alien = {{STANDBY_POSITION, STANDBY_POSITION, ALIEN_HEIGHT, ALIEN_WIDTH, 1}, type, 0};
	return alien;
}

static barrierPixel_t createBarrierPixel()
{
	barrierPixel_t pixel = {{STANDBY_POSITION, STANDBY_POSITION, BARRIER_PIXEL_HEIGHT, BARRIER_PIXEL_WIDTH, 1}};
	return pixel;
}

static int setBarrierShape(barrier_t *barrier)
{
	if(!barrier)
	{
		printf("Error: barrier is NULL\n");
		return -1; // returns an error if the barrier is NULL
	}

	// Depending on the platform, sets some pixels as dead to give the shape to the barrier
	#ifdef PLATFORM_RPI
	barrier->pixel[0][0].entity.isAlive = 0;
	barrier->pixel[0][3].entity.isAlive = 0;
	barrier->pixel[2][1].entity.isAlive = 0;
	barrier->pixel[2][2].entity.isAlive = 0;
	#else
	barrier->pixel[0][0].entity.isAlive = 0;
	barrier->pixel[0][5].entity.isAlive = 0;
	barrier->pixel[3][1].entity.isAlive = 0;
	barrier->pixel[3][2].entity.isAlive = 0;
	barrier->pixel[3][3].entity.isAlive = 0;
	barrier->pixel[3][4].entity.isAlive = 0;
	#endif

	return 0;
}


/******************************************************************************/