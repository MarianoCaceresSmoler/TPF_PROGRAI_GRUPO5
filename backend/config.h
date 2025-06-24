/***************************************************************************//**
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

  #define ALIENS_COLS 11
  #define ALIENS_FILS 5

  #define BARRIER_HEIGHT 4
  #define BARRIER_WIDTH 6
  #define BARRIERS 4

  #define SHIP_SIZE 10
  #define ALIEN_SIZE 10
  #define BULLET_SIZE 3
  #define POWERUP_SIZE 5

#else  // Raspberry Pi
  #define SCREEN_WIDTH 16
  #define SCREEN_HEIGHT 16

  #define ALIENS_COLS 5
  #define ALIENS_FILS 3

  #define BARRIER_HEIGHT 2
  #define BARRIER_WIDTH 3
  #define BARRIERS 3

  #define SHIP_SIZE 4
  #define ALIEN_SIZE 3
  #define BULLET_SIZE 1
  #define POWERUP_SIZE 2
#endif

#define BARRIER_PIXEL_SIZE 2

#define ALIEN_MAX_MOVE_INTERVAL 1000
#define ALIEN_MIN_MOVE_INTERVAL 100 

#define BULLET_SPEED 10

#define SHIP_MOVE_RATE 5

#define MOTHERSHIP_CHANCE 10  // %

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
 * @brief TODO: returns 1 to spawn mothership, if not returns 0
 * @param param1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
 * @return Descripcion valor que devuelve
*/
// +ej: char lcd_goto (int fil, int col);+

/**
 * @brief TODO: function to get a random number using rand from stdlib
 * @param min min value
 * @param max max value
 * @return a random number between min and max
*/
int get_random_number(int min, int max);


/*******************************************************************************
 ******************************************************************************/

#endif // _CONFIG_H_