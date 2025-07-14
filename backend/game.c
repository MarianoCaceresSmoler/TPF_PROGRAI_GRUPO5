/***************************************************************************/ /**
   @game.c
   @Game structs and functions
   @Grupo_5
  ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

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

/**
 * @brief function to update a ship type entity
 * @param ship pointer to the ship type entity
 * @param moveLeft true if the left key is pressed, false otherwise
 * @param moveRight true if the right key is pressed, false otherwise
 * @param tickCounter the current game ticks
 * @return 0 if the ship was updated successfully, -1 otherwise
 */
static int updateShip(ship_t *ship, bool moveLeft, bool moveRight, int tickCounter);

/**
 * @brief function to update a bullet type entity
 * @param bullet pointer to the bullet type entity
 * @return 0 if the bullet was updated successfully, -1 otherwise
 */
static int updateBullet(bullet_t *bullet, int tickCounter);

/**
 * @brief function to update an alienformation type entity
 * @param aliens pointer to the alienformation type entity
 * @param tickCounter the current game ticks
 * @param aliensRemaining the number of aliens alive in the level
 * @param activePowerUp array with the state of every power up
 * @return 0 if the alien formation was updated successfully, -1 otherwise
 */
static int updateAliens(alienFormation_t *aliens, int tickCounter, int aliensRemaining, int activePowerUp[POWERUP_TYPES]);

/**
 * @brief function to update a mothership type entity
 * @param bullet pointer to the mothership type entity
 * @param tickCounter the current game ticks
 * @return 0 if the mothership was updated successfully, -1 otherwise
 */
static int updateMothership(mothership_t *mothership, int tickCounter);

/**
 * @brief function to update every powerUp type entity
 * @param powerUp is an array with the powerUp structures to update
 * @param activePowerUp is an array with every active or inactive powerUp
 * @param tickCounter the current game ticks
 * @return 0 if the powerups were updated successfully, -1 otherwise

 */
static int updatePowerUps(powerUp_t powerUp[POWERUP_TYPES], int activePowerUp[POWERUP_TYPES], int tickCounter);

/**
 * @brief function to update an entity explosion/live state
 * @param entity pointer to the entity
 * @return 0 if the entity explosion was updated successfully, -1 otherwise
 */
static int updateEntityExplosion(entity_t *entity);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static int temperaturas_actuales[4];+

static bool randSeedIsSet = false;

/*******************************************************************************
 *******************************************************************************
						GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


// COMENTAR Y VALIDAR TODO EN LAS FUNCIONES GLOBALES Y SUS PROTOTIPOS EN GAME.H
// DESPUES, SEGUIR CON EL FRONTEND


void gameInit(game_t *game)
{
	if (game) // only inits the game if game parameter is not NULL
	{
		// initialize rand seed the first time this function is called
		if(!randSeedIsSet)
		{
			srand(time(NULL));
			randSeedIsSet = true;
		}

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
			game->nameTag[i] = '_'; // game tag is set to "___" by default
		game->nameTag[i] = 0;
		
	}
	else
	{
		printf("Error: game parameter is NULL\n");
	}

}

void levelInit(game_t *game)
{
	if (!game) // only inits the level if game parameter is not NULL
	{
		printf("Error: game parameter is NULL\n");
		return;	
	}

	// set bullets to standby position
	if(setEntity(&game->shipBullet.entity, STANDBY_POSITION, STANDBY_POSITION))
	{
		printf("Error setting ship bullet\n");
		game->status = GAME_ERROR; // sets game status to error if there was an error setting ship bullet
		return;
	}
	
	if(setEntity(&game->alienBullet.entity, STANDBY_POSITION, STANDBY_POSITION))
	{
		printf("Error setting alien bullet\n");
		game->status = GAME_ERROR; // sets game status to error if there was an error setting alien bullet
		return;
	}

	int i;
	for (i = 0; i < POWERUP_TYPES; i++)
	{
		// set power up to standby position
		if(setEntity(&game->powerUp[i].entity, STANDBY_POSITION, STANDBY_POSITION))
		{
			printf("Error setting power up\n");
			game->status = GAME_ERROR; // sets game status to error if there was an error setting power up
			return;
		}
		game->activePowerUp[i] = false;
	}

	// set ship to initial position
	if(setEntity(&game->ship.entity, SHIP_INITIAL_X, SHIP_INITIAL_Y))
	{
		printf("Error setting ship\n");
		game->status = GAME_ERROR; // sets game status to error if there was an error setting the ship
		return;
	}

	// initial ship and mothership values
	game->ship.canShoot = 1;
	game->mothership.entity.isAlive = 0;

	// set aliens to initial position
	if(setAliens(&game->aliens))
	{
		printf("Error setting aliens\n");
		game->status = GAME_ERROR; // sets game status to error if there was an error setting the aliens
		return;
	}
	game->aliens.direction = MOVING_RIGHT; // initially aliens move to the right

	// set game status and information
	game->status = GAME_LOADING;
	game->loadingTimer = LOADING_TIME;
	game->aliensRemaining = ALIENS_NUMBER;
	game->tickCounter = 0;

	// set barriers to initial position only on first level
	if (game->currentLevel > 1)
		return;

	if(setBarriers(game->barriers))
	{
		printf("Error setting barriers\n");
		game->status = GAME_ERROR; // sets game status to error if there was an error setting the barriers
		return;
	}
}

void gamePause(game_t *game)
{
	if(!game) // check if game is null
	{
		printf("Error: game parameter is NULL\n");
		return;		
	}

	game->status = GAME_PAUSED; // set game status to paused
}

void gameResume(game_t *game)
{
	if(!game) // check if game is null
	{
		printf("Error: game parameter is NULL\n");
		return;		
	}

	game->status = GAME_RUNNING; // set game status to running
}

void gameEnd(game_t *game)
{
	if(!game) // check if game is null
	{
		printf("Error: game parameter is NULL\n");
		return;		
	}

	game->status = GAME_END; // set game status to end

	score_t score;
	score.score = game->score; // copies final score

	int i;
	for (i = 0; game->nameTag[i] != '\0'; i++)
		score.tag[i] = game->nameTag[i]; // copies the user tag name

	game->scoreRank = updateScoreRank(score); // saves final score rank, and updates the highscores file (if necessary)

	if(game->scoreRank < 0) // if there was an error updating the scores file, set game status to GAME_ERROR
	{		
		printf("Error updating highscores\n");
		game->status = GAME_ERROR;
		return;
	}
	
	// saves the highscores from the highscores file to game
	if (getHighScores(game->highScores)) // if there was an error reading the scores file, set game status to GAME_ERROR
	{
		printf("Error getting highscores\n");
		game->status = GAME_ERROR;
	}
}

void gameReset(game_t *game)
{
	if(!game) // check if game is null
	{
		printf("Error: game parameter is NULL\n");
		return;		
	}

	// resets game state and information
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
	if(!game) // check if game is null
	{
		printf("Error: game parameter is NULL\n");
		return;		
	}

	if (game->status != GAME_RUNNING) // if game is not running, ends it
	{
		gameEnd(game);
		return;
	}

	int points = 0;

	if (game->aliensRemaining == 0)
	{
		// if all aliens have been killed, level is complete
		game->currentLevel++;
		if (game->ship.livesLeft < SHIP_MAX_LIVES) // if ship has not reached max lives, add one
			game->ship.livesLeft++;

		levelInit(game);
	}
	else if (game->ship.entity.isAlive == false) // if the ship has been destroyed, updates it state
	{
		if (game->ship.livesLeft == 0) // if ship has no lives left, game is over
		{
			gameEnd(game);
			game->status = GAME_END;
		}
		else
		{
			if(setEntity(&game->ship.entity, SHIP_INITIAL_X, SHIP_INITIAL_Y)) // resets ship position
			{
				printf("Error setting ship\n");
				game->status = GAME_ERROR; // if there was an error setting the ship, set game status to GAME_ERROR
				return;
			}
			game->ship.invencibilityTicks = INVENCIBILITY_TICKS; // set invencibility timer for the ship
		}
	}

	// If rebuild barries power up was taken, rebuild them
	if (game->activePowerUp[REBUILDBARRIERS_POWERUP])
	{
		if(setBarriers(game->barriers))
		{
			printf("Error setting barriers\n");
			game->status = GAME_ERROR; // if there was an error setting the barriers, set game status to GAME_ERROR
			return;
		}
		game->activePowerUp[REBUILDBARRIERS_POWERUP] = false;
	}

	// If one up power up was taken, add one to the ship's lives (only if ship has not reached max lives)
	if (game->activePowerUp[ONEUP_POWERUP] && game->ship.livesLeft < SHIP_MAX_LIVES)
	{
		game->ship.livesLeft++;
		game->activePowerUp[ONEUP_POWERUP] = false;
	}

	if(updateShip(&game->ship, input.leftKeyPressed, input.rightKeyPressed, game->tickCounter)) // updates ship entity
	{
		printf("Error updating ship\n");
		game->status = GAME_ERROR; //  if there was an error updating the ship, set game status to GAME_ERROR
		return;
	}

	if (game->shipBullet.entity.isAlive == false)
		game->ship.canShoot = true; // if ship bullet has been destroyed, ship can shoot again

	if (game->shipBullet.entity.isAlive)
	{
		if(updateBullet(&game->shipBullet, game->tickCounter)) // if ship bullet is alive, update it
		{
			printf("Error updating ship bullet\n");
			game->status = GAME_ERROR; // if there was an error updating the ship bullet, set game status to GAME_ERROR
			return;
		}
	}
	else if (game->ship.canShoot && input.shootKeyPressed) // if ship can shoot and shoot key was pressed, shoot from the ship and disable can shoot
	{
		if(shootFromEntity(&game->shipBullet, &game->ship.entity))
		{
			printf("Error shooting from ship\n");
			game->status = GAME_ERROR; // if there was an error shooting from the ship, set game status to GAME_ERROR
			return;
		}
		game->ship.canShoot = false; // disable ship's ability to shoot
	}

	if (game->alienBullet.entity.isAlive == false && game->tickCounter > ONE_SECOND * 3) 
		game->aliens.canShoot = true; // allows the aliens to shoot only after 3 seconds of gameplay, and only if their bullet has been destroyed

	static int ticksSinceLastShot = 0; // to later get the number of ticks since the last shot

	if (game->alienBullet.entity.isAlive)
	{
		if(updateBullet(&game->alienBullet, game->tickCounter)) // if alien bullet is alive, updates it
		{
			printf("Error updating alien bullet\n");
			game->status = GAME_ERROR; // if there was an error updating the alien bullet, set game status to GAME_ERROR
			return;
		}
	}
	else if (game->aliens.canShoot && game->activePowerUp[FREEZE_POWERUP] == false)
	{
		// if aliens can shoot and no freeze powerup is active, shoot from the aliens

		// get the position to shoot from
		int alienColumnToShoot = getNearestColumnAlive(game->aliens, game->ship.entity.x);
		int alienRowToShoot = getNearestRowAlive(game->aliens, alienColumnToShoot);		 

		ticksSinceLastShot++; // updates the ticks since the last shot

		if (ticksSinceLastShot >= ALIENS_SHOOT_INTERVAL && alienRowToShoot >= 0 && alienColumnToShoot >= 0 && game->aliens.alien[alienRowToShoot][alienColumnToShoot].entity.isAlive)
		{
			// if the number of ticks since the last shot is greater than or equal to the shoot interval, shoot from and alien (only if it is defined and alive)
			if(shootFromEntity(&game->alienBullet, &game->aliens.alien[alienRowToShoot][alienColumnToShoot].entity))
			{
				printf("Error shooting from alien\n");
				game->status = GAME_ERROR; // if there was an error shooting from the alien, set game status to GAME_ERROR
				return;
			}
			ticksSinceLastShot = 0; // resets the number of ticks after shooting
		}
		game->aliens.canShoot = false; // disable aliens ability to shoot
	}

	// update the alien formation
	int aliensUpdateResult = updateAliens(&game->aliens, game->tickCounter, game->aliensRemaining, game->activePowerUp);
	if(aliensUpdateResult == -1)
	{
		printf("Error updating aliens\n");
		game->status = GAME_ERROR; // if there was an error updating the aliens, set game status to GAME_ERROR
		return;
	}
	else if(aliensUpdateResult)
	{
		gameEnd(game); // ends the game if the function returns 1 (aliens reached the bottom of the screen)
		return;
	}

	// update the power ups
	if(updatePowerUps(game->powerUp, game->activePowerUp, game->tickCounter))
	{
		printf("Error updating power ups\n");
		game->status = GAME_ERROR; // if there was an error updating power ups, set game status to GAME_ERROR
		return;
	}

	if (game->mothership.entity.isAlive)
	{
		// if the mothership is alive, update it
		if(updateMothership(&game->mothership, game->tickCounter))
		{
			printf("Error updating mothership\n");
			game->status = GAME_ERROR; // if there was an error updating the mothership, set game status to GAME_ERROR
			return;
		}
	}
	else if (game->tickCounter && game->tickCounter % MOTHERSHIP_TIMER == 0 && rand() % 100 < MOTHERSHIP_CHANCE)
	{
		// if the mothership is not alive, spawn it after a certain number of ticks and taking into account the mothership chance

		game->mothership.direction = (rand() % 2 == 0) ? MOVING_RIGHT : MOVING_LEFT; // to get a random direction

		// spawn the mothership in the left or right part of the screen, depending on the random direction stored
		if(setEntity(&game->mothership.entity, (game->mothership.direction == MOVING_LEFT ? MOTHERSHIP_RIGHT_INITIAL_X : MOTHERSHIP_LEFT_INITIAL_X), MOTHERSHIP_INITIAL_Y))
		{
			printf("Error setting mothership entity\n");
			game->status = GAME_ERROR; // sets game status to error if there was an error setting the mothership entity
			return;
		}
	}

	// updates score if an alien was killed
	points = handleCollisions(game);

	if (points < 0)
	{
		printf("Error handling collidions\n");
		game->status = GAME_ERROR; // sets error status if collisions points received are less than 0
		return;
	}	
	else if (points) // if points are greater than 0, it means an alien was killed
	{
		game->score += points; // updates the score

		// gives the chance to set a powerup
		int powerUpType = rand() % POWERUP_TYPES;
		if (game->powerUp[powerUpType].entity.isAlive == false && game->activePowerUp[powerUpType] == false && rand() % 100 < POWERUP_CHANCE)
		{
			// set the powerup entity at top of the screen, at a random initial x position
			if(setEntity(&game->powerUp[powerUpType].entity, rand() % (SCREEN_SIZE - POWERUP_WIDTH), POWERUP_INITIAL_Y))
			{
				printf("Error setting powerup entity\n");
				game->status = GAME_ERROR; // sets game status to error if there was an error setting power ups
				return;
			}
		}
	}

	game->tickCounter++; // increment the tick counter after all the updates

}

/*******************************************************************************
 *******************************************************************************
						LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static int updateShip(ship_t *ship, bool moveLeft, bool moveRight, int tickCounter)
{
	#ifdef PLATFORM_RPI // in rpi, updates ship taking into account the ship move interval
		if (tickCounter % SHIP_MOVE_INTERVAL != 0)
			return;
	#endif

	if(!ship)
	{
		printf("Error: ship is NULL\n");
		return -1; // returns error if ship is NULL
	}

	if (ship->entity.explosionTimer > 0) // if ship is exploding, only updates the explosion
	{
		ship->canShoot = 0;
		if(updateEntityExplosion(&ship->entity))
		{
			printf("Error updating ship explosion\n");
			return -1; // returns error if there was an error updating the ship explosion
		}
	}
	else if (ship->entity.isAlive)
	{
		if (ship->invencibilityTicks > 0) // if ship is invincible, decreases the invencibility timer
			ship->invencibilityTicks--;

		// Updates ship position and direction
		if (moveLeft && !moveRight && ship->entity.x > SHIP_LEFT_X_BORDER)
		{			
			if(moveEntityX(&ship->entity, -SHIP_MOVE_RATE))
			{
				printf("Error moving ship in X\n");
				return -1; // returns error if there was an error moving the ship
			}
			ship->direction = MOVING_LEFT;
		}
		else if (moveRight && !moveLeft && ship->entity.x < SHIP_RIGHT_X_BORDER)
		{			
			if(moveEntityX(&ship->entity, SHIP_MOVE_RATE))
			{
				printf("Error moving ship in X\n");
				return -1; // returns error if there was an error moving the ship
			}
			ship->direction = MOVING_RIGHT;
		}
		else
		{
			ship->direction = STILL;
		}
	}

	return 0;
}

static int updateBullet(bullet_t *bullet, int tickCounter)
{
	if(!bullet)
	{
		printf("Error: bullet is NULL\n");
		return -1; // returns error if bullet is NULL
	}

	if (bullet->entity.isAlive) // only updates if the bullet is alive
	{
		// If the bullet goes out of the screen, it is removed
		if (bullet->entity.x < 0 || bullet->entity.x > SCREEN_SIZE || bullet->entity.y < 0 || bullet->entity.y > SCREEN_SIZE)
		{
			bullet->entity.isAlive = 0;
		}
		else
		{
			#ifdef PLATFORM_RPI // in rpi, updates bullet taking into account the bullet move interval
				if (tickCounter % BULLET_MOVE_INTERVAL != 0)
				return 0;
			#endif
			
			if(moveEntityY(&bullet->entity, bullet->direction * BULLET_MOVE_RATE)) // updates bullet position
			{
				printf("Error moving bullet in Y\n");
				return -1; // returns error if there was an error moving the bullet
			}
		}		
	}
	return 0;
}

static int updateAliens(alienFormation_t *aliens, int gameTicks, int aliensRemaining, int activePowerUp[POWERUP_TYPES])
{

	if(!aliens)
	{
		printf("Error: aliens is NULL\n");
		return -1; // returns error if aliens is NULL
	}

	int firstColumn = -1, lastColumn = ALIENS_COLS - 1, i, j;
	static int rowToMove;

	// Updates explosion animation for each alien if needed
	for (i = 0; i < ALIENS_ROWS; i++)
	{
		for (j = 0; j < ALIENS_COLS; j++)
		{
			if (aliens->alien[i][j].entity.explosionTimer > 0)
			{
				if(updateEntityExplosion(&aliens->alien[i][j].entity))
				{
					printf("Error updating alien explosion\n");
					return -1; // returns error if there was an error updating the alien explosion
				}
			}
		}
	}

	// Finds the first and last column that contains at least one alive alien
	firstColumn = getFirstColumnAlive(*aliens);
	lastColumn = getLastColumnAlive(*aliens);

	if(firstColumn < 0 || lastColumn < 0 || firstColumn > ALIENS_COLS || lastColumn > ALIENS_COLS)
	{
		printf("Error geting first or last column alive\n");
		return -1; // returns an error if there are no aliens alive to update
	}

	// If the ALIENRETREAT power-up is active, move all aliens back to their starting Y position
	if (activePowerUp[ALIENRETREAT_POWERUP])
	{
		int retreatMoveRate = -(aliens->alien[0][0].entity.y - ALIENS_INITIAL_Y);
		for (i = 0; i < ALIENS_ROWS; i++)
		{
			for (j = 0; j < ALIENS_COLS; j++)
			{
				if(moveEntityY(&aliens->alien[i][j].entity, retreatMoveRate))
				{
					printf("Error moving alien in Y\n");
					return -1; // returns an error if there was an error moving the alien
				}
			}
		}
		activePowerUp[ALIENRETREAT_POWERUP] = false;
	}

	// Initialize which row will move first (rowToMove cycles through the rows)
	if (gameTicks == 0)
		rowToMove = ALIENS_ROWS;

	// --- Aliens movement speed logic ---
	// aliensFactor: increases as more aliens are killed (0 at start, 1 when only 1 left)
	float aliensFactor = (float)(ALIENS_NUMBER - aliensRemaining) / (ALIENS_NUMBER - 1);
	// timeFactor: increases over time and is capped at 1.0
	float timeFactor = (float)gameTicks / 1000.0f;

	if (timeFactor > 1.0f)
		timeFactor = 1.0f;

	// speedFactor: weighted combination of aliensFactor and timeFactor
	float speedFactor = aliensFactor * ALIEN_COUNT_SPEED_WEIGHT + timeFactor * TIME_SPEED_WEIGHT;

	// Calculate how often the aliens should move based on the current speed factor
	int moveInterval = ALIEN_MAX_MOVE_INTERVAL - (int)((ALIEN_MAX_MOVE_INTERVAL - ALIEN_MIN_MOVE_INTERVAL) * speedFactor);

	if (moveInterval < ALIEN_MIN_MOVE_INTERVAL)
		moveInterval = ALIEN_MIN_MOVE_INTERVAL; // to not surpass the min move interval

	// If it's not time to move or the FREEZE power-up is active, skip the update
	if (gameTicks % moveInterval != 0 || activePowerUp[FREEZE_POWERUP])
		return 0;

	// Select the next row to move
	if (rowToMove == 0)
		rowToMove = ALIENS_ROWS - 1;
	else
		rowToMove--;

	// --- Handle alien movement based on current direction ---
	switch (aliens->direction)
	{
	case MOVING_RIGHT:
		// If not reached the right border, move the selected row to the right
		if (aliens->alien[0][lastColumn].entity.x < ALIENS_X_RIGHT_BORDER)
		{
			for (i = 0; i < ALIENS_COLS; i++)
			{
				if(moveEntityX(&aliens->alien[rowToMove][i].entity, ALIEN_X_MOVE_RATE))
				{
					printf("Error moving aliens in X\n");
					return -1; // returns error if there was an error moving the aliens
				}
				aliens->alien[rowToMove][i].isMoving = !(aliens->alien[rowToMove][i].isMoving); // Toggle animation frame
			}
		}
		else
		{
			// If border is reached, change direction to down
			aliens->direction = MOVING_DOWN; 
			rowToMove++;
		}
		break;

	case MOVING_LEFT:
		// If not reached the left border, move the selected row to the left
		if (aliens->alien[0][firstColumn].entity.x > ALIENS_X_LEFT_BORDER)
		{
			for (i = 0; i < ALIENS_COLS; i++)
			{
				if(moveEntityX(&aliens->alien[rowToMove][i].entity, -ALIEN_X_MOVE_RATE))
				{
					printf("Error moving aliens in X\n");
					return -1; // returns error if there was an error moving the aliens
				}
				aliens->alien[rowToMove][i].isMoving = !(aliens->alien[rowToMove][i].isMoving); // Toggle animation frame
			}
		}
		else
		{
			// If border is reached, change direction to down
			aliens->direction = MOVING_DOWN;
			rowToMove++;
		}
		break;

	case MOVING_DOWN:		
		// Move the selected row down

		for (i = 0; i < ALIENS_COLS; i++)
		{
			if(moveEntityY(&aliens->alien[rowToMove][i].entity, ALIEN_Y_MOVE_RATE))
			{
				printf("Error moving aliens in Y\n");
				return -1; // returns error if there was an error moving the aliens
			}
		}

		// After moving down all rows, decide new direction (left or right)
		if (rowToMove == 0)
		{
			if (aliens->alien[0][firstColumn].entity.x > 2 * ALIEN_X_MOVE_RATE)
				aliens->direction = MOVING_LEFT;
			else
				aliens->direction = MOVING_RIGHT;
		}		

		int lastRowAlive = getLastRowAlive(*aliens);

		if (aliens->alien[lastRowAlive][0].entity.y >= SCREEN_HEIGHT)
			return 1; // if the aliens have reached the bottom of the screen, return 1 to indicate game over

		break;

	default:
		printf("Error: moving direction not defined\n");
		return -1; // returns an error if the moving direction is not defined
		break;
	}

	return 0;
}

static int updateMothership(mothership_t *mothership, int tickCounter)
{
	#ifdef PLATFORM_RPI //  in rpi, updates mothership taking into account the mothership move interval
		if (tickCounter % MOTHERSHIP_MOVE_INTERVAL != 0)
			return;
	#endif

	if(!mothership)
	{
		printf("Error: mothership is NULL\n");
		return -1; // returns an error if the mothership is NULL
	}

	// If the explosion timer is set, updates the explosion, othewhise moves the entity in the X axis
	if (mothership->entity.explosionTimer > 0)
	{
		if(updateEntityExplosion(&mothership->entity))
		{
			printf("Error updating mothership explosion");
			return -1; // returns error if there was an error updating the mothership explosion
		}
	}
	else if (mothership->entity.isAlive && mothership->entity.x >= MOTHERSHIP_LEFT_INITIAL_X && mothership->entity.x <= MOTHERSHIP_RIGHT_INITIAL_X)
	{
		if(moveEntityX(&mothership->entity, mothership->direction * MOTHERSHIP_MOVE_RATE / 2))
		{
			printf("Error moving mothership in X\n");
			return -1; // returns an error if the mothership could not be moved in the X axis
		}
	}
	else
		mothership->entity.isAlive = false; // if mothership is not anymore in the screen, kills it
	
	return 0;
}

static int updatePowerUps(powerUp_t powerUp[POWERUP_TYPES], int activePowerUp[POWERUP_TYPES], int tickCounter)
{
	int i;
	for (i = 0; i < POWERUP_TYPES; i++)
	{
		// If a power-up is currently active and has time left
		if (activePowerUp[i] && powerUp[i].ticksLeft > 0)
		{
			powerUp[i].ticksLeft--; // Decrease the remaining time
			if (powerUp[i].ticksLeft == 0)
			{
				activePowerUp[i] = false; // Deactivate power-up when time is up
			}
		}
		// If the power-up is not active but is alive on screen
		else if (powerUp[i].entity.isAlive == true)
		{
			// Check if the power-up has gone out of screen bounds
			if (powerUp[i].entity.x < 0 || powerUp[i].entity.x > SCREEN_SIZE || powerUp[i].entity.y < 0 || powerUp[i].entity.y > SCREEN_SIZE)
				powerUp[i].entity.isAlive = 0; // Remove the power-up from the game
			else
			{
				#ifdef PLATFORM_RPI // in rpi, move only at certain tick intervals
					if (tickCounter % POWERUP_MOVE_INTERVAL != 0)
						return;
				#endif
				
				if(moveEntityY(&powerUp[i].entity, POWERUP_MOVE_RATE)) // move the power-up downwards
				{
					printf("Error moving powerup in Y\n");
					return -1; // returns an error if the powerup could not be moved in the Y axis
				}
			}
		}
	}

	return 0;
}

static int updateEntityExplosion(entity_t *entity)
{
	if(!entity)
	{
		printf("Error: entity is null\n");
		return -1; // returns error if entity is NULL
	}

	entity->explosionTimer--; // Decrement explosion timer

	// Once the timer reaches zero, the entity is no longer alive
	if (!entity->explosionTimer)
	{
		entity->isAlive = 0; 
	}

	return 0;
}
