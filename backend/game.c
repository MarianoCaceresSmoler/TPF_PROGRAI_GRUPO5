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
#include <stdio.h>

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
 * @brief function to update a ship type entity
 * @param ship pointer to the ship type entity
 * @param moveLeft true if the left key is pressed, false otherwise
 * @param moveRight true if the right key is pressed, false otherwise
 * @param tickCounter the current game tick
 */
static void updateShip(ship_t *ship, bool moveLeft, bool moveRight, int tickCounter);

/**
 * @brief function to update a bullet type entity
 * @param bullet pointer to the bullet type entity
 */
static void updateBullet(bullet_t *bullet, int tickCounter);

/**
 * @brief function to update an alienformation type entity
 * @param aliens pointer to the alienformation type entity
 * @param tickCounter the current game tick
 * @param aliensRemaining the number of aliens alive in the level
 * @param activePowerUp array with the state of every power up
 */
static void updateAliens(alienFormation_t *aliens, int tickCounter, int aliensRemaining, int activePowerUp[POWERUP_TYPES]);

/**
 * @brief function to update a mothership type entity
 * @param bullet pointer to the mothership type entity
 * @param tickCounter the current game tick
 */
static void updateMothership(mothership_t *mothership, int tickCounter);

/**
 * @brief function to update every powerUp type entity
 * @param powerUp is an array with the powerUp structures to update
 * @param activePowerUp is an array with every active or inactive powerUp
 * @param tickCounter the current game tick

 */
static void updatePowerUps(powerUp_t powerUp[POWERUP_TYPES], int activePowerUp[POWERUP_TYPES], int tickCounter);

/**
 * @brief function to update an entity explosion/live state
 * @param entity pointer to the entity
 */
static void updateEntityExplosion(entity_t *entity);

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
	// initialize rand seed
	srand(time(NULL));

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
		game->activePowerUp[i] = 0;
	}

	// initialize game parameters
	game->status = GAME_MENU;
	game->score = 0;
	game->currentLevel = 1;
	game->tickCounter = 0;
	game->aliensRemaining = ALIENS_NUMBER;
	for (i = 0; i < MAX_NAME_CHARS; i++)
		game->nameTag[i] = '_';
	game->nameTag[i] = 0;
}

void levelInit(game_t *game)
{
	// set bullets to standby position
	setEntity(&game->shipBullet.entity, STANDBY_POSITION, STANDBY_POSITION);
	setEntity(&game->alienBullet.entity, STANDBY_POSITION, STANDBY_POSITION);

	int i;
	for (i = 0; i < POWERUP_TYPES; i++)
	{
		setEntity(&game->powerUp[i].entity, STANDBY_POSITION, STANDBY_POSITION);
		game->activePowerUp[i] = false;
	}

	// set ship to initial position
	setEntity(&game->ship.entity, SHIP_INITIAL_X, SHIP_INITIAL_Y);
	game->ship.canShoot = 1;

	game->mothership.entity.isAlive = 0;

	// set aliens to initial position
	setAliens(&game->aliens);
	game->aliens.direction = MOVING_RIGHT;

	// set game status
	game->status = GAME_LOADING;
	game->loadingTimer = LOADING_TIME;
	game->aliensRemaining = ALIENS_NUMBER;
	game->tickCounter = 0;

	if (game->currentLevel > 1)
		return;

	// set barriers to initial position only on first level
	setBarriers(game->barriers);
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

	score_t score;
	score.score = game->score;

	int i;
	for (i = 0; game->nameTag[i] != '\0'; i++)
		score.tag[i] = game->nameTag[i]; // copies the name

	game->scoreRank = updateScoreRank(score);
	if (getHighScores(game->highScores))
		printf("Error geting highscores");
}

void gameReset(game_t *game)
{
	game->ship.livesLeft = SHIP_INITIAL_LIVES;
	game->score = 0;
	game->ship.entity.explosionTimer = 0;
	game->aliens.canShoot = 0;
	game->ship.canShoot = 1;
	game->currentLevel = 1;
	levelInit(game);
}

void gameUpdate(game_t *game, inputStatus_t input)
{
	if (game->status != GAME_RUNNING)
	{
		gameEnd(game);
		return;
	}

	int points = 0;

	// updates game status
	if (game->aliensRemaining == 0)
	{
		game->currentLevel++;
		if (game->ship.livesLeft < SHIP_MAX_LIVES)
			game->ship.livesLeft++;

		levelInit(game);
	}
	else if (game->ship.entity.isAlive == false)
	{
		if (game->ship.livesLeft == 0)
		{
			gameEnd(game);
			game->status = GAME_END;
		}
		else
		{
			setEntity(&game->ship.entity, SHIP_INITIAL_X, SHIP_INITIAL_Y);
			game->ship.invencibilityTicks = INVENCIBILITY_TICKS;
		}
	}

	if (game->activePowerUp[REBUILDBARRIERS_POWERUP])
	{
		setBarriers(game->barriers);
		game->activePowerUp[REBUILDBARRIERS_POWERUP] = false;
	}
	if (game->activePowerUp[ONEUP_POWERUP] && game->ship.livesLeft < SHIP_MAX_LIVES)
	{
		game->ship.livesLeft++;
		game->activePowerUp[ONEUP_POWERUP] = false;
	}

	// updates entities
	updateShip(&game->ship, input.leftKeyPressed, input.rightKeyPressed, game->tickCounter);

	if (game->shipBullet.entity.isAlive == false)
		game->ship.canShoot = true;

	if (game->shipBullet.entity.isAlive)
		updateBullet(&game->shipBullet, game->tickCounter);
	else if (game->ship.canShoot && input.shootKeyPressed)
	{
		shootFromEntity(&game->shipBullet, &game->ship.entity);
		game->ship.canShoot = false;
	}

	if (game->alienBullet.entity.isAlive == false && game->tickCounter > ONE_SECOND * 3)
		game->aliens.canShoot = true;

	static int ticksSinceLastShot = 0;

	if (game->alienBullet.entity.isAlive)
		updateBullet(&game->alienBullet, game->tickCounter);
	else if (game->aliens.canShoot && game->activePowerUp[FREEZE_POWERUP] == false)
	{
		int alienColumnToShoot = getNearestColumnAlive(game->aliens, game->ship.entity.x);
		int alienRowToShoot = getNearestRowAlive(game->aliens, alienColumnToShoot);

		ticksSinceLastShot++;

		if (ticksSinceLastShot >= ALIENS_SHOOT_INTERVAL && alienRowToShoot >= 0 && alienColumnToShoot >= 0 && game->aliens.alien[alienRowToShoot][alienColumnToShoot].entity.isAlive)
		{
			shootFromEntity(&game->alienBullet, &game->aliens.alien[alienRowToShoot][alienColumnToShoot].entity);
			ticksSinceLastShot = 0;
		}
		game->aliens.canShoot = false;
	}

	updateAliens(&game->aliens, game->tickCounter, game->aliensRemaining, game->activePowerUp);

	updatePowerUps(game->powerUp, game->activePowerUp, game->tickCounter);

	if (game->mothership.entity.isAlive)
		updateMothership(&game->mothership, game->tickCounter);
	else if (game->tickCounter % MOTHERSHIP_TIMER == 0 && rand() % 100 < MOTHERSHIP_CHANCE)
	{
		game->mothership.direction = (rand() % 2 == 0) ? MOVING_RIGHT : MOVING_LEFT;
		setEntity(&game->mothership.entity, (game->mothership.direction == MOVING_LEFT ? MOTHERSHIP_RIGHT_INITIAL_X : MOTHERSHIP_LEFT_INITIAL_X), MOTHERSHIP_INITIAL_Y);
	}

	// updates score if an alien is killed
	if ((points = handleCollisions(game)))
	{
		game->score += points;

		int powerUpType = rand() % POWERUP_TYPES;
		if (game->powerUp[powerUpType].entity.isAlive == false && game->activePowerUp[powerUpType] == false && rand() % 100 < POWERUP_CHANCE)
			setEntity(&game->powerUp[powerUpType].entity, rand() % (SCREEN_SIZE - POWERUP_WIDTH), POWERUP_INITIAL_Y);
	}

	game->tickCounter++;
}

/*******************************************************************************
 *******************************************************************************
						LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void updateShip(ship_t *ship, bool moveLeft, bool moveRight, int tickCounter)
{
	#ifdef PLATFORM_RPI
		if (tickCounter % SHIP_MOVE_INTERVAL != 0)
			return;
	#endif

	if (ship->entity.explosionTimer > 0)
	{
		ship->canShoot = 0;
		updateEntityExplosion(&ship->entity);
	}
	else if (ship->entity.isAlive)
	{
		if (ship->invencibilityTicks > 0)
			ship->invencibilityTicks--;

		if (moveLeft && !moveRight && ship->entity.x > SHIP_LEFT_X_BORDER)
		{
			moveEntityX(&ship->entity, -SHIP_MOVE_RATE);
			ship->direction = MOVING_LEFT;
		}
		else if (moveRight && !moveLeft && ship->entity.x < SHIP_RIGHT_X_BORDER)
		{
			moveEntityX(&ship->entity, SHIP_MOVE_RATE);
			ship->direction = MOVING_RIGHT;
		}
		else
		{
			ship->direction = STILL;
		}
	}
}

static void updateBullet(bullet_t *bullet, int tickCounter)
{
	if (bullet->entity.isAlive == false)
		return;

	if (bullet->entity.x < 0 || bullet->entity.x > SCREEN_SIZE || bullet->entity.y < 0 || bullet->entity.y > SCREEN_SIZE)
	{
		bullet->entity.isAlive = 0;
		return;
	}

	#ifdef PLATFORM_RPI
		if (tickCounter % BULLET_MOVE_INTERVAL != 0)
		return;
	#endif
	
	moveEntityY(&bullet->entity, bullet->direction * BULLET_MOVE_RATE);
}

static void updateAliens(alienFormation_t *aliens, int gameTicks, int aliensRemaining, int activePowerUp[POWERUP_TYPES])
{
	int firstColumn = -1, lastColumn = ALIENS_COLS - 1, i, j;
	static int rowToMove;

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

	if (activePowerUp[ALIENRETREAT_POWERUP])
	{
		int retreatMoveRate = -(aliens->alien[0][0].entity.y - ALIENS_INITIAL_Y);
		for (i = 0; i < ALIENS_ROWS; i++)
		{
			for (j = 0; j < ALIENS_COLS; j++)
			{
				moveEntityY(&aliens->alien[i][j].entity, retreatMoveRate);
			}
		}
		activePowerUp[ALIENRETREAT_POWERUP] = false;
	}

	// rowToMove is used to move the alien rows separately
	if (gameTicks == 0)
		rowToMove = ALIENS_ROWS;

	// aliens velocity adjust
	float aliensFactor = (float)(ALIENS_NUMBER - aliensRemaining) / (ALIENS_NUMBER - 1); // 0 at start, 1 when 1 alien left
	float timeFactor = (float)gameTicks / 1000.0f;										 // Goes up with time until it reaches a cap

	if (timeFactor > 1.0f)
		timeFactor = 1.0f;

	float speedFactor = aliensFactor * ALIEN_COUNT_SPEED_WEIGHT + timeFactor * TIME_SPEED_WEIGHT; // Ponderated combination

	int moveInterval = ALIEN_MAX_MOVE_INTERVAL - (int)((ALIEN_MAX_MOVE_INTERVAL - ALIEN_MIN_MOVE_INTERVAL) * speedFactor); // Calculates new move interval

	if (moveInterval < ALIEN_MIN_MOVE_INTERVAL)
		moveInterval = ALIEN_MIN_MOVE_INTERVAL; // to not surpass the min move interval

	if (gameTicks % moveInterval != 0 || activePowerUp[FREEZE_POWERUP])
		return;

	if (rowToMove == 0)
		rowToMove = ALIENS_ROWS - 1;
	else
		rowToMove--;

	switch (aliens->direction)
	{
	case MOVING_RIGHT:
		if (aliens->alien[0][lastColumn].entity.x < ALIENS_X_RIGHT_BORDER)
		{
			for (i = 0; i < ALIENS_COLS; i++)
			{
				moveEntityX(&aliens->alien[rowToMove][i].entity, ALIEN_X_MOVE_RATE);
				aliens->alien[rowToMove][i].isMoving = !(aliens->alien[rowToMove][i].isMoving);
			}
		}
		else
		{
			aliens->direction = MOVING_DOWN;
			rowToMove++;
		}
		break;

	case MOVING_LEFT:
		if (aliens->alien[0][firstColumn].entity.x > ALIENS_X_LEFT_BORDER)
		{
			for (i = 0; i < ALIENS_COLS; i++)
			{
				moveEntityX(&aliens->alien[rowToMove][i].entity, -ALIEN_X_MOVE_RATE);
				aliens->alien[rowToMove][i].isMoving = !(aliens->alien[rowToMove][i].isMoving);
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
			moveEntityY(&aliens->alien[rowToMove][i].entity, ALIEN_Y_MOVE_RATE);
		}

		if (rowToMove == 0)
		{
			if (aliens->alien[0][firstColumn].entity.x > 2 * ALIEN_X_MOVE_RATE)
				aliens->direction = MOVING_LEFT;
			else
				aliens->direction = MOVING_RIGHT;
		}
		break;

	default:
		break;
	}
}

static void updateMothership(mothership_t *mothership, int tickCounter)
{
	#ifdef PLATFORM_RPI
		if (tickCounter % MOTHERSHIP_MOVE_INTERVAL != 0)
			return;
	#endif

	if (mothership->entity.explosionTimer > 0)
		updateEntityExplosion(&mothership->entity);
	else if (mothership->entity.isAlive && mothership->entity.x >= MOTHERSHIP_LEFT_INITIAL_X && mothership->entity.x <= MOTHERSHIP_RIGHT_INITIAL_X)
		moveEntityX(&mothership->entity, mothership->direction * MOTHERSHIP_MOVE_RATE / 2);
	else
		mothership->entity.isAlive = false;
}

static void updatePowerUps(powerUp_t powerUp[POWERUP_TYPES], int activePowerUp[POWERUP_TYPES], int tickCounter)
{

	int i;
	for (i = 0; i < POWERUP_TYPES; i++)
	{
		if (activePowerUp[i] && powerUp[i].ticksLeft > 0)
		{
			powerUp[i].ticksLeft--;
			if (powerUp[i].ticksLeft == 0)
			{
				activePowerUp[i] = false;
			}
		}
		else if (powerUp[i].entity.isAlive == true)
		{
			if (powerUp[i].entity.x < 0 || powerUp[i].entity.x > SCREEN_SIZE || powerUp[i].entity.y < 0 || powerUp[i].entity.y > SCREEN_SIZE)
				powerUp[i].entity.isAlive = 0;
			else
			{
				#ifdef PLATFORM_RPI
					if (tickCounter % POWERUP_MOVE_INTERVAL != 0)
						return;
				#endif
				moveEntityY(&powerUp[i].entity, POWERUP_MOVE_RATE);
			}
		}
	}
}

static void updateEntityExplosion(entity_t *entity)
{
	entity->explosionTimer--;
	if (!entity->explosionTimer)
	{
		entity->isAlive = 0; // when the timer hits 0, the entity is no longer alive
	}
}
