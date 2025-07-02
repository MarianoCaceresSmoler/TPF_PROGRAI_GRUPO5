/***************************************************************************/ /**
   @game.c
   @Game structs and functions
   @Grupo_5
  ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

// +Incluir el header propio (ej: #include "template.h")+

#include "game.h"
#include "entities.h"
#include "physics.h"
#include "scores.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define ALIENS_TICKRATE (ALIEN_MAX_MOVE_TICKRATE - (game->tickCounter / 3000) - (ALIENS_NUMBER - game->aliensRemaining) / 5)

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
 * @param shipBullet bullet associated to the ship
 * @param input input in the last game tick
 */
static void updateShip(ship_t *ship, input_t input);

/**
 * @brief function to update a bullet type entity
 * @param bullet pointer to the bullet type entity
 */
static void updateBullet(bullet_t *bullet);

/**
 * @brief function to update an alienformation type entity
 * @param aliens pointer to the alienformation type entity
 */
static void updateAliens(alienFormation_t *aliens);

/**
 * @brief function to update a mothership type entity
 * @param bullet pointer to the mothership type entity
 */
static void updateMothership(mothership_t *mothership);

/**
 * @brief updates an entity explosion/live state
 * @param entity pointer to the entity
 */
static void updateEntityExplosion(entity_t *entity);

/**
 * @brief gets the first column with at least one alien alive
 * @param aliens pointer to the alien formation
 */
static int getFirstColumnAlive(alienFormation_t aliens);

/**
 * @brief gets the last column with at least one alien alive
 * @param aliens pointer to the alien formation
 */
static int getLastColumnAlive(alienFormation_t aliens);

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
	game->status = GAME_RUNNING;
	game->tickCounter = 0;
	game->aliensRemaining = ALIENS_NUMBER;

	// reset entities to initial position
	setEntity(&game->ship.entity, SHIP_INITIAL_X, SHIP_INITIAL_Y);

	setEntity(&game->mothership.entity, MOTHERSHIP_INITIAL_X, MOTHERSHIP_INITIAL_Y);

	int i, j, k;
	for (i = 0; i < ALIENS_ROWS; i++)
	{
		for (j = 0; j < ALIENS_COLS; j++)
		{
			setEntity(&game->aliens.alien[i][j].entity, ALIENS_INITIAL_X + j * ALIEN_SEPARATION, ALIENS_INITIAL_Y + i * ALIEN_SEPARATION);
		}
	}

	for (k = 0; k < BARRIERS; k++)
	{
		for (i = 0; i < BARRIER_HEIGHT; i++)
		{
			for (j = 0; j < BARRIER_WIDTH; j++)
			{
				setEntity(&game->barriers[k].pixel[i][j].entity, BARRIERS_INITIAL_X + k * BARRIERS_SEPARATION + j * BARRIER_PIXEL_WIDTH, BARRIERS_INITIAL_Y + i * BARRIER_PIXEL_HEIGHT);
				if (i != 1 && (i == j || i == 3 - j)) // determines barrier shape
					game->barriers[k].pixel[i][j].entity.isAlive = 0;
			}
		}
	}

	// reset bullets to standby position
	setEntity(&game->shipBullet.entity, STANDBY_POSITION, STANDBY_POSITION);
	setEntity(&game->alienBullet.entity, STANDBY_POSITION, STANDBY_POSITION);
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

void gameUpdate(game_t *game, input_t input)
{
	if (game->status == GAME_RUNNING)
	{
		int points = 0, row, column, alienTickRate; // ROW Y COLUMN SON PARA EL ALIEN QUE DISPARA
		game->tickCounter++;

		// updates entities
		updateShip(&game->ship, input);

		if (game->ship.canShoot && input == INPUT_SHOOT)
			shootFromEntity(&game->shipBullet, &game->ship.entity);
		else if (game->shipBullet.entity.isAlive)
			updateBullet(&game->shipBullet);

		if (game->aliens.canShoot) // && FUNCION QUE SE FIJA QUE ALIEN DISPARA
			shootFromEntity(&game->alienBullet, &game->aliens.alien[1][1].entity);
		else if (game->alienBullet.entity.isAlive)
			updateBullet(&game->alienBullet);

		static int alienTickCounter = 0;

		alienTickRate = (ALIENS_TICKRATE >= ALIEN_MIN_MOVE_TICKRATE ? ALIENS_TICKRATE : ALIEN_MIN_MOVE_TICKRATE);

		if (alienTickCounter == alienTickRate)
		{
			updateAliens(&game->aliens);
			alienTickCounter = 0;
		}

		alienTickCounter++;

		updateMothership(&game->mothership);

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

static void updateShip(ship_t *ship, input_t input)
{
	if (ship->entity.explosionTimer > 0)
	{
		ship->canShoot = 0;
		updateEntityExplosion(&ship->entity);
	}
	else
	{
		switch (input)
		{
		case INPUT_LEFT:
			if (ship->entity.x > SHIP_MOVE_RATE)
			{
				moveEntityX(&ship->entity, -SHIP_MOVE_RATE);
				ship->direction = MOVING_LEFT;
			}
			else
				ship->direction = STILL;
			break;

		case INPUT_RIGHT:
			if (ship->entity.x < SCREEN_SIZE - SHIP_MOVE_RATE)
			{
				moveEntityX(&ship->entity, SHIP_MOVE_RATE);
				ship->direction = MOVING_RIGHT;
			}
			else
				ship->direction = STILL;
			break;

		default:
			ship->direction = STILL;
			break;
		}
	}
}

static void updateBullet(bullet_t *bullet)
{
	if (bullet->entity.x >= 0 && bullet->entity.x <= SCREEN_SIZE && bullet->entity.y >= 0 && bullet->entity.y <= SCREEN_SIZE)
	{
		moveEntityY(&bullet->entity, bullet->direction * BULLET_MOVE_RATE);
	}
	else
	{
		bullet->entity.isAlive = 0;
	}
}

static void updateAliens(alienFormation_t *aliens)
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
	static int rowToMove = 5;
	if (rowToMove == 0)
		rowToMove = 4;
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
		if (!rowToMove)
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
	else if (mothership->entity.x >= -MOTHERSHIP_WIDTH && mothership->entity.x <= SCREEN_SIZE + MOTHERSHIP_WIDTH)
		moveEntityX(&mothership->entity, mothership->direction * MOTHERSHIP_MOVE_RATE);
}

static void updateEntityExplosion(entity_t *entity)
{
	entity->explosionTimer--;
	if (!entity->explosionTimer)
	{
		entity->isAlive = 0; // when the timer hits 0, the alien needs to be removed
	}
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