
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

#define CHECK_IS_IN_DISPLAY(x, y) (((x) >= 0) && ((x) < DISP_CANT_X_DOTS) && ((y) > 0) && ((y) < DISP_CANT_Y_DOTS))
#define ANIMATION_DURATION (ONE_SECOND * 4)
#define ANIMATION_FRAME_DURATION (ANIMATION_DURATION / 6)

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
static void *render(void *gamearg);
static void renderLoading(game_t *game);
static void renderGame(game_t *game);
static void renderMenu(game_t *game);
static void renderGameOver(game_t *game);

/**
 * @brief draws an object in display
 * @param x position x of the object
 * @param y position y of the object
 * @param width width of the object
 * @param height height of the object
 */
static void drawObject(int x, int y, int width, int height);

static void drawStartMenu();
static void drawPauseMenu();
static void drawStartAnimation(int animationTicks);
static void drawFrame1();
static void drawFrame2();
static void drawFrame3();
static void drawFrame4();

/**
 * @brief private functions to draw the differents elements in display
 */
static void drawShip(ship_t ship);
static void drawAliens(alienFormation_t aliens);
static void drawAliensLoading(alienFormation_t aliens, int aliensToDraw);
static void drawMothership(mothership_t mothership);
static void drawBullets(bullet_t shipBullet, bullet_t alienBullet);
static void drawPowerUps(powerUp_t powerUps[POWERUP_TYPES]);
static void drawBarriers(barrier_t barriers[BARRIERS]);

/**
 * @brief private function to draw HUD during gameplay
 * @param lives the lives left
 */
static void drawHUD(int lives);

/**
 * @brief private function to draw a character
 * @param c the character to draw
 * @param x the x position
 * @param y the y position
 */
static void drawChar(char c, int x, int y);

/**
 * @brief private function to draw text
 * @param text the text to draw
 * @param x the x position
 * @param y the y position
 */
static void drawText(const char *text, int x, int y);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static int temperaturas_actuales[4];+

static pthread_t renderThread;
static bool stopRenderThread = false;

const uint8_t font3x5_letters[26][5] =
    {
        // A
        {0b010,
         0b101,
         0b111,
         0b101,
         0b101},

        // B
        {0b110,
         0b101,
         0b110,
         0b101,
         0b110},

        // C
        {0b011,
         0b100,
         0b100,
         0b100,
         0b011},

        // D
        {0b110,
         0b101,
         0b101,
         0b101,
         0b110},

        // E
        {0b111,
         0b100,
         0b110,
         0b100,
         0b111},

        // F
        {0b111,
         0b100,
         0b110,
         0b100,
         0b100},

        // G
        {0b011,
         0b100,
         0b101,
         0b101,
         0b011},

        // H
        {0b101,
         0b101,
         0b111,
         0b101,
         0b101},

        // I
        {0b111,
         0b010,
         0b010,
         0b010,
         0b111},

        // J
        {0b001,
         0b001,
         0b001,
         0b101,
         0b010},

        // K
        {0b101,
         0b101,
         0b110,
         0b101,
         0b101},

        // L
        {0b100,
         0b100,
         0b100,
         0b100,
         0b111},

        // M
        {0b101,
         0b111,
         0b111,
         0b101,
         0b101},

        // N
        {0b101,
         0b111,
         0b111,
         0b111,
         0b101},

        // O
        {0b010,
         0b101,
         0b101,
         0b101,
         0b010},

        // P
        {0b110,
         0b101,
         0b110,
         0b100,
         0b100},

        // Q
        {0b010,
         0b101,
         0b101,
         0b111,
         0b011},

        // R
        {0b110,
         0b101,
         0b110,
         0b101,
         0b101},

        // S
        {0b011,
         0b100,
         0b010,
         0b001,
         0b110},

        // T
        {0b111,
         0b010,
         0b010,
         0b010,
         0b010},

        // U
        {0b101,
         0b101,
         0b101,
         0b101,
         0b111},

        // V
        {0b101,
         0b101,
         0b101,
         0b101,
         0b010},

        // W
        {0b101,
         0b101,
         0b111,
         0b111,
         0b101},

        // X
        {0b101,
         0b101,
         0b010,
         0b101,
         0b101},

        // Y
        {0b101,
         0b101,
         0b010,
         0b010,
         0b010},

        // Z
        {0b111,
         0b001,
         0b010,
         0b100,
         0b111}};

const uint8_t font3x5_digits[12][5] =
    {
        // 0
        {0b111,
         0b101,
         0b101,
         0b101,
         0b111},

        // 1
        {0b010,
         0b110,
         0b010,
         0b010,
         0b111},

        // 2
        {0b111,
         0b001,
         0b111,
         0b100,
         0b111},

        // 3
        {0b111,
         0b001,
         0b111,
         0b001,
         0b111},

        // 4
        {0b101,
         0b101,
         0b111,
         0b001,
         0b001},

        // 5
        {0b111,
         0b100,
         0b111,
         0b001,
         0b111},

        // 6
        {0b111,
         0b100,
         0b111,
         0b101,
         0b111},

        // 7
        {0b111,
         0b001,
         0b010,
         0b010,
         0b010},

        // 8
        {0b111,
         0b101,
         0b111,
         0b101,
         0b111},

        // 9
        {0b111,
         0b101,
         0b111,
         0b001,
         0b111},

        // :
        {0b000,
         0b010,
         0b000,
         0b010,
         0b000},

        // space
        {0b000,
         0b000,
         0b000,
         0b000,
         0b000}};


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void initGraphics(game_t *game)
{
    disp_init();

    pthread_create(&renderThread, NULL, render, game); // creates thread for rendering

    disp_clear();
}

void cleanupGraphics(void)
{
    stopRenderThread = true;
    pthread_join(renderThread, NULL);
    disp_clear();
    disp_update();
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void *render(void *gamearg)
{
    game_t *game = (game_t *)gamearg;

    while (!stopRenderThread)
    {
        disp_clear(); // first clears all the leds

        if (game->status == GAME_LOADING)
            renderLoading(game);
        else if (game->status == GAME_RUNNING)
            renderGame(game);
        else if (game->status == GAME_MENU || game->status == GAME_PAUSED)
            renderMenu(game);
        else if (game->status == GAME_END)
            renderGameOver(game);
        else
            cleanupGraphics();

        disp_update(); // updates the display

        SDL_Delay(1000 / FPS); // 30 FPS --> 33 ms delay
    }

    return NULL;
}

static void renderLoading(game_t *game)
{
    drawAliensLoading(game->aliens, LOADING_TIME - game->loadingTimer);
}

static void renderGame(game_t *game)
{
    drawShip(game->ship);
    drawAliens(game->aliens);
    drawMothership(game->mothership);
    drawBullets(game->shipBullet, game->alienBullet);
    drawBarriers(game->barriers);
    drawPowerUps(game->powerUp);
    drawHUD(game->ship.livesLeft);
}

static void renderMenu(game_t *game)
{
    if (game->status == GAME_MENU)
    {
        static int animationTicks = ANIMATION_DURATION;
        if (animationTicks > 0)
        {
            drawStartAnimation(animationTicks);
            animationTicks--;
        }
        else
            drawStartMenu();
    }
    else if (game->status == GAME_PAUSED)
    {
        drawPauseMenu();
    }
}

static void renderGameOver(game_t *game)
{
    if (game->status == GAME_END)
    {
        int finalScore = game->score;
        int finalLevel = game->currentLevel;

        // Convertir a string
        char scoreStr[6];
        char levelStr[6];

        sprintf(scoreStr, "S:%d", finalScore);
        sprintf(levelStr, "L:%d", finalLevel);

        // Mostrar score arriba, level abajo
        drawText(scoreStr, 0, 2); // Y=2 deja margen arriba
        drawText(levelStr, 0, 9); // Y=9 para segunda línea
    }
}

static void drawObject(int x, int y, int width, int height)
{
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            int px = x + col;
            int py = y + row;

            if (!CHECK_IS_IN_DISPLAY(px, py))
                continue;

            dcoord_t coord = {.x = px, .y = py};
            disp_write(coord, D_ON);
        }
    }
}

static void drawStartAnimation(int animationTicks)
{
    if (animationTicks == 0)
        return;

    int frames = animationTicks % ONE_SECOND;

    if (frames < ONE_SECOND / 4)
        drawFrame1();
    else if (frames < ONE_SECOND / 2)
        drawFrame2();
    else if (frames < ONE_SECOND * 3 / 4)
        drawFrame3();
    else
        drawFrame4();
}

static void drawFrame1()
{
    char matrix[SCREEN_HEIGHT][SCREEN_WIDTH] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0},
        {0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };
    int i, j;

    for (i = 0; i < SCREEN_HEIGHT; i++)
    {
        for (j = 0; j < SCREEN_WIDTH; j++)
        {
            dcoord_t coord = {.x = j, .y = i};
            disp_write(coord, matrix[i][j] ? D_ON : D_OFF);
        }
    }
}

static void drawFrame2()
{
    char matrix[SCREEN_HEIGHT][SCREEN_WIDTH] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0},
        {0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    int i, j;

    for (i = 0; i < SCREEN_HEIGHT; i++)
    {
        for (j = 0; j < SCREEN_WIDTH; j++)
        {
            dcoord_t coord = {.x = j, .y = i};
            disp_write(coord, matrix[i][j] ? D_ON : D_OFF);
        }
    }
}

static void drawFrame3()
{
    char matrix[SCREEN_HEIGHT][SCREEN_WIDTH] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    int i, j;

    for (i = 0; i < SCREEN_HEIGHT; i++)
    {
        for (j = 0; j < SCREEN_WIDTH; j++)
        {
            dcoord_t coord = {.x = j, .y = i};
            disp_write(coord, matrix[i][j] ? D_ON : D_OFF);
        }
    }
}

static void drawFrame4()
{
    char matrix[SCREEN_HEIGHT][SCREEN_WIDTH] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };
    int i, j;

    for (i = 0; i < SCREEN_HEIGHT; i++)
    {
        for (j = 0; j < SCREEN_WIDTH; j++)
        {
            dcoord_t coord = {.x = j, .y = i};
            disp_write(coord, matrix[i][j] ? D_ON : D_OFF);
        }
    }
}

static void drawStartMenu()
{
}

static void drawPauseMenu()
{
}

static void drawShip(ship_t ship)
{
    if (ship.entity.isAlive)
    {
        int x = ship.entity.x;
        int y = ship.entity.y;

        if (ship.invencibilityTicks % 2 == 0) // ticks mode if ship is invincible after being shooted
            drawObject(x, y, SHIP_WIDTH, SHIP_HEIGHT);
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

static void drawPowerUps(powerUp_t powerUps[POWERUP_TYPES])
{
    int i, x, y;

    for (i = 0; i < POWERUP_TYPES; i++)
    {
        if (powerUps[i].entity.isAlive) // only displays the powerup if it is alive
        {
            // Position of the powerup
            x = powerUps[i].entity.x;
            y = powerUps[i].entity.y;

            drawObject(x, y, POWERUP_WIDTH, POWERUP_HEIGHT); // draws the powerup            
        }
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
                    if (CHECK_IS_IN_DISPLAY(pixel.x, pixel.y))
                    {
                        dcoord_t coord = {.x = pixel.x, .y = pixel.y};
                        disp_write(coord, D_ON);
                    }
                }
            }
        }
    }
}

static void drawHUD(int lives)
{
    // Draw lives as points in the upper left part of the screen
    int i;

    for (i = 0; i < lives; i++)
    {
        dcoord_t coord = {.x = i * 2, .y = 0}; // *2 for separation
        disp_write(coord, D_ON);
    }
}

static void drawChar(char c, int x, int y)
{

    const uint8_t *matrixChar = NULL;
    int col, row;

    if (c >= 'A' && c <= 'Z')
    {
        matrixChar = font3x5_letters[c - 'A'];
    }
    else if (c >= '0' && c <= '9')
    {
        matrixChar = font3x5_digits[c - '0'];
    }
    else if (c == ':')
    {
        matrixChar = font3x5_digits[10];
    }
    else if (c == ' ')
    {
        matrixChar = font3x5_digits[11];
    }
    else
    {
        return; // not a printable character
    }

    // Draws the pixels of the character
    for (row = 0; row < 5; row++)
    {
        for (col = 0; col < 3; col++)
        {
            if (matrixChar[row] & (1 << (2 - col)))
            {
                dcoord_t coord = {.x = x + col, .y = y + row};
                disp_write(coord, D_ON);
            }
        }
    }
}

static void drawText(const char *text, int x, int y)
{
    while (*text)
    {
        drawChar(*text, x, y);
        x += 4; // 3 width + 1 space
        text++;
    }
}
