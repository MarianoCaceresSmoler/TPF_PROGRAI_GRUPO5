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

#define MOVING_UP -1
#define MOVING_LEFT -1
#define STILL 0
#define MOVING_DOWN 1
#define MOVING_RIGHT 1

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

static barrierPixel_t createBarrierPixel(int x, int y, int isAlive);
static alien_t createAlien(int x, int y, int type);


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

ship_t createShip(int x, int y)
{
	ship_t ship = {{x, y, SHIP_HEIGHT, SHIP_WIDTH, 1}, SHIP_LIVES, 1, 0, 0};
	return ship;
}

alienFormation_t createEnemies(int x, int y)
{
	alienFormation_t enemies;
	int i, j;
	for(i = 0; i < ALIENS_ROWS; i++)
	{
		for(j = 0; j < ALIENS_COLS; j++)
		{
			(enemies.alien[i][j]) = createAlien(x + j * 2 * ALIEN_WIDTH, y + i * 2 * ALIEN_HEIGHT, i);
		}
	}
	enemies.direction = MOVING_RIGHT;
	enemies.canShoot = 0;
	return enemies;
}

mothership_t createMothership(int x, int y)
{
	mothership_t mothership = {{x, y, SHIP_HEIGHT, SHIP_WIDTH, 0}, 0};
}

barrier_t createBarrier(int x, int y)
{
	barrier_t barrier;
	int i, j;
	for(i = 0; i < BARRIER_HEIGHT; i++)
	{
		for(j = 0; j < BARRIER_WIDTH; j++)
		{
			(barrier.pixel[i][j]) = createBarrierPixel(x + j * BARRIER_PIXEL_WIDTH, y + i * BARRIER_PIXEL_HEIGHT, (i != 1 && (i == j || i == 3-j)) ? 0: 1);
		}
	}
	return barrier;
}

bullet_t createBullet(int x, int y, int direction)
{
	bullet_t bullet = {{x, y, BULLET_HEIGHT, BULLET_WIDTH, 0}, direction};
	return bullet;
}

powerUp_t createPowerUp(int x, int y, int type)
{
	powerUp_t powerUp = {{x, y, POWERUP_HEIGHT, POWERUP_WIDTH, 0}, type};
	return powerUp;
}

void shipShoot(ship_t * ship, bullet_t * bullet)
{
	bullet->entity.x = ship->entity.x + SHIP_WIDTH / 2;
	bullet->entity.y = ship->entity.y - BULLET_HEIGHT / 2;
	bullet->direction = MOVING_UP;
	bullet->entity.isAlive = 1;
}

void alienShoot(alien_t * alien, bullet_t * bullet)
{
	bullet->entity.x = alien->entity.x + ALIEN_WIDTH / 2;
	bullet->entity.y = alien->entity.y + BULLET_HEIGHT / 2;
	bullet->direction = MOVING_DOWN;
	bullet->entity.isAlive = 1;
}

void moveShipLeft(ship_t * ship)
{
	ship->entity.x -= SHIP_MOVE_RATE;
	ship->direction = MOVING_LEFT;
}

void moveShipRight(ship_t * ship)
{
	ship->entity.x += SHIP_MOVE_RATE;
	ship->direction = MOVING_RIGHT;
}

void moveEnemiesLeft(alienFormation_t * enemies, int moveRate)
{
	int i, j;
	for (i = 0; i < ALIENS_ROWS; i++)
	{
		for (j = 0; j < ALIENS_COLS; j++)
		{
			enemies->alien[i][j].entity.x -= moveRate;
		}
	}
}

void moveEnemiesRight(alienFormation_t * enemies, int moveRate)
{
	int i, j;
	for (i = 0; i < ALIENS_ROWS; i++)
	{
		for (j = 0; j < ALIENS_COLS; j++)
		{
			enemies->alien[i][j].entity.x += moveRate;
		}
	}
}

void moveEnemiesDown(alienFormation_t * enemies, int moveRate)
{
	int i, j;
	for (i = 0; i < ALIENS_ROWS; i++)
	{
		for (j = 0; j < ALIENS_COLS; j++)
		{
			enemies->alien[i][j].entity.y -= moveRate;
		}
	}
}

void moveMothership(mothership_t * mothership, int moveRate)
{
	mothership->entity.x += mothership->direction * moveRate;
}

void moveBullet(bullet_t * bullet, int moveRate)
{
	bullet->entity.y += bullet->direction * moveRate;
}

int getAlienPoints(alien_t alien)
{
    switch(alien.alienType)
    {
    case 0:
        return ALIEN_TYPE_0_POINTS;
        break;
    case 1:
        return ALIEN_TYPE_1_POINTS;
        break;
    case 2:
        return ALIEN_TYPE_2_POINTS;
        break;
    default:
		return 0;
        break;
    }
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static alien_t createAlien(int x, int y, int type)
{
	alien_t alien = {{x, y, ALIEN_HEIGHT, ALIEN_WIDTH, 1}, type};
	return alien;
}

static barrierPixel_t createBarrierPixel(int x, int y, int isAlive)
{
	barrierPixel_t pixel = {{x, y, BARRIER_PIXEL_HEIGHT, BARRIER_PIXEL_WIDTH, isAlive}};
	return pixel;
}

/******************************************************************************/