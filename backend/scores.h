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

typedef struct 
{
	unsigned char tag[4];
	unsigned short int score;
} score_t;

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
 * @brief updates score file with the new score
 * @param lastScore the score to update
 * @return the rank of the score in the top scores list, 0 if score is not in top 10
 */
int updateScoreRank(score_t lastScore);

/**
 * @brief function to get the amount of points given by the type of alien killed
 * @param alien is the alien killed
 * @return int with the score the alien gives
 */
int getAlienPoints(alien_t alien);

/*******************************************************************************
 ******************************************************************************/

#endif // SCORES_H