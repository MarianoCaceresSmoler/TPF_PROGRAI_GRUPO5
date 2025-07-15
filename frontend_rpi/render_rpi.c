/***************************************************************************//**
  @file 	render_rpi.c
  @brief    Render functions for the matrix display
  @author 	Grupo_5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
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

// Macro to check if an element in (x,y) position enters in the display
#define CHECK_IS_IN_DISPLAY(x, y) (((x) >= 0) && ((x) < DISP_CANT_X_DOTS) && ((y) > 0) && ((y) < DISP_CANT_Y_DOTS))

// Constants to draw text on display
#define CHAR_WIDTH 3
#define CHAR_SEPARATION 1
#define CHAR_WIDTH_PIXELS (CHAR_WIDTH + CHAR_SEPARATION)
#define CHAR_HEIGHT 5

// Durations for animations
#define ANIMATION_DURATION (ONE_SECOND * 4)
#define SWIPING_TEXT_DELAY 100

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/**
 * @brief function to render game in a separate thread
 * @param gamearg pointer to game object used in main
*/
static void *render(void *gamearg);

/**
 * @brief function to render loading screen
 * @param game pointer to the game object created in main
 */
static void renderLoading(game_t *game);

/**
 * @brief function to render gameplay screen
 * @param game pointer to the game object created in main
 */
static void renderGame(game_t *game);

/**
 * @brief function to render menu screen
 * @param game pointer to the game object created in main
 */
static void renderMenu(game_t *game);

/**
 * @brief function to render game over screen
 * @param game pointer to the game object created in main
 */
static void renderGameOver(game_t *game);

/**
 * @brief draws a rectangular object in display
 * @param x position x of the object
 * @param y position y of the object
 * @param width width of the object
 * @param height height of the object
 */
static void drawObject(int x, int y, int width, int height);

/**
 * @brief function to draw start menu
 * @param gameStatus pointer to real time game status
 */
static void drawStartMenu(gameStatus_t *gameStatus);

/**
 * @brief function to draw pause menu
 * @param gameStatus pointer to real time game status
 */
static void drawPauseMenu(gameStatus_t *gameStatus);

// Functions to draw initial menu animation 
// param animationTicks is used to determine which frame to draw

/**
 * @brief function to draw start animation
 * @param animationTicks ticks of animation left to play
 */
static void drawStartAnimation(int animationTicks);

/**
 * @brief function to draw start animation frame 1
 */
static void drawFrame1(void);

/**
 * @brief function to draw start animation frame 2
 */
static void drawFrame2(void);

/**
 * @brief function to draw start animation frame 3
 */
static void drawFrame3(void);

/**
 * @brief function to draw start animation frame 4
 */
static void drawFrame4(void);

/**
 * @brief function to draw ship on display
 * @param ship copy of ship structure
 */
static void drawShip(ship_t ship);

/**
 * @brief function to draw aliens on display
 * @param aliens copy of alienformation structure
 */
static void drawAliens(alienFormation_t aliens);

/**
 * @brief function to draw loading aliens on display
 * @param aliens copy of alienformation structure
 * @param aliensToDraw total number of aliens to draw
 */
static void drawAliensLoading(alienFormation_t aliens, int aliensToDraw);

/**
 * @brief function to draw mothership on display
 * @param mothership copy of mothership structure
 */
static void drawMothership(mothership_t mothership);

/**
 * @brief function to draw bullets on display
 * @param shipBullet copy of ship bullet structure
 * @param alienBullet copy of alien bullet structure
 */
static void drawBullets(bullet_t shipBullet, bullet_t alienBullet);

/**
 * @brief function to draw power ups on display
 * @param powerUps array with every power up on the game
 */
static void drawPowerUps(powerUp_t powerUps[POWERUP_TYPES]);

/**
 * @brief function to draw barriers on display
 * @param barriers array with the barriers to draw
 */
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

/**
 * @brief draws text scrolling on display
 * @param x initial position x of the text
 * @param y initial position y of the text
 * @param width width of the object
 * @param gameStatus pointer to real time game status
 */
static void swipeText(const char * text, int x, int y, gameStatus_t *gameStatus);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// Variables to control the input thread
static pthread_t renderThread;
static bool stopRenderThread = false;

// Matrix to get formatted letters
const uint8_t font3x5_letters[26][CHAR_HEIGHT] =
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

// Matrix to get formatted digits
const uint8_t font3x5_digits[12][CHAR_HEIGHT] =
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

int initGraphics(game_t *game)
{
    if(!game)
    {
        printf("Error: game param is NULL");
        return -1; // return an error code the game object received is NULL
    }

    // Initialize the display
    disp_init(); 
    
    // Creates thread for rendering
    if(pthread_create(&renderThread, NULL, render, game))
    {
        printf("Error creating render thread\n");
        return -1; // return an error code if the thread creation fails
    }

    disp_clear();
    return 0;
}

void cleanupGraphics(void) // to stop the loop in render
{
    stopRenderThread = true; // to stop the loop in render
    pthread_join(renderThread, NULL); // waits for the thread to finish
    
    // Clears display and updates
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
    // Cast the generic pointer to the proper type
    game_t *game = (game_t *)gamearg;

    while (!stopRenderThread)
    {
        disp_clear(); // first clears all the leds

        // Then renders depending on game state
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
    drawAliensLoading(game->aliens, LOADING_TIME - game->loadingTimer); // draws aliens loading
}

static void renderGame(game_t *game)
{
    // Draw each element of the game
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
    // If the game is currently in the main menu state
    if (game->status == GAME_MENU)
    {
        // Static variable to control the duration of the start animation
        static int animationTicks = ANIMATION_DURATION;

        // If there are still animation ticks left, play the start animation
        if (animationTicks > 0)
        {
            drawStartAnimation(animationTicks); // Draw the animation frame
            animationTicks--; // Decrease the animation timer
        }
        else
        {
            // Once the animation finishes, draw the main menu
            drawStartMenu(&game->status);
        }
    }
    // If the game is paused, show the pause menu
    else if (game->status == GAME_PAUSED)
    {
        drawPauseMenu(&game->status);
    }
}

static void renderGameOver(game_t *game)
{
    if (game->status == GAME_END)
    {
        // Get final score and level
        int finalScore = game->score;
        int finalScoreRank = game->scoreRank;
        int finalLevel = game->currentLevel;

        disp_clear(); // clears display

        // Draws GAME OVER
        int i;
        for (i = ANIMATION_DURATION; i > 0; i--)
        {
            disp_clear();
            if (i % 20 >= 10)
            {
                drawText("GAME", 1, 2);
                drawText("OVER", 1, 8);
            }
            
            disp_update();
            SDL_Delay(10);
        }
        char finalStr[24];

        // if score is on top ten, show it
        if(finalScoreRank)
        {
            disp_clear();
            char scoreStr[2];
            drawText("TOP", 1, 2);
            sprintf(scoreStr, "%d", finalScoreRank);
            drawText(scoreStr, 1, 8);
            disp_update();
            SDL_Delay(1000);
        }
        // Draws final score and level
        sprintf(finalStr, "SCORE:%d  LEVEL:%d", finalScore, finalLevel); // converts to string
        swipeText(finalStr, SCREEN_WIDTH, 5, &game->status); // draws text scrolling
    }
}

static void drawObject(int x, int y, int width, int height)
{
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        { 
            // Draws each pixel of the object individually
            int px = x + col;
            int py = y + row;

            if (!CHECK_IS_IN_DISPLAY(px, py)) // only draws if the pixel fits in the display
                continue;

            dcoord_t coord = {.x = px, .y = py};
            disp_write(coord, D_ON);
        }
    }
}

static void drawStartAnimation(int animationTicks)
{
    // Draws an animation of 4 frames of an alien moving

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

static void drawFrame1(void)
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

static void drawFrame2(void)
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

static void drawFrame3(void)
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

static void drawFrame4(void)
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

static void drawStartMenu(gameStatus_t *gameStatus)
{
    swipeText("HOLD BUTTON TO PLAY", SCREEN_WIDTH, 5, gameStatus); // swiping text in start menu after the animation is over
}

static void drawPauseMenu(gameStatus_t *gameStatus)
{

    // Shows a wheel to restart, and an X to close
    char matrix[SCREEN_HEIGHT][SCREEN_WIDTH] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
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

static void drawShip(ship_t ship)
{
    if (ship.entity.isAlive)
    {
        int x = ship.entity.x;
        int y = ship.entity.y;

        if (ship.invencibilityTicks % 2 == 0) // ticks mode if ship is invincible after being shooted
            drawObject(x, y, SHIP_WIDTH, SHIP_HEIGHT); // draws ship in display
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

            drawObject(x, y, ALIEN_WIDTH, ALIEN_HEIGHT); // draws each alien individually
        }
    }
}

static void drawAliensLoading(alienFormation_t aliens, int aliensToDraw)
{
    int drawn = 0; // to store the aliens drawn in one frame

    // draws aliens from bottom to top, from left to right
    for (int i = ALIENS_ROWS - 1; i >= 0; i--)
    {
        for (int j = 0; j < ALIENS_COLS; j++)
        {
            if (drawn >= aliensToDraw)
                return; // when we have drawn the aliens we wanted (per frame)

            alien_t alien = aliens.alien[i][j];

            drawObject(alien.entity.x, alien.entity.y, ALIEN_WIDTH, ALIEN_HEIGHT); // draws each alien individually
            drawn++; // increment the counter
        }
    }
}

static void drawMothership(mothership_t mothership)
{
    if (mothership.entity.isAlive && mothership.entity.explosionTimer == 0) // skips if mothership is dead or has exploded
    {
        int x = mothership.entity.x;
        int y = mothership.entity.y;

        drawObject(x, y, MOTHERSHIP_WIDTH, MOTHERSHIP_HEIGHT); // draws the mothership
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

    // Draws each barrier pixel by pixel
    for (i = 0; i < BARRIERS; i++)
    {
        for (x = 0; x < BARRIER_HEIGHT; x++)
        {
            for (y = 0; y < BARRIER_WIDTH; y++)
            {
                entity_t pixel = barriers[i].pixel[x][y].entity;

                // Only draws the pixel if it is alive
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

    // Determine the bitmap to use based on the character
    if (c >= 'A' && c <= 'Z')
    {
        // Uppercase letters: get the corresponding 3x5 font matrix
        matrixChar = font3x5_letters[c - 'A'];
    }
    else if (c >= '0' && c <= '9')
    {
        // Digits: get the corresponding 3x5 font matrix
        matrixChar = font3x5_digits[c - '0'];
    }
    else if (c == ':')
    {
        // Colon character uses the 11th index in digits font
        matrixChar = font3x5_digits[10];
    }
    else if (c == ' ')
    {
        // Space character uses the 12th index in digits font (blank matrix)
        matrixChar = font3x5_digits[11];
    }
    else
    {
        // Unsupported or non-printable character: do nothing
        return; 
    }

    // Loop through the 5x3 char
    for (row = 0; row < CHAR_HEIGHT; row++)
    {
        for (col = 0; col < CHAR_WIDTH; col++)
        {
            // Check if the bit corresponding to this pixel is set
            // and ensure the pixel is within the screen bounds
            if (matrixChar[row] & (1 << (CHAR_WIDTH - 1 - col)) && x+col >= 0 && x+col < SCREEN_WIDTH && y+row > 0 && y+row < SCREEN_HEIGHT)
            {
                // Set the pixel on the display at the correct position
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
        // Draw each character at the current position
        drawChar(*text, x, y);

        // Move the x position to the right for the next character
        // 3 pixels for character width + 1 pixel space = 4 pixels
        x += 4;

        // Move to the next character in the string
        text++;
    }
}

static void swipeText(const char * text, int x, int y, gameStatus_t *gameStatus)
{
    int i, j;
    int length = strlen(text);          // Get the length of the text
    gameStatus_t lastStatus = *gameStatus;  // Store the current game status at the start

    // Animate from position x to x - text length (scrolling to the left)
    for (i = x, j = 0; 
         i > x - length * CHAR_WIDTH_PIXELS && lastStatus == *gameStatus; 
         i--, j++)
    {
        disp_clear(); // Clear the screen

        // Draw the text at the current position
        // Add a small vertical offset using j % 2 to give a bouncing effect
        drawText(text, i, y + j % 2);

        disp_update(); // Refresh the display

        SDL_Delay(SWIPING_TEXT_DELAY); // Delay to control animation speed
    }
}
