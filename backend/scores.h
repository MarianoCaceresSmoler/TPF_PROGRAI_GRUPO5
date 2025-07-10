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

#include "entities.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef struct 
{
	char tag[MAX_NAME_CHARS + 1];
	unsigned int score;
} score_t;


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: extern unsigned int anio_actual;+

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief updates score file with the new score
 * @param lastScore the score to update
 * @return the rank of the score in the top scores list, 0 if score is not in top 10
 */
int updateScoreRank(score_t lastScore);

/**
 * @brief get the content in the highscores file
 * @param scores array to save the highscores
 * @return 0 if all ok, -1 if there was an error
 */
int getHighScores(score_t scores[MAX_SCORES]);


/**
 * @brief function to get the amount of points given by the type of alien killed
 * @param alien is the alien killed
 * @return int with the score the alien gives
 */
int getAlienPoints(alien_t alien);

/*******************************************************************************
 ******************************************************************************/

#endif // SCORES_H