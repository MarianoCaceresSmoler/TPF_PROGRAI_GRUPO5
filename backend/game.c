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

/* "update" functions update a type of entity
 * ship/enemies are pointers to the entity to move
 * moveRate is the amount of units moved
 */
static void gameUpdate(game_t *game, input_t input);
static void updateShip(ship_t *ship, bullet_t *shipBullet, input_t input);
static void updateBullet(bullet_t *bullet);
static void updateAliens(alienFormation_t *aliens, bullet_t *alienBullet);
static void updateMothership(mothership_t *mothership);

/**
 * @brief function to check the game status
 * @param game pointer to the game information
 * @return 0 if the game is not over, 1 if the game is over
 */
static int checkGameStatus(game_t *game);

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
	game->ship = createShip(SHIP_INITIAL_X, SHIP_INITIAL_Y);
	game->aliens = createEnemies(ALIENS_INITIAL_X, ALIENS_INITIAL_Y);
	game->mothership = createMothership(MOTHERSHIP_INITIAL_X, MOTHERSHIP_INITIAL_Y);

	int i;
	for (i = 0; i < BARRIERS; i++)
	{
		game->barriers[i] = createBarrier(200 * i, 600); // DEFINIR POSICION DE BARRERAS
	}

	game->shipBullet = createBullet(0, 0, MOVING_UP);
	game->alienBullet = createBullet(0, 0, MOVING_DOWN);

	// FALTA CREAR POWERUPS

	game->status = GAME_RUNNING;
	game->score = 0;
	game->currentLevel = 1;
	game->tickCounter = 0;
	game->aliensRemaining = ALIENS_NUMBER;
}

levelInit(game_t * game)
{
	game->tickCounter = 0;
	game->aliensRemaining = ALIENS_NUMBER;

	// reset ship
	setEntity(&game->ship, SHIP_INITIAL_X, SHIP_INITIAL_Y);

	// reset mothersip
	setEntity(&game->mothership, MOTHERSHIP_INITIAL_X, MOTHERSHIP_INITIAL_Y);

	int i, j;
	for(i = 0; i < ALIENS_ROWS; i++)
	{
		for(j = 0; j < ALIENS_COLS; j++)
		{
			setEntity(&game->);
		}
	}
}

void gameReset(game_t *game) // REVISAR SI VA
{
	gameInit(game);
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

void manageInput(game_t *game, input_t input)
{
	if (game->status == GAME_RUNNING)
	{
		int points;
		game->tickCounter++;

		gameUpdate(game, input);
		points = handleCollisions(game);
		incrementScore(game, points);
	}
}

/*******************************************************************************
 *******************************************************************************
						LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void gameUpdate(game_t *game, input_t input)
{
	updateShip(&game->ship, &game->shipBullet, input);
	updateBullet(&game->shipBullet);
	updateAliens(&game->aliens, &game->alienBullet);
	updateBullet(&game->alienBullet);
	updateMothership(&game->mothership);
}

static void updateShip(ship_t *ship, bullet_t *shipBullet, input_t input)
{
	switch (input)
	{
	case INPUT_LEFT:
		if(ship->entity.x > 0)
			moveShipLeft(ship);
		else
			ship->direction = STILL;
		break;

	case INPUT_RIGHT:
		if(ship->entity.x < SCREEN_SIZE)
			moveShipRight(ship);
		else
			ship->direction = STILL;
		break;

	case INPUT_SHOOT:
		if(ship->canShoot)
			shipShoot(ship, shipBullet);
		break;
	default:
		ship->direction = STILL;
		break;
	}
}

static void updateBullet(bullet_t *bullet)
{
	if(bullet->entity.x >= 0 && bullet->entity.x <= SCREEN_SIZE && bullet->entity.y >= 0 && bullet->entity.y <= SCREEN_SIZE)
	{
		moveBullet(bullet, BULLET_MOVE_RATE);
	}
	else
	{
		bullet->entity.isAlive = 0;
	}
}

static void updateAliens(alienFormation_t *aliens, bullet_t *alienBullet)
{
	int firstColumn = -1, lastColumn = ALIENS_COLS - 1, i, j;

	for (j = 0; j < ALIENS_COLS; j++)
	{
		for (i = 0; i < ALIENS_ROWS; i++)
		{
			if (aliens->alien[i][j].entity.isAlive)
			{
				lastColumn = j;
				if (firstColumn == -1)
					firstColumn = j;
			}
		}
	}

	switch (aliens->direction)
	{
	case MOVING_RIGHT:
		if (aliens->alien[0][lastColumn].entity.x < SCREEN_SIZE - ALIEN_WIDTH)
		{
			moveEnemiesRight(aliens, ALIEN_MIN_MOVE_INTERVAL);
		}
		else
		{
			aliens->direction = MOVING_DOWN;
		}
		break;

	case MOVING_LEFT:
		if (aliens->alien[0][firstColumn].entity.x > 0)
		{
			moveEnemiesLeft(aliens, ALIEN_MIN_MOVE_INTERVAL);
		}
		else
		{
			aliens->direction = MOVING_DOWN;
		}
		break;

	case MOVING_DOWN:
		moveEnemiesDown(aliens, ALIEN_MIN_MOVE_INTERVAL);
		if (aliens->alien[0][firstColumn].entity.x > 0)
			aliens->direction = MOVING_LEFT;
		else
			aliens->direction = MOVING_RIGHT;
		break;

	default:
		break;
	}
}

static void updateMothership(mothership_t *mothership)
{
	if(mothership->entity.x >= -MOTHERSHIP_WIDTH && mothership->entity.x <= SCREEN_SIZE+MOTHERSHIP_WIDTH)
	{
		moveMothership(mothership, MOTHERSHIP_MOVE_RATE);
	}
}

static int checkGameStatus(game_t * game)
{
	if (game->aliensRemaining == 0)
	{
		game->currentLevel++;
		nextLevel(game);
	}
	else if (!game->ship.entity.isAlive)
	{
		gameEnd(game);
	}
}