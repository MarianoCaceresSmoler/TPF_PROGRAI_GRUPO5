/***************************************************************************/ /**
   @game.c
   @Game structs and functions
   @Grupo_5
  ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "game.h"
#include "entities.h"
#include "physics.h"
#include "scores.h"
#include <stdlib.h>
#include <time.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// defines tickrate of the aliens depending on the game ticks and the number of aliens remaining

#define TICKS_TO_MOVE_ALIENS(gameTicks, aliensRemaining) (ALIEN_MAX_MOVE_TICKRATE - ((gameTicks) / 3000) - ((ALIENS_NUMBER - (aliensRemaining)) / 5))

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
 * @brief function to update a ship type entity
 * @param ship pointer to the ship type entity
 * @param moveLeft true if the left key is pressed, false otherwise
 * @param moveRight true if the right key is pressed, false otherwise
 */
static void updateShip(ship_t *ship, bool moveLeft, bool moveRight);

/**
 * @brief function to update a bullet type entity
 * @param bullet pointer to the bullet type entity
 */
static void updateBullet(bullet_t *bullet);

/**
 * @brief function to update an alienformation type entity
 * @param aliens pointer to the alienformation type entity
 */
static void updateAliens(alienFormation_t *aliens, int tickCounter, int aliensRemaining);

/**
 * @brief function to update a mothership type entity
 * @param bullet pointer to the mothership type entity
 */
static void updateMothership(mothership_t *mothership);

/**
 * @brief function to update an entity explosion/live state
 * @param entity pointer to the entity
 */
static void updateEntityExplosion(entity_t *entity);

static int getNearestColumnAlive(alienFormation_t aliens, short int shipX);

static int getNearestRowAlive(alienFormation_t aliens, int column);

/**
 * @brief function to get the first column with at least one alien alive
 * @param aliens pointer to the alien formation
 */
static int getFirstColumnAlive(alienFormation_t aliens);

/**
 * @brief function to get the last column with at least one alien alive
 * @param aliens pointer to the alien formation
 */
static int getLastColumnAlive(alienFormation_t aliens);

/**
 * @brief function to set the shape of a barrier
 * @param barrier pointer to the barrier type entity
 */
static void setBarrierShape(barrier_t *barrier);

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

void gameInit(game_t *game)
{
	// initialize entities
	game->ship = createShip();
	game->aliens = createAlienFormation(ALIENS_ROWS, ALIENS_COLS);
	game->mothership = createMothership();

	int i;
	for (i = 0; i < BARRIERS; i++)
	{
		game->barriers[i] = createBarrier();
	}

	game->shipBullet = createBullet(MOVING_UP);
	game->alienBullet = createBullet(MOVING_DOWN);

	for (i = 0; i < POWERUP_TYPES; i++)
	{
		game->powerUp[i] = createPowerUp(i);
	}

	// initialize game parameters
	game->status = GAME_MENU;
	game->score = 0;
	game->currentLevel = 1;
	game->tickCounter = 0;
	game->aliensRemaining = ALIENS_NUMBER;
}

void levelInit(game_t *game)
{
	// reset bullets to standby position
	setEntity(&game->shipBullet.entity, STANDBY_POSITION, STANDBY_POSITION);
	setEntity(&game->alienBullet.entity, STANDBY_POSITION, STANDBY_POSITION);

	// set entities to initial position
	setEntity(&game->ship.entity, SHIP_INITIAL_X, SHIP_INITIAL_Y);
	game->ship.canShoot = 1;

	game->mothership.entity.isAlive = 0;

	// set aliens to initial position
	int i, j, k;
	for (i = 0; i < ALIENS_ROWS; i++)
	{
		for (j = 0; j < ALIENS_COLS; j++)
		{
			setEntity(&game->aliens.alien[i][j].entity, ALIENS_INITIAL_X + j * ALIEN_X_SEPARATION, ALIENS_INITIAL_Y + i * ALIEN_Y_SEPARATION);
		}
	}

	game->status = GAME_RUNNING;
	game->tickCounter = 0;
	game->aliensRemaining = ALIENS_NUMBER;

	if(game->currentLevel > 1)
		return;
	// set barriers to initial position
	for (k = 0; k < BARRIERS; k++)
	{
		for (i = 0; i < BARRIER_HEIGHT; i++)
		{
			for (j = 0; j < BARRIER_WIDTH; j++)
			{
				setEntity(&game->barriers[k].pixel[i][j].entity, BARRIERS_INITIAL_X + k * BARRIERS_SEPARATION + j * BARRIER_PIXEL_WIDTH, BARRIERS_INITIAL_Y + i * BARRIER_PIXEL_HEIGHT);
				setBarrierShape(&game->barriers[k]);
			}
		}
	}
}

void gamePause(game_t *game)
{
	game->status = GAME_PAUSED;
}

void gameResume(game_t *game)
{
	game->status = GAME_RUNNING;
}

void gameEnd(game_t *game)
{
	game->status = GAME_END;
}

void gameReset(game_t *game)
{
	levelInit(game);
	game->ship.livesLeft = SHIP_LIVES;
	game->ship.entity.explosionTimer = 0;
	game->aliens.canShoot = 0;
	game->ship.canShoot = 1;
}

void gameUpdate(game_t *game, inputStatus_t input)
{
	static bool firstTime = true;
	if (firstTime)
	{
		srand(time(NULL)); // seed the random number generator
		firstTime = false;
	}

	if (game->status == GAME_RUNNING)
	{
		int points = 0;
		game->tickCounter++;

		// updates game status
		if(game->aliensRemaining == 0)
		{
			game->currentLevel++;
			levelInit(game);
		}
		else if(game->ship.entity.isAlive == false)
		{
			if(game->ship.livesLeft == 0)
				gameEnd(game);
			else
				setEntity(&game->ship.entity, SHIP_INITIAL_X, SHIP_INITIAL_Y);
		}

		// updates entities
		updateShip(&game->ship, input.leftKeyPressed, input.rightKeyPressed);

		if (game->shipBullet.entity.isAlive == false)
			game->ship.canShoot = true;

		if (game->shipBullet.entity.isAlive)
			updateBullet(&game->shipBullet);
		else if (game->ship.canShoot && input.shootKeyPressed)
		{
			shootFromEntity(&game->shipBullet, &game->ship.entity);
			game->ship.canShoot = false;
		}

		if (game->alienBullet.entity.isAlive == false)
			game->aliens.canShoot = true;

		if (game->alienBullet.entity.isAlive)
			updateBullet(&game->alienBullet);
		else if (game->aliens.canShoot)
		{
			int alienColumnToShoot = getNearestColumnAlive(game->aliens, game->ship.entity.x);
			int alienRowToShoot = getNearestRowAlive(game->aliens, alienColumnToShoot);

			if (alienRowToShoot >= 0 && alienColumnToShoot >= 0 && game->aliens.alien[alienRowToShoot][alienColumnToShoot].entity.isAlive)
				shootFromEntity(&game->alienBullet, &game->aliens.alien[alienRowToShoot][alienColumnToShoot].entity);
			
			game->aliens.canShoot = false;
		}

		updateAliens(&game->aliens, game->tickCounter, game->aliensRemaining);

		if(game->mothership.entity.isAlive)
			updateMothership(&game->mothership);
		else if(game->tickCounter % 150 == 0 && rand() % 100 < MOTHERSHIP_CHANCE)
		{
			game->mothership.direction = (rand() % 2 == 0)? MOVING_RIGHT: MOVING_LEFT;
			setEntity(&game->mothership.entity, (game->mothership.direction == MOVING_LEFT? MOTHERSHIP_RIGHT_INITIAL_X: MOTHERSHIP_LEFT_INITIAL_X), MOTHERSHIP_INITIAL_Y);
		}

		// updates score if an alien is killed
		if ((points = handleCollisions(game)))
			incrementScore(game, points);
	}
}

/*******************************************************************************
 *******************************************************************************
						LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void updateShip(ship_t *ship, bool moveLeft, bool moveRight)
{
	if (ship->entity.explosionTimer > 0)
	{
		ship->canShoot = 0;
		updateEntityExplosion(&ship->entity);
	}
	else if (ship->entity.isAlive)
	{
		if (moveLeft && ship->entity.x > SHIP_MOVE_RATE)
		{
			moveEntityX(&ship->entity, -SHIP_MOVE_RATE);
			ship->direction = MOVING_LEFT;
		}
		else
			ship->direction = STILL;

		if (moveRight && ship->entity.x < SCREEN_SIZE - SHIP_WIDTH - SHIP_MOVE_RATE)
		{
			moveEntityX(&ship->entity, SHIP_MOVE_RATE);
			ship->direction = MOVING_RIGHT;
		}
		else
			ship->direction = STILL;

		if (moveLeft && moveRight)
			ship->direction = STILL;
	}
}

static void updateBullet(bullet_t *bullet)
{
	if (bullet->entity.isAlive == false)
		return;

	if (bullet->entity.x < 0 || bullet->entity.x > SCREEN_SIZE || bullet->entity.y < 0 || bullet->entity.y > SCREEN_SIZE)
	{
		bullet->entity.isAlive = 0;
		return;
	}	
	
	moveEntityY(&bullet->entity, bullet->direction * BULLET_MOVE_RATE);
	
}

static void updateAliens(alienFormation_t *aliens, int gameTicks, int aliensRemaining)
{
	int firstColumn = -1, lastColumn = ALIENS_COLS - 1, i, j;

	for (i = 0; i < ALIENS_ROWS; i++)
	{
		for (j = 0; j < ALIENS_COLS; j++)
		{
			if (aliens->alien[i][j].entity.explosionTimer > 0)
				updateEntityExplosion(&aliens->alien[i][j].entity);
		}
	}

	firstColumn = getFirstColumnAlive(*aliens);
	lastColumn = getLastColumnAlive(*aliens);

	// rowToMove is used to move the alien rows separately
	static int rowToMove = ALIENS_ROWS;

	if(gameTicks % 2 != 0)
		return;

	if (rowToMove == 0)
		rowToMove = ALIENS_ROWS - 1;
	else
		rowToMove--;

	switch (aliens->direction)
	{
	case MOVING_RIGHT:
		if (aliens->alien[0][lastColumn].entity.x < SCREEN_SIZE - ALIEN_WIDTH - ALIEN_MOVE_RATE)
		{
			for (i = 0; i < ALIENS_COLS; i++)
			{
				moveEntityX(&aliens->alien[rowToMove][i].entity, ALIEN_MOVE_RATE);
			}
		}
		else
		{
			aliens->direction = MOVING_DOWN;
			rowToMove++;
		}
		break;

	case MOVING_LEFT:
		if (aliens->alien[0][firstColumn].entity.x > ALIEN_MOVE_RATE)
		{
			for (i = 0; i < ALIENS_COLS; i++)
			{
				moveEntityX(&aliens->alien[rowToMove][i].entity, -ALIEN_MOVE_RATE);
			}
		}
		else
		{
			aliens->direction = MOVING_DOWN;
			rowToMove++;
		}
		break;

	case MOVING_DOWN:
		for (i = 0; i < ALIENS_COLS; i++)
		{
			moveEntityY(&aliens->alien[rowToMove][i].entity, ALIEN_MOVE_RATE);
		}

		if (rowToMove == 0)
		{
			if (aliens->alien[0][firstColumn].entity.x > 2 * ALIEN_MOVE_RATE)
				aliens->direction = MOVING_LEFT;
			else
				aliens->direction = MOVING_RIGHT;
		}
		break;

	default:
		break;
	}
}

static void updateMothership(mothership_t *mothership)
{
	if (mothership->entity.explosionTimer > 0)
		updateEntityExplosion(&mothership->entity);
	else if (mothership->entity.isAlive && mothership->entity.x >= MOTHERSHIP_LEFT_INITIAL_X && mothership->entity.x <= MOTHERSHIP_RIGHT_INITIAL_X)
		moveEntityX(&mothership->entity, mothership->direction * MOTHERSHIP_MOVE_RATE);
	else 
		mothership->entity.isAlive = false;
}

static void updateEntityExplosion(entity_t *entity)
{
	entity->explosionTimer--;
	if (!entity->explosionTimer)
	{
		entity->isAlive = 0; // when the timer hits 0, the entity is no longer alive
	}
}

static int getNearestColumnAlive(alienFormation_t aliens, short int shipX)
{
	int i, nearestColumn = -1;

	for(i = 0; i < ALIENS_COLS && nearestColumn == -1; i++)
	{
		if(aliens.alien[0][i].entity.x + ALIEN_WIDTH  >= shipX + SHIP_WIDTH / 2 && 
			aliens.alien[0][i].entity.x <= shipX + SHIP_WIDTH / 2)
		{
			nearestColumn = i;
		}
	}
	
	return nearestColumn;
}

static int getNearestRowAlive(alienFormation_t aliens, int column)
{
	int row, nearestRow = -1;
	for (row = ALIENS_ROWS - 1; row > 0 && nearestRow == -1; row--)
	{
		if (aliens.alien[row][column].entity.isAlive)
		{
			nearestRow = row;
		}
	}
	return nearestRow;
}

static int getFirstColumnAlive(alienFormation_t aliens)
{
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

static int getLastColumnAlive(alienFormation_t aliens)
{
	int i, j, lastColumn = -1;
	for (j = ALIENS_COLS - 1; j > 0 && lastColumn == -1; j--)
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

static void setBarrierShape(barrier_t *barrier)
{
	int i, j;
	for (i = 0; i < BARRIER_HEIGHT; i++)
	{
		for (j = 0; j < BARRIER_WIDTH; j++)
		{
			if ((i == 0 && (j == 0 || j == 5)) || (i == 3 && (j >= 1 && j <= 4))) // determines barrier shape
				barrier->pixel[i][j].entity.isAlive = 0;
			else
				barrier->pixel[i][j].entity.isAlive = 1;
		}
	}
}