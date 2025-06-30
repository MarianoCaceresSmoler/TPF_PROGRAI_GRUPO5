/***************************************************************************/ /**
   @scores.h
   @Saving and updating scores
   @Grupo_5
  ******************************************************************************/

#ifndef SCORES_H
#define SCORES_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "game.h"

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
 * @brief increment player score
 * @param game pointer to game object
 * @param points the number to add to score
 */
void incrementScore(game_t *game, int points);

/**
 * @brief init scores system, load top scores in saved file
 */
void scores_init();

/**
 * @brief get top scores saved
 * @return array of scores
 */
int *get_top_scores();

/**
 * @brief add new score to ranking if qualifies
 * @param score the score to add
 */
void add_score(int score);

/**
 * @brief save actual ranking to file
 */
void save_scores();

/*******************************************************************************
 ******************************************************************************/

#endif // SCORES_H