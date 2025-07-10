
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
static void *render(void *arg);
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
 */
void drawObject(int x, int y, int width, int height);

/**
 * @brief private functions to draw the differents elements in display
 */
static void drawShip(ship_t ship);
static void drawAliens(alienFormation_t aliens);
static void drawAliensLoading(alienFormation_t aliens, int aliensToDraw);
static void drawMothership(mothership_t mothership);
static void drawBullets(bullet_t shipBullet, bullet_t alienBullet);
static void drawBarriers(barrier_t barriers[BARRIERS]);

/**
 * @brief private function to draw HUD during gameplay
 * @param score the actual score
 * @param lives the lives left
 * @param level the actual level
 */
static void drawHUD(int score, int lives, int level);

/**
 * @brief private function to draw a character
 * @param c the character
 * @param x position in x
 * @param y position in y
 */
void drawChar(char c, int x, int y);

/**
 * @brief private function to draw text
 * @param text the text to draw
 * @param x position in x
 * @param y position in y
 */
void drawText(const char *text, int x, int y);

/**
 * @brief private function to draw blinking text
 * @param text the text to draw
 * @param x position in x
 * @param y position in y
 * @param times the number of times the text will blink
 * @param delay the delay between each blink
 */
void blinkText(const char *text, int x, int y, int times, int delay);

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

const unsigned char font5x5[26][5] = {
    // A
    {0b01110,
     0b10001,
     0b11111,
     0b10001,
     0b10001},

    // B
    {0b11110,
     0b10001,
     0b11110,
     0b10001,
     0b11110},

    // C
    {0b01110,
     0b10001,
     0b10000,
     0b10001,
     0b01110},

    // D
    {0b11110,
     0b10001,
     0b10001,
     0b10001,
     0b11110},

    // E
    {0b11111,
     0b10000,
     0b11110,
     0b10000,
     0b11111},

    // F
    {0b11111,
     0b10000,
     0b11110,
     0b10000,
     0b10000},

    // G
    {0b01110,
     0b10000,
     0b10111,
     0b10001,
     0b01110},

    // H
    {0b10001,
     0b10001,
     0b11111,
     0b10001,
     0b10001},

    // I
    {0b01110,
     0b00100,
     0b00100,
     0b00100,
     0b01110},

    // J
    {0b00001,
     0b00001,
     0b00001,
     0b10001,
     0b01110},

    // K
    {0b10001,
     0b10010,
     0b11100,
     0b10010,
     0b10001},

    // L
    {0b10000,
     0b10000,
     0b10000,
     0b10000,
     0b11111},

    // M
    {0b10001,
     0b11011,
     0b10101,
     0b10001,
     0b10001},

    // N
    {0b10001,
     0b11001,
     0b10101,
     0b10011,
     0b10001},

    // O
    {0b01110,
     0b10001,
     0b10001,
     0b10001,
     0b01110},

    // P
    {0b11110,
     0b10001,
     0b11110,
     0b10000,
     0b10000},

    // Q
    {0b01110,
     0b10001,
     0b10001,
     0b10011,
     0b01111},

    // R
    {0b11110,
     0b10001,
     0b11110,
     0b10010,
     0b10001},

    // S
    {0b01111,
     0b10000,
     0b01110,
     0b00001,
     0b11110},

    // T
    {0b11111,
     0b00100,
     0b00100,
     0b00100,
     0b00100},

    // U
    {0b10001,
     0b10001,
     0b10001,
     0b10001,
     0b01110},

    // V
    {0b10001,
     0b10001,
     0b10001,
     0b01010,
     0b00100},

    // W
    {0b10001,
     0b10001,
     0b10101,
     0b11011,
     0b10001},

    // X
    {0b10001,
     0b01010,
     0b00100,
     0b01010,
     0b10001},

    // Y
    {0b10001,
     0b01010,
     0b00100,
     0b00100,
     0b00100},

    // Z
    {0b11111,
     0b00010,
     0b00100,
     0b01000,
     0b11111}};

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

static void *render(void *arg)
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

    return NULL;

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
    drawHUD(globalGameStatus->score, globalGameStatus->ship.livesLeft, globalGameStatus->currentLevel);
}

static void renderMenu(void)
{
    if (globalGameStatus->status == GAME_MENU)
    {
        blinkText("SPACE", 0, 0, 3, 500);
        blinkText("INVADERS", 0, 8, 3, 500);
        blinkText("HOLD BUTTON TO PLAY", 0, 8, 3, 500);
    }
    else if (globalGameStatus->status == GAME_PAUSED)
    {
        blinkText("DOWN + BUTTON TO EXIT", 0, 0, 3, 500);
        blinkText("UP + BUTTON TO RESTART", 0, 8, 3, 500);
    }
}

static void renderGameOver(void)
{
    blinkText("SPACE", 0, 0, 3, 500);
    blinkText("INVADERS", 0, 8, 3, 500);
    blinkText("DOWN + BUTTON TO EXIT", 0, 0, 3, 500);
    blinkText("UP + BUTTON TO RESTART", 0, 8, 3, 500);
}

void drawObject(int x, int y, int width, int height)
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
            disp_write(coord, D_ON);
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
            drawObject(x, y, SHIP_WIDTH, SHIP_HEIGHT);
        }
    }
}

static void drawAliens(alienFormation_t aliens)
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

            drawObject(x, y, ALIEN_WIDTH, ALIEN_HEIGHT);
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

            drawObject(alien.entity.x, alien.entity.y, ALIEN_WIDTH, ALIEN_HEIGHT);

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

        drawObject(x, y, MOTHERSHIP_WIDTH, MOTHERSHIP_HEIGHT);
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
        drawObject(x, y, BULLET_WIDTH, BULLET_HEIGHT);
    }

    // Alien bullet
    // Only draws if the alien bullet is alive
    if (alienBullet.entity.isAlive)
    {
        x = alienBullet.entity.x;
        y = alienBullet.entity.y;
        drawObject(x, y, BULLET_WIDTH, BULLET_HEIGHT);
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

static void drawHUD(int score, int lives, int level)
{
    // HACER
}

void drawChar(char c, int x, int y)
{
    if (c < 'A' || c > 'Z')
        return; // Solo letras mayúsculas

    int index = c - 'A'; // 'A' está en font5x5[0]
    for (int row = 0; row < 5; row++)
    {
        uint8_t line = font5x5[index][row];
        for (int col = 0; col < 5; col++)
        {
            if (line & (1 << (4 - col))) // Bit activo
            {
                drawObject(x + col, y + row, 1, 1); // Un pixel
            }
        }
    }
}

void drawText(const char *text, int x, int y)
{
    int spacing = 6; // 5 de letra + 1 espacio
    for (int i = 0; text[i] != '\0'; i++)
    {
        drawChar(text[i], x + i * spacing, y);
    }
}

void blinkText(const char *text, int x, int y, int times, int delay)
{
    for (int i = 0; i < times; i++)
    {
        drawText(text, x, y);
        SDL_Delay(delay);
        disp_clear();
        SDL_Delay(delay);
    }
}
