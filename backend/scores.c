/***************************************************************************/ /**
   @scores.c
   @Saving and updating scores
   @Grupo_5
  ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
// +Incluir el header propio (ej: #include "template.h")+

#include <stdio.h>
#include <string.h>
#include "scores.h"
#include "config.h"
#include "game.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: unsigned int anio_actual;+

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static void falta_envido (int);+

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static int temperaturas_actuales[4];+

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void incrementScore(game_t * game, int points)
{
	game->score += points;
}
/*
int * readTopScore(int rank)
{
    return highScores;
}

void addScore(int score)
{
    // if top score does not enter the top, exit
    if (score <= highScores[MAX_SCORES - 1].score)
        return;

    // insert new score in correct position
    int i, j, scoreInserted;
    for (i = 0, scoreInserted = 0; i < MAX_SCORES && !scoreInserted; i++)
    {
        if (score > highScores[i].score)
        {
            // move scores down
            for (j = MAX_SCORES - 1; j > i; j--)
            {
                highScores[j].score = highScores[j - 1].score;
            }

            // insert new score
            highScores[i].score = score;
            scoreInserted = 1;
        }
    }
}

void saveScores()
{
    FILE *f = fopen(SCORE_FILE, "w+"); // if file does not exist, create it
    if (!f) 
       return;
    fwrite(highScores[0].score, sizeof(int), MAX_SCORES, f);
    fclose(f);
}
*/
int getAlienPoints(alien_t alien)
{
    switch(alien.alienType)
    {
    case 0:
        return ALIEN_TYPE_0_POINTS;
        break;
    case 1:
        return ALIEN_TYPE_1_POINTS;
        break;
    case 2:
        return ALIEN_TYPE_2_POINTS;
        break;
    case 3:
        return ALIEN_TYPE_3_POINTS;
        break;
    case 4:
        return ALIEN_TYPE_4_POINTS;
        break;
    default:
		return 0;
        break;
    }
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
