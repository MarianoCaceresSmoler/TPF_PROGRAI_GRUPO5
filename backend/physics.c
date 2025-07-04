/***************************************************************************/ /**
   @physics.h
   @Physics ant interactions
   @Grupo_5
  ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
// +Incluir el header propio (ej: #include "template.h")+

#include "physics.h"
#include "config.h"
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
 * @param entityA, entityB the two entities to compare
 * @return 1 if there is a collision, 0 if not
 */
static int checkEntitiesCollision(entity_t entityA, entity_t entityB);

/**
 * @brief functions to check each possible collision individually
 * @param game pointer to the game information
 */
static int checkBulletHitsAliens(game_t *game);
static void checkBulletHitsShip(game_t *game);
static void checkBulletHitsBarriers(game_t *game);
static void checkBulletHitsMothership(game_t *game);
static void checkAllienHitsBarrier(game_t *game);
static void checkAlienHitsShip(game_t *game);

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
    int points = 0;

    if (game)
    {
        // Check for collisions between bullets and other objects
        points += checkBulletHitsAliens(game);
        checkBulletHitsShip(game);
        checkBulletHitsBarriers(game);
        checkBulletHitsMothership(game);
        checkAllienHitsBarrier(game);
        checkAlienHitsShip(game);
    }

    return points;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static int checkEntitiesCollision(entity_t entityA, entity_t entityB)
{
    // Check if the entities are colliding
    return (
        entityA.x <= entityB.x + entityB.width &&
        entityA.x + entityA.width >= entityB.x &&
        entityA.y <= entityB.y + entityB.height &&
        entityA.y + entityA.height >= entityB.y);
}

static int checkBulletHitsAliens(game_t *game)
{
    int i, j;

    if (game->shipBullet.entity.isAlive) // check collision only if the bullet is active
    {
        for (i = 0; i < ALIENS_ROWS; i++)
        {
            for (j = 0; j < ALIENS_COLS; j++)
            {
                if (game->aliens.alien[i][j].entity.isAlive && checkEntitiesCollision(game->aliens.alien[i][j].entity, game->shipBullet.entity))
                {
                    // if collision detected, kill the bullet and set the alien explosion timer
                    // game->aliens.alien[i][j].entity.isAlive = 0;
                    game->aliens.alien[i][j].entity.explosionTimer = EXPLOSION_TIMER;
                    game->aliensRemaining--;
                    game->shipBullet.entity.isAlive = 0;

                    // allows the ship to shoot
                    game->ship.canShoot = 1;

                    return getAlienPoints(game->aliens.alien[i][j]);
                }
            }
        }
    }

    return 0;
}

static void checkBulletHitsShip(game_t *game)
{
    if (game->alienBullet.entity.isAlive && game->ship.livesLeft > 0 && game->ship.entity.isAlive) // check collision only if the entitys are alive
    {
        if (checkEntitiesCollision(game->alienBullet.entity, game->ship.entity))
        {
            // if collision detected, kill the bullet and decrement ship's lives
            game->alienBullet.entity.isAlive = 0;
            game->ship.livesLeft -= 1;
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
                if ((game->alienBullet.entity.isAlive || game->shipBullet.entity.isAlive) && game->barriers[i].pixel[j][k].entity.isAlive) // check collision only if the entitys are alive
                {
                    if (checkEntitiesCollision(game->alienBullet.entity, game->barriers[i].pixel[j][k].entity))
                    {
                        // if collision detected, kill the bullet and the pixel
                        game->alienBullet.entity.isAlive = 0;
                        game->barriers[i].pixel[j][k].entity.isAlive = 0;

                        // allows the aliens to shoot
                        game->aliens.canShoot = 1;
                    }

                    if (checkEntitiesCollision(game->shipBullet.entity, game->barriers[i].pixel[j][k].entity))
                    {
                        // if collision detected, kill the bullet and the pixel
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

static void checkBulletHitsMothership(game_t *game)
{
    if (game->shipBullet.entity.isAlive && game->mothership.entity.isAlive) // check collision only if the entitys are alive
    {
        if (checkEntitiesCollision(game->shipBullet.entity, game->mothership.entity))
        {
            // if collision detected, kill the bullet and set the mothership explosion timer
            game->shipBullet.entity.isAlive = 0;
            game->mothership.entity.explosionTimer = EXPLOSION_TIMER;

            // allows the ship to shoot
            game->ship.canShoot = 1;
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
                        if (game->barriers[i].pixel[j][k].entity.isAlive && game->aliens.alien[x][y].entity.isAlive) // check collision only if the entitys are alive
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
                if (game->aliens.alien[i][j].entity.isAlive && checkEntitiesCollision(game->aliens.alien[i][j].entity, game->ship.entity))
                {
                    // if collision detected, kills the ship and ends the game
                    game->ship.entity.explosionTimer = EXPLOSION_TIMER;
                    game->ship.livesLeft = 0;
                }
            }
        }
    }
}
