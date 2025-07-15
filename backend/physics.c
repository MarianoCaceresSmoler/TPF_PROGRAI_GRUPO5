/***************************************************************************/ /**
   @physics.h
   @Physics ant interactions
   @Grupo_5
  ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
// +Incluir el header propio (ej: #include "template.h")+

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "config.h"

#include "physics.h"
#include "entities.h"
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

// +ej: static void falta_envido (int);+

/**
 * @brief function to check a collision between two entities
 * @param entityA the first entity
 * @param entityB the second entity
 * @return 1 if there is a collision, 0 if not
 */
static int checkEntitiesCollision(entity_t entityA, entity_t entityB);

/**
 * @brief functions to check each possible collision individually
 * @param game pointer to the game information
 */
static int checkBulletHitsAliens(game_t *game); // returns the points acumulated if an alien was hit
static int checkBulletHitsMothership(game_t *game); // returns the points acumulated if the mothership was hit
static void checkBulletHitsShip(game_t *game);
static void checkBulletHitsBarriers(game_t *game);
static void checkBulletHitsBullet(game_t *game);
static void checkAllienHitsBarrier(game_t *game);
static void checkAlienHitsShip(game_t *game);
static void checkPowerUpHitsShip(game_t *game);

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

int handleCollisions(game_t *game)
{
    if(!game)
        return -1; // if game is NULL, returns an error
    
    int points = 0;

    // Check for collisions between different objects during the game
    points += checkBulletHitsAliens(game); // add points for each alien hit
    points += checkBulletHitsMothership(game); // add points if the mothership was hit
    checkBulletHitsShip(game);
    checkBulletHitsBarriers(game);
    checkBulletHitsBullet(game);
    checkAllienHitsBarrier(game);
    checkAlienHitsShip(game);
    checkPowerUpHitsShip(game);
   
    return points; // returns the points acumulated for the collisions
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static int checkEntitiesCollision(entity_t entityA, entity_t entityB)
{
    // Only adds the size offset if the entity's size is more than 1 pixel
    int hA = entityA.height <= 1? 0: entityA.height;
    int hB = entityB.height <= 1? 0: entityB.height;

    return (
        entityA.x < entityB.x + entityB.width &&
        entityA.x + entityA.width > entityB.x &&
        entityA.y <= entityB.y + hB &&
        entityA.y + hA >= entityB.y);
}

static int checkBulletHitsAliens(game_t *game)
{
    int i, j, points;

    if (game->shipBullet.entity.isAlive) // check collision only if the bullet is active
    {
        for (i = 0; i < ALIENS_ROWS; i++)
        {
            for (j = 0; j < ALIENS_COLS; j++)
            {
                if (game->aliens.alien[i][j].entity.isAlive && checkEntitiesCollision(game->aliens.alien[i][j].entity, game->shipBullet.entity)) // checks collision only if the alien is alive
                {
                    // if collision detected, kill the bullet and set the alien explosion timer
                    game->aliens.alien[i][j].entity.explosionTimer = EXPLOSION_TIMER;
                    game->aliensRemaining--;
                    game->shipBullet.entity.isAlive = 0;

                    // allows the ship to shoot
                    game->ship.canShoot = 1;

                    points = getAlienPoints(game->aliens.alien[i][j].alienType); // gets the alien's points for the impact

                    if(!points)
                    {
                        printf("Error geting alien's points"); 
                        game->status = GAME_ERROR; // sets the error status if the alien gave 0 points             
                    }

                    return points;
                }
            }
        }
    }

    return 0;
}

static void checkBulletHitsShip(game_t *game)
{
    if(game->ship.invencibilityTicks > 0)
        return; // skips if ship is in invencibility mode
    
    if (game->alienBullet.entity.isAlive && game->ship.livesLeft > 0 && game->ship.entity.isAlive) // check collision only if the entities are alive
    {
        if (checkEntitiesCollision(game->alienBullet.entity, game->ship.entity))
        {
            // if collision detected, kill the bullet and decrement ship's lives
            game->alienBullet.entity.isAlive = 0;
            game->ship.livesLeft--;
            game->ship.entity.explosionTimer = EXPLOSION_TIMER;

            // allows the aliens to shoot
            game->aliens.canShoot = 1;
        }
    }
}

static void checkBulletHitsBarriers(game_t *game)
{
    int i, j, k;
    for (i = 0; i < BARRIERS; i++)
    {
        for (j = 0; j < BARRIER_HEIGHT; j++)
        {
            for (k = 0; k < BARRIER_WIDTH; k++)
            {
                if ((game->alienBullet.entity.isAlive || game->shipBullet.entity.isAlive) && game->barriers[i].pixel[j][k].entity.isAlive) // check collision only if the entities are alive
                {
                    if (checkEntitiesCollision(game->alienBullet.entity, game->barriers[i].pixel[j][k].entity))
                    {
                        // if collision detected, kill the alien bullet and the barrier pixel
                        game->alienBullet.entity.isAlive = 0;
                        game->barriers[i].pixel[j][k].entity.isAlive = 0;

                        // allows the aliens to shoot
                        game->aliens.canShoot = 1;
                    }

                    if (checkEntitiesCollision(game->shipBullet.entity, game->barriers[i].pixel[j][k].entity))
                    {
                        // if collision detected, kill the ship bullet and the barrier pixel
                        game->shipBullet.entity.isAlive = 0;
                        game->barriers[i].pixel[j][k].entity.isAlive = 0;

                        // allows the ship to shoot
                        game->ship.canShoot = 1;
                    }
                }
            }
        }
    }
}

static int checkBulletHitsMothership(game_t *game)
{
    int mothershipPoints = 0;
   
    if (game->shipBullet.entity.isAlive && game->mothership.entity.isAlive) // check collision only if the entities are alive
    {
        if (checkEntitiesCollision(game->shipBullet.entity, game->mothership.entity))
        {
            // if collision detected, kill the bullet and set the mothership explosion timer
            game->shipBullet.entity.isAlive = 0;
            game->mothership.entity.explosionTimer = MOTHERSHIP_EXPLOSION_TIMER;
            mothershipPoints = MIN_MOTHERSHIP_POINTS + rand() % ((MAX_MOTHERSHIP_POINTS - MIN_MOTHERSHIP_POINTS) / 10 + 1) * 10; // assign a random value that is a multiple of 10 between MIN_MOTHERSHIP_POINTS and MAX_MOTHERSHIP_POINTS

            // allows the ship to shoot
            game->ship.canShoot = 1;
        }
    }
    return mothershipPoints;
}

static void checkBulletHitsBullet(game_t *game)
{
    if (game->shipBullet.entity.isAlive && game->alienBullet.entity.isAlive) // check collision only if the entities are alive
    {
        /* Checks collision between bullets using general checkEntitiesCollision and a second validation for Raspberry,
        because there bullets have 1px size, and a collision could not be detected from a frame to the next one */        
        if (checkEntitiesCollision(game->shipBullet.entity, game->alienBullet.entity) || (game->shipBullet.entity.x == game->alienBullet.entity.x && game->shipBullet.entity.y + BULLET_HEIGHT == game->alienBullet.entity.y))
        {
            // if collision detected, kill the bullets
            game->shipBullet.entity.isAlive = 0;
            game->alienBullet.entity.isAlive = 0;

            // allows the ship and aliens to shoot
            game->ship.canShoot = 1;
            game->aliens.canShoot = 1;

        }
    }
}

static void checkAllienHitsBarrier(game_t *game)
{
    int i, j, k, x, y;
    for (i = 0; i < BARRIERS; i++)
    {
        for (j = 0; j < BARRIER_HEIGHT; j++)
        {
            for (k = 0; k < BARRIER_WIDTH; k++)
            {
                for (x = 0; x < ALIENS_ROWS; x++)
                {
                    for (y = 0; y < ALIENS_COLS; y++)
                    {
                        if (game->barriers[i].pixel[j][k].entity.isAlive && game->aliens.alien[x][y].entity.isAlive) // check collision only if the entities are alive
                        {
                            if (checkEntitiesCollision(game->aliens.alien[x][y].entity, game->barriers[i].pixel[j][k].entity))
                            {
                                // if collision detected, kill the barrier pixel
                                game->barriers[i].pixel[j][k].entity.isAlive = 0;
                            }
                        }
                    }
                }
            }
        }
    }
}

static void checkAlienHitsShip(game_t *game)
{
    int i, j;

    if (game->ship.entity.isAlive) // check collision only if the ship is alive
    {
        for (i = 0; i < ALIENS_ROWS; i++)
        {
            for (j = 0; j < ALIENS_COLS; j++)
            {
                if (game->aliens.alien[i][j].entity.isAlive && checkEntitiesCollision(game->aliens.alien[i][j].entity, game->ship.entity)) // checks collision only if the alien is alive
                {
                    // if collision detected, kills the ship and ends the game
                    game->ship.entity.explosionTimer = EXPLOSION_TIMER;
                    game->ship.livesLeft = 0;
                }
            }
        }
    }
}

static void checkPowerUpHitsShip(game_t *game)
{
    int i;

    for (i = 0; i < POWERUP_TYPES; i++)
    {
        if(game->powerUp[i].entity.isAlive && game->ship.entity.isAlive) // checks collision only if the power up and the ship are alive
        {
            if(checkEntitiesCollision(game->powerUp[i].entity, game->ship.entity))
            {
                // if collision detected, kills the power up and gives the ship a power up
                game->powerUp[i].entity.isAlive = 0;
                game->activePowerUp[i] = true;
                game->powerUp[i].ticksLeft = (i == FREEZE_POWERUP? FREEZE_POWERUP_DURATION: POWERUP_USES); // if the power up is the freeze one, sets the ticks left to the freeze duration, otherwise set its to powerup uses (generally 1)
            }
        }
    }
}
