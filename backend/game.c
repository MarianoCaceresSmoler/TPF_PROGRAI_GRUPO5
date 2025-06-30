/***************************************************************************//**
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
static void gameUpdate(game_t * game, input_t input);
static void updateShip(ship_t * ship, bullet_t * shipBullet, input_t input);
static void updateBullet(bullet_t * bullet);
static void updateAliens(alienFormation_t * aliens, bullet_t * alienBullet);
static void updateMothership(mothership_t * mothership);


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

void gameInit(game_t * game)
{
	game->status = GAME_RUNNING;
	game->score = 0;
	game->currentLevel = 1;
	game->tickCounter = 0;
	game->aliensRemaining = ALIENS_NUMBER;
}

void gameReset(game_t * game)
{
	gameInit(game);
}

void gamePause(game_t * game)
{
	game->status = GAME_PAUSED;
}

void gameResume(game_t * game)
{
	game->status = GAME_RUNNING;
}

void gameEnd(game_t * game)
{
	game->status = GAME_END;
}

void resetLevel(game_t * game)
{
	game->tickCounter = 0;
	game->aliensRemaining = ALIENS_NUMBER;
}

void nextLevel(game_t * game)
{
	game->currentLevel += 1;
	resetLevel(game);
}

void manageInput(game_t * game, input_t input)
{
	if (game->status == GAME_RUNNING)
	{
		int points;

		game->tickCounter++;

		gameUpdate(game, input);
		points = handleCollisions(game);
		incrementScore(game, points);

		if (game->aliensRemaining == 0)
		{
			game->currentLevel++;
			nextLevel(game);
		}
		else if(!game->ship.entity.isAlive)
		{
			gameEnd(game);
		}

	}
	
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void gameUpdate(game_t * game, input_t input)
{
	updateShip(&game->ship, &game->shipBullet, input);
	updateBullet(&game->shipBullet);
	updateAliens(&game->aliens, &game->alienBullet);
	updateBullet(&game->alienBullet);
	updateMothership(&game->mothership);
}

static void updateShip(ship_t * ship, bullet_t * shipBullet, input_t input)
{
	switch (input)
	{
	case INPUT_LEFT:
		moveShipLeft(ship);
		break;
	
	case INPUT_RIGHT:
		moveShipRight(ship);
		break;

	case INPUT_SHOOT:
		shipShoot(ship, shipBullet);
		break;
	default:
		ship->movingLeft = 0;
		ship->movingRight = 0;
		break;
	}
	
}

static void updateBullet(bullet_t * bullet)
{
	moveBullet(bullet, BULLET_MOVE_RATE);
}

static void updateAliens(alienFormation_t * aliens, bullet_t * alienBullet)
{
	int firstColumn = -1, lastColumn, i, j;

	for (j = 0; j < ALIENS_COLS; j++)
	{
		for (i = 0; i < ALIENS_ROWS; i++)
		{
			if(aliens->alien[i][j].entity.isAlive)
			{
				lastColumn = j;
				if(firstColumn == -1) firstColumn = j;
			}
		}
	}

	switch (aliens->direction)
	{
	case MOVING_RIGHT:
		moveEnemiesRight(aliens, ALIEN_MIN_MOVE_INTERVAL);
		break;
	
	case MOVING_LEFT:
		moveEnemiesLeft(aliens, ALIEN_MIN_MOVE_INTERVAL);
		break;

	case MOVING_DOWN:
		moveEnemiesDown(aliens, ALIEN_MIN_MOVE_INTERVAL);
		break;

	default:
		break;
	}
	
	

}

static void updateMothership(mothership_t * mothership)
{
	moveMothership(mothership, MOTHERSHIP_MOVE_RATE);
}

 