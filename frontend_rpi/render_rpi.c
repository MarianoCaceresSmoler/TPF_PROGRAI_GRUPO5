
/***************************************************************************/ /**
   @render_rpi.c
   @Render frontend
   @Grupo_5
  ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
// +Incluir el header propio (ej: #include "template.h")+

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include <SDL2/SDL.h> // to use delays
#include "../backend/config.h"
#include "../backend/game.h"
#include "../backend/entities.h"
#include "render_rpi.h"
#include "disdrv.h"

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
 * @brief private functions to render the different parts of the game
 */
static void *render(void);
static void renderLoading(void);
static void renderGame(void);
static void renderMenu(void);
static void renderGameOver(void);

/**
 * @brief draws an object in display
 * @param x position x of the object
 * @param y position y of the object
 * @param width width of the object
 * @param height height of the object
 * @param state state of the LEDs: D_ON or D_OFF
 */
void drawObject(int x, int y, int width, int height, dlevel_t state);

/**
 * @brief private functions to draw the differents elements in display
 */
static void drawShip(ship_t ship);
static void drawAliens(alienFormation_t alienFormation);
static void drawAliensLoading(alienFormation_t aliens, int aliensToDraw);
static void drawMothership(mothership_t mothership);
static void drawBullets(bullet_t shipBullet, bullet_t alienBullet);
static void drawBarriers(barrier_t barriers[BARRIERS]);
static void drawPowerUps(powerUp_t powerUp[POWERUP_TYPES], int activePowerUp[POWERUP_TYPES]);

/**
 * @brief private function to draw HUD during gameplay
 * @param score the actual score
 * @param lives the lives left
 * @param level the actual level
 */
static void drawHUD(int score, int lives, int level);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static int temperaturas_actuales[4];+

static bool stopRenderThread = false;
static game_t *globalGameStatus = NULL;
static pthread_t renderThread;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void initGraphics(game_t *game)
{
    disp_init();

    globalGameStatus = game; // to get the game status frame to frame

    pthread_create(&renderThread, NULL, render, NULL); // creates thread for rendering

    disp_clear();
}

void cleanupGraphics(void)
{
    stopRenderThread = true;
    disp_clear();
    disp_update();
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void *render(void)
{
    while (!stopRenderThread)
    {
        disp_clear(); // first clears all the leds

        if (globalGameStatus->status == GAME_LOADING)
            renderLoading();
        else if (globalGameStatus->status == GAME_RUNNING)
            renderGame();
        else if (globalGameStatus->status == GAME_MENU || globalGameStatus->status == GAME_PAUSED)
            renderMenu();
        else if (globalGameStatus->status == GAME_END)
            renderGameOver();
        else
            cleanupGraphics();

        disp_update(); // updates the display

        SDL_Delay(1000 / FPS); // 30 FPS --> 33 ms delay
    }
}

static void renderLoading(void)
{
    drawAliensLoading(globalGameStatus->aliens, LOADING_TIME - globalGameStatus->loadingTimer);
}

static void renderGame(void)
{
    drawShip(globalGameStatus->ship);
    drawAliens(globalGameStatus->aliens);
    drawMothership(globalGameStatus->mothership);
    drawBullets(globalGameStatus->shipBullet, globalGameStatus->alienBullet);
    drawBarriers(globalGameStatus->barriers);
    drawPowerUps(globalGameStatus.powerUp, globalGameStatus.activePowerUp);
    drawHUD(globalGameStatus->score, globalGameStatus->ship.livesLeft, globalGameStatus->currentLevel);
}

static void renderMenu(void)
{
    // Usamos parpadeo para simular texto
    static unsigned int blinkTimer = 0;
    blinkTimer++;

    if (globalGameStatus->status == GAME_MENU)
    {
    }
    else if (globalGameStatus->status == GAME_PAUSED)
    {
        // Mensaje "GAME PAUSED"

        if (blinkTimer % 3 == 0)
        {
            // Dibuja letras grandes (simulando "GAME")
            drawObject(2, 2, 4, 1, D_ON);  // G
            drawObject(9, 3, 4, 2, D_ON);  // A
            drawObject(15, 3, 4, 2, D_ON); // M
            drawObject(21, 3, 4, 2, D_ON); // E

            // Dibuja letras grandes (simulando "PAUSED")
            drawObject(3, 6, 4, 2, D_ON);  // P
            drawObject(9, 6, 4, 2, D_ON);  // A
            drawObject(15, 6, 4, 2, D_ON); // U
            drawObject(21, 6, 4, 2, D_ON); // S
            drawObject(27, 6, 4, 2, D_ON); // E
            drawObject(33, 6, 4, 2, D_ON); // D
        }
    }

    // Nave del jugador (centrada abajo)
    int shipX = (DISP_CANT_X_DOTS - SHIP_WIDTH) / 2;
    int shipY = DISP_CANT_Y_DOTS - SHIP_HEIGHT - 1;
    drawObject(shipX, shipY, SHIP_WIDTH, SHIP_HEIGHT, D_ON);
}

static void renderGameOver(void)
{
    static uint8_t gameOverBlink = 0;
    int x = (DISP_CANT_X_DOTS - SHIP_WIDTH) / 2;
    int y = DISP_CANT_Y_DOTS - SHIP_HEIGHT - 1;

    // Nave explotada: parpadeo
    if ((gameOverBlink++ / 5) % 2 == 0)
    {
        drawObject(x, y, SHIP_WIDTH, SHIP_HEIGHT, D_ON);
    }
    else
    {
        drawObject(x, y, SHIP_WIDTH, SHIP_HEIGHT, D_OFF);
    }

    if ((gameOverBlink / 10) % 2 == 0)
    {
        drawObject(0, 0, 1, 1, D_ON);
        drawObject(DISP_CANT_X_DOTS - 1, 0, 1, 1, D_ON);
        drawObject(0, DISP_CANT_Y_DOTS - 1, 1, 1, D_ON);
        drawObject(DISP_CANT_X_DOTS - 1, DISP_CANT_Y_DOTS - 1, 1, 1, D_ON);
    }
}

void drawObject(int x, int y, int width, int height, dlevel_t state)
{
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            int px = x + col;
            int py = y + row;

            if (px >= DISP_CANT_X_DOTS || py >= DISP_CANT_Y_DOTS)
                continue;

            dcoord_t coord = {.x = px, .y = py};
            disp_write(coord, state);
        }
    }
}

static void drawShip(ship_t ship)
{
    if (ship.entity.isAlive)
    {
        int x = ship.entity.x;
        int y = ship.entity.y;

        if (ship.invencibilityTicks % 2 == 0) // ticks mode if ship is invincible after being shooted
        {
            drawObject(x, y, SHIP_WIDTH, SHIP_HEIGHT, D_ON);
        }
    }
}

static void drawAliens(alienFormation_t alienFormation)
{
    for (int i = 0; i < ALIENS_ROWS; i++)
    {
        for (int j = 0; j < ALIENS_COLS; j++)
        {
            alien_t *alien = &aliens.alien[i][j];

            if (!alien->entity.isAlive || alien->entity.explosionTimer > 0) // skip dead aliens
                continue;

            int x = alien->entity.x;
            int y = alien->entity.y;

            drawObject(x, y, ALIEN_WIDTH, ALIEN_HEIGHT, D_ON);
        }
    }
}

static void drawAliensLoading(alienFormation_t aliens, int aliensToDraw)
{
    int drawn = 0;

    // draws aliens from bottom to top, from left to right
    for (int i = ALIENS_ROWS - 1; i >= 0; i--)
    {
        for (int j = 0; j < ALIENS_COLS; j++)
        {
            if (drawn >= aliensToDraw)
                return; // when we have drawn the aliens we wanted

            alien_t alien = aliens.alien[i][j];

            drawObject(alien.entity.x, alien.entity.y, ALIEN_WIDTH, ALIEN_HEIGHT, D_ON);

            drawn++;
        }
    }
}

static void drawMothership(mothership_t mothership)
{
    if (mothership.entity.isAlive && mothership.entity.explosionTimer == 0) // skips if mothership is dead or has exploded
    {
        int x = mothership.entity.x;
        int y = mothership.entity.y;

        drawObject(x, y, MOTHERSHIP_WIDTH, MOTHERSHIP_HEIGHT, D_ON);
    }
}
static void drawBullets(bullet_t shipBullet, bullet_t alienBullet)
{
    int x, y;

    // Ship bullet
    // Only draws if the ship bullet is alive
    if (shipBullet.entity.isAlive)
    {
        x = shipBullet.entity.x;
        y = shipBullet.entity.y;
        drawObject(x, y, BULLET_WIDTH, BULLET_HEIGHT, D_ON);
    }

    // Alien bullet
    // Only draws if the alien bullet is alive
    if (alienBullet.entity.isAlive)
    {
        x = alienBullet.entity.x;
        y = alienBullet.entity.y;
        drawObject(x, y, BULLET_WIDTH, BULLET_HEIGHT, D_ON);
    }
}

static void drawBarriers(barrier_t barriers[BARRIERS])
{
    int i, x, y;

    for (i = 0; i < BARRIERS; i++)
    {
        for (x = 0; x < BARRIER_HEIGHT; x++)
        {
            for (y = 0; y < BARRIER_WIDTH; y++)
            {
                entity_t pixel = barriers[i].pixel[x][y].entity;

                // Only draws if the pixel is alive
                if (pixel.isAlive)
                {
                    dcoord_t coord = {.x = pixel.x, .y = pixel.y};
                    disp_write(coord, D_ON);
                }
            }
        }
    }
}

static void drawPowerUps(powerUp_t powerUp[POWERUP_TYPES], int activePowerUp[POWERUP_TYPES]);