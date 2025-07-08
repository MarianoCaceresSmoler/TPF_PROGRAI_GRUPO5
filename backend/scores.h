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
/*
typedef struct 
{
	unsigned char tag[4];
	unsigned short int  score;
} score_t;

score_t highScores[MAX_SCORES];
*/
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
 * @brief get a top score 
 * @return array of scores
 */
getHighScores(int rank);

/**
 * @brief add new score to ranking if qualifies
 * @param score the score to add
 */
void addScore(int score);

/**
 * @brief save actual ranking to file
 */
void saveScores();

/**
 * @brief function to get the amount of points given by the type of alien killed
 * @param alien is the alien killed
 * @return int with the score the alien gives
 */
int getAlienPoints(alien_t alien);

/*******************************************************************************
 ******************************************************************************/

#endif // SCORES_H