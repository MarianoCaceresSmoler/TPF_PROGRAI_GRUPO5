/***************************************************************************//**
  @file     +Nombre del archivo (ej: template.h)+
  @brief    +Descripcion del archivo+
  @author   +Nombre del autor (ej: Salvador Allende)+
 ******************************************************************************/

#ifndef RENDER_H
#define RENDER_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



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
 * @brief completar descripcion
 * @param param1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
 * @return Descripcion valor que devuelve
*/
// +ej: char lcd_goto (int fil, int col);+

void render_game(game_t *game);  // Dibuja todo
void draw_ship(ship_t *ship);
void draw_aliens(alienFormation_t *aliens);
void draw_bullets(bullet_t bullets[]);
void draw_barriers(barrier_t barriers[]);
void draw_score(int score);
void init_graphics();
void cleanup_graphics();


/*******************************************************************************
 ******************************************************************************/

#endif // RENDER_H
