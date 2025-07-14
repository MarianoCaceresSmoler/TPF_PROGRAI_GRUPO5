/***************************************************************************/ /**
   @config.h
   @Constants and game configurations
   @Grupo_5
  ******************************************************************************/

#ifndef _CONFIG_H_
#define _CONFIG_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define SHIP_INITIAL_LIVES 3 // Initial number of lives for the player
#define SHIP_MAX_LIVES 5 // Maximum number of lives for the player
#define INVENCIBILITY_TICKS 30 // Ticks of invencibility after dying

#define STANDBY_POSITION 2000 // Logical position for inactive entities

#define TIME_SPEED_WEIGHT 0.4
#define ALIEN_COUNT_SPEED_WEIGHT (1 - TIME_SPEED_WEIGHT)

#define MOTHERSHIP_CHANCE 20 // % of mothership chance
#define MOTHERSHIP_TIMER 150 // Interval between every mothership appareance chance
#define POWERUP_CHANCE 15 // % of power up chanche everytime an enemy dies

#define MAX_SCORES 10 // Number of scores displayed on screen
#define MAX_SCORE 99999 // Maximum score that can be saved

#define MAX_NAME_CHARS 3 // Number of characters to save as nametag

#define MIN_MOTHERSHIP_POINTS 100 // Minimum amount of points the mothership can give
#define MAX_MOTHERSHIP_POINTS 250 // Maximum amount of points the mothership can give
#define ALIEN_TYPE_0_POINTS 50 // Points given by a type 0 alien
#define ALIEN_TYPE_1_POINTS 40 // Points given by a type 1 alien
#define ALIEN_TYPE_2_POINTS 30 // Points given by a type 2 alien
#define ALIEN_TYPE_3_POINTS 20 // Points given by a type 3 alien
#define ALIEN_TYPE_4_POINTS 10 // Points given by a type 4 alien

#define EXPLOSION_TIMER 6 // Ticks of duration of the explosion
#define MOTHERSHIP_EXPLOSION_TIMER 30 // Ticks of duration of the mothership explosion
#define TOTAL_INPUTS 8 // Total number of inputs

#define LOADING_TIME (ALIENS_NUMBER) // Time to load aliens on display

#define FPS 30 // Frames per second to render

////////////////////////////////////////////////////////////////////////////////////////
/////////// SPECIFIC PARAMETERS FOR DIFFERENT PLATFORMS ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

#ifdef PLATFORM_RPI

// Display dimensions
#define SCREEN_WIDTH 16
#define SCREEN_HEIGHT 16
#define SCREEN_SIZE SCREEN_WIDTH

// Total number of aliens
#define ALIENS_ROWS 4
#define ALIENS_COLS 4
#define ALIENS_NUMBER (ALIENS_COLS * ALIENS_ROWS)

// Objects dimensions
#define SHIP_WIDTH 3
#define SHIP_HEIGHT 1
#define MOTHERSHIP_WIDTH 3
#define MOTHERSHIP_HEIGHT 2
#define EXPLOSION_WIDTH 2
#define EXPLOSION_HEIGHT 2
#define ALIEN_WIDTH 2
#define ALIEN_HEIGHT 1
#define BULLET_WIDTH 1
#define BULLET_HEIGHT 1
#define POWERUP_WIDTH 2
#define POWERUP_HEIGHT 2
#define BARRIER_PIXEL_WIDTH 1
#define BARRIER_PIXEL_HEIGHT 1
#define BARRIER_WIDTH 4
#define BARRIER_HEIGHT 3

#define BARRIERS 3 // Total number of barriers

// Positions configurations for all entities
#define SHIP_INITIAL_X 1
#define SHIP_INITIAL_Y 15
#define MOTHERSHIP_LEFT_INITIAL_X (0 - MOTHERSHIP_WIDTH)
#define MOTHERSHIP_RIGHT_INITIAL_X (SCREEN_SIZE + MOTHERSHIP_WIDTH)
#define MOTHERSHIP_INITIAL_Y 0
#define ALIENS_INITIAL_X 1
#define ALIENS_INITIAL_Y 2
#define ALIEN_X_SEPARATION 3 
#define ALIEN_Y_SEPARATION 2 
#define BARRIERS_INITIAL_X 1
#define BARRIERS_INITIAL_Y 11
#define BARRIERS_SEPARATION 5
#define SCORE_INITIAL_X 0
#define SCORE_INITIAL_Y 0
#define POWERUP_INITIAL_Y MOTHERSHIP_INITIAL_Y

// Move rates and move intervals
#define ALIEN_X_MOVE_RATE 1
#define ALIEN_Y_MOVE_RATE 1
#define ALIEN_MAX_MOVE_INTERVAL 30
#define ALIEN_MIN_MOVE_INTERVAL 10
#define MOTHERSHIP_MOVE_RATE 1
#define MOTHERSHIP_MOVE_INTERVAL 14
#define BULLET_MOVE_RATE 1
#define BULLET_MOVE_INTERVAL 10
#define SHIP_MOVE_INTERVAL 2
#define SHIP_MOVE_RATE 1
#define POWERUP_MOVE_RATE 1
#define POWERUP_MOVE_INTERVAL 15

// Borders configurations
#define SHIP_LEFT_X_BORDER (0)
#define SHIP_RIGHT_X_BORDER (SCREEN_SIZE - SHIP_WIDTH)
#define ALIENS_X_LEFT_BORDER (0)
#define ALIENS_X_RIGHT_BORDER (SCREEN_SIZE - ALIEN_WIDTH)

// PowerUps configurations
#define POWERUP_TYPES 4
#define POWERUP_USES 1
#define ONE_SECOND 30 // ticks
#define FREEZE_POWERUP_DURATION (ONE_SECOND * 4)

// For space between bullets
#define ALIENS_SHOOT_INTERVAL ONE_SECOND

// File for highscores
#define SCORE_FILE "backend/Files/HiScores_RPI"

#else // PLATFORM PC

// Display dimensions
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define SCREEN_SIZE SCREEN_WIDTH

// Total number of aliens
#define ALIENS_ROWS 5
#define ALIENS_COLS 11
#define ALIENS_NUMBER (ALIENS_COLS * ALIENS_ROWS)

// Objects dimensions
#define SHIP_WIDTH 108
#define SHIP_HEIGHT 72
#define MOTHERSHIP_WIDTH 120
#define MOTHERSHIP_HEIGHT 72
#define EXPLOSION_WIDTH 120
#define EXPLOSION_HEIGHT 120
#define ALIEN_WIDTH 56
#define ALIEN_HEIGHT 56
#define BULLET_WIDTH 12
#define BULLET_HEIGHT 36
#define POWERUP_WIDTH 36
#define POWERUP_HEIGHT 36
#define BARRIER_PIXEL_WIDTH 20
#define BARRIER_PIXEL_HEIGHT 18
#define BARRIER_HEIGHT 4
#define BARRIER_WIDTH 6
#define BARRIERS 6
#define FONT_TEXT_SIZE 42
#define FONT_GAME_SIZE 36
#define TITLE_WIDTH 800
#define TITLE_HEIGHT 200

// Positions configurations for all entities
#define SHIP_INITIAL_X (SCREEN_WIDTH / 9)
#define SHIP_INITIAL_Y (SCREEN_HEIGHT - SHIP_HEIGHT * 2 - FONT_GAME_SIZE * 3 / 2)
#define MOTHERSHIP_LEFT_INITIAL_X (0 - MOTHERSHIP_WIDTH)
#define MOTHERSHIP_RIGHT_INITIAL_X (SCREEN_SIZE + MOTHERSHIP_WIDTH)
#define MOTHERSHIP_INITIAL_Y (FONT_GAME_SIZE * 5 / 2)
#define ALIENS_INITIAL_X (SCREEN_WIDTH / (ALIENS_COLS + 1) - ALIEN_WIDTH / 2)
#define ALIENS_INITIAL_Y (MOTHERSHIP_INITIAL_Y + MOTHERSHIP_HEIGHT + ALIEN_HEIGHT / 2)
#define ALIEN_X_SEPARATION (ALIEN_WIDTH * 5 / 2)
#define ALIEN_Y_SEPARATION (ALIEN_HEIGHT * 3 / 2)
#define BARRIERS_INITIAL_X (SCREEN_SIZE / (BARRIERS * 2 + 1))
#define BARRIERS_INITIAL_Y (SHIP_INITIAL_Y - 2 * BARRIER_HEIGHT * BARRIER_PIXEL_HEIGHT)
#define BARRIERS_SEPARATION (2 * BARRIERS_INITIAL_X)
#define SCORE_INITIAL_X (SCREEN_SIZE + ((SCREEN_WIDTH-SCREEN_SIZE)/3))
#define SCORE_INITIAL_Y (SCREEN_HEIGHT/5)
#define POWERUP_INITIAL_Y MOTHERSHIP_INITIAL_Y

// Move rates and move intervals
#define ALIEN_X_MOVE_RATE 22
#define ALIEN_Y_MOVE_RATE 46
#define ALIEN_MAX_MOVE_INTERVAL 12
#define ALIEN_MIN_MOVE_INTERVAL 1
#define MOTHERSHIP_MOVE_RATE 8
#define BULLET_MOVE_RATE 15
#define SHIP_MOVE_RATE 10
#define POWERUP_MOVE_RATE 5

// Borders configurations
#define SHIP_LEFT_X_BORDER (SHIP_MOVE_RATE)
#define SHIP_RIGHT_X_BORDER (SCREEN_SIZE - SHIP_WIDTH - SHIP_MOVE_RATE)
#define ALIENS_X_LEFT_BORDER (ALIEN_X_MOVE_RATE)
#define ALIENS_X_RIGHT_BORDER (SCREEN_SIZE - ALIEN_WIDTH - ALIEN_X_MOVE_RATE)

// PowerUps configurations
#define POWERUP_TYPES 4
#define POWERUP_USES 1
#define ONE_SECOND 30 // ticks
#define FREEZE_POWERUP_DURATION (ONE_SECOND * 6)

// For space between bullets
#define ALIENS_SHOOT_INTERVAL (ONE_SECOND / 2)

// File for highscores
#define SCORE_FILE "backend/Files/HiScores_PC" // Path to the score file

#endif

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: extern unsigned int anio_actual;+

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 ******************************************************************************/

#endif // _CONFIG_H_
