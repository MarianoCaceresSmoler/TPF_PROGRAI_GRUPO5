/***************************************************************************//**
  @file 	  scores.h
  @brief 	  Score functions to update and get max scores
  @author 	Grupo_5
 ******************************************************************************/

#ifndef SCORES_H
#define SCORES_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "config.h"

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef struct // Structure of a score with its nametag
{
	char tag[MAX_NAME_CHARS + 1];
	unsigned int score;
} score_t;


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief updates score file with the new score
 * @param lastScore the score to list
 * @return the rank of the score in the top scores list, 0 if score is not in top 10, -1 if there was an error
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
 * @param alienType is the type of alien killed
 * @return int with the score the alien gives, 0 if the alien is not in the list
 */
int getAlienPoints(unsigned char alienType);

/*******************************************************************************
 ******************************************************************************/

#endif // SCORES_H