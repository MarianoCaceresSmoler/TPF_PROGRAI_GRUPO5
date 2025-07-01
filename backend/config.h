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

#define LOGICAL_WIDTH 64
#define LOGICAL_HEIGHT 32

#ifdef PLATFORM_PC

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define ALIENS_ROWS 5
#define ALIENS_COLS 11

#define SHIP_WIDTH 240
#define SHIP_HEIGHT 120
#define MOTHERSHIP_WIDTH 240
#define MOTHERSHIP_HEIGHT 120
#define EXPLOSION_WIDTH 120
#define EXPLOSION_HEIGHT 120
#define ALIEN_WIDTH 240
#define ALIEN_HEIGHT 120
#define BULLET_WIDTH 40
#define BULLET_HEIGHT 20
#define POWERUP_WIDTH 120
#define POWERUP_HEIGHT 120
#define BARRIER_PIXEL_WIDTH 40
#define BARRIER_PIXEL_HEIGHT 40
#define BARRIER_HEIGHT 4
#define BARRIER_WIDTH 6
#define BARRIERS 4

#define TITLE_WIDTH 800
#define TITLE_HEIGHT 200

#define SHIP_INITIAL_X (SCREEN_WIDTH / 2 - SHIP_HEIGHT / 2)
#define SHIP_INITIAL_Y (SCREEN_HEIGHT * 0.95 - SHIP_HEIGHT / 2)
#define ALIENS_INITIAL_X (SCREEN_WIDTH / ALIENS_COLS - ALIEN_HEIGHT / 2)
#define ALIENS_INITIAL_Y (ALIEN_HEIGHT)
#define SCORE_INITIAL_X 20
#define SCORE_INITIAL_Y 20

#define ALIEN_MAX_MOVE_INTERVAL 60
#define ALIEN_MIN_MOVE_INTERVAL 12
#define ALIEN_MAX_MOVE_TICKRATE 15
#define ALIEN_MIN_MOVE_TICKRATE 2
#define MOTHERSHIP_MOVE_RATE 12
#define BULLET_MOVE_RATE 15
#define SHIP_MOVE_RATE 12

#else // Raspberry Pi

#define SCREEN_WIDTH 16
#define SCREEN_HEIGHT 16

#define ALIENS_ROWS 4
#define ALIENS_COLS 5

#define SHIP_WIDTH 3
#define SHIP_HEIGHT 2
#define MOTHERSHIP_WIDTH 3
#define MOTHERSHIP_HEIGHT 2
#define EXPLOSION_WIDTH 2
#define EXPLOSION_HEIGHT 2
#define ALIEN_WIDTH 2
#define ALIEN_HEIGHT 1
#define BULLET_WIDTH 1
#define BULLET_HEIGHT 2
#define POWERUP_WIDTH 1
#define POWERUP_HEIGHT 1
#define BARRIER_PIXEL_WIDTH 1
#define BARRIER_PIXEL_HEIGHT 1
#define BARRIER_WIDTH 4
#define BARRIER_HEIGHT 3
#define BARRIERS 3

#define TITLE_WIDTH 8
#define TITLE_HEIGHT 2

#define SHIP_INITIAL_X 1
#define SHIP_INITIAL_Y 14
#define ALIENS_INITIAL_X 1
#define ALIENS_INITIAL_Y 1
#define SCORE_INITIAL_X 0
#define SCORE_INITIAL_Y 0

#define ALIEN_MAX_MOVE_INTERVAL 5
#define ALIEN_MIN_MOVE_INTERVAL 1
#define ALIEN_MAX_MOVE_TICKRATE 20
#define ALIEN_MIN_MOVE_TICKRATE 10
#define MOTHERSHIP_MOVE_RATE 1
#define BULLET_MOVE_RATE 1
#define SHIP_MOVE_RATE 1

#endif

#define SHIP_LIVES 3

#define ALIENS_NUMBER (ALIENS_COLS * ALIENS_ROWS)

#define MOTHERSHIP_CHANCE 10 // %
#define MOTHERSHIP_INITIAL_X (0 - MOTHERSHIP_WIDTH)
#define MOTHERSHIP_INITIAL_Y 0

#define MAX_SCORES 10
#define SCORE_FILE "scores.txt"

#define ALIEN_TYPE_0_POINTS 50
#define ALIEN_TYPE_1_POINTS 40
#define ALIEN_TYPE_2_POINTS 30
#define ALIEN_TYPE_3_POINTS 20
#define ALIEN_TYPE_4_POINTS 10

#define FPS 30

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

/**
 * @brief function to get a random number using rand from stdlib
 * @param min min value
 * @param max max value
 * @return a random number between min and max
 */
int get_random_number(int min, int max);

/*******************************************************************************
 ******************************************************************************/

#endif // _CONFIG_H_
