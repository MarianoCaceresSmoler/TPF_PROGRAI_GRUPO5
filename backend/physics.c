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

/**
 * @brief updates the objects explosion/live state
 * @param game pointer to the game information
 * @return the points acumulated if there was a collision with an alien
*/
static void updateExplosions(game_t *game); 

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
        updateExplosions(game);
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
                    game->aliens.alien[i][j].entity.isAlive = 0;
                    game->aliens.alien[i][j].entity.explosionTimer = 15;
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
            game->ship.entity.explosionTimer = 15;

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
            game->mothership.entity.explosionTimer = 15;

            // allows the ship to shoot
            game->ship.canShoot = 1;
        }
    }
}

static void updateExplosions(game_t *game)
{
    // Ship
    if (game->ship.entity.explosionTimer > 0) {
        game->ship.entity.explosionTimer--;
    }

    // Aliens
    for (int i = 0; i < ALIENS_ROWS; i++) {
        for (int j = 0; j < ALIENS_COLS; j++) {
            entity_t *e = &game->aliens.alien[i][j].entity;
            if (e->explosionTimer > 0) {
                e->explosionTimer--;
                if (e->explosionTimer == 0) {
                    e->isAlive = 0; // when the timer hits 0, the alien needs to be removed
                }
            }
        }
    }

    // Mothership
    if (game->mothership.entity.explosionTimer > 0) {
        game->mothership.entity.explosionTimer--;
        if (game->mothership.entity.explosionTimer == 0) {
            game->mothership.entity.isAlive = 0; // when the timer hits 0, the mothership needs to be removed
        }
    }

}
