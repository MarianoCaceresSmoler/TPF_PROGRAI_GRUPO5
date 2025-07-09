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

int updateScoreRank(score_t lastScore)
{
    if(lastScore.score > MAX_SCORE)
        lastScore.score = MAX_SCORE; // if last score was higher than the max score limit

    // Open highscores file in r+ mode        
    FILE *scoreFiles = fopen(SCORE_FILE, "r+");
    if (!scoreFiles)
    {
        printf("Error opening score file.\n");
        return -1;
    }

    int i, rank = -1;
    score_t highScore;

    for (i = MAX_SCORES; i > 0; i--)
    {
        fseek(scoreFiles, (i - 1) * 10, SEEK_SET);
        if(fscanf(scoreFiles, "%3s %05d", highScore.tag, &highScore.score) != 2)
        {
            printf("Error reading score file at position %d.\n", i);
            fclose(scoreFiles);
            return -1;
        }
        if(lastScore.score <= highScore.score)
        {
            rank = i + 1;
            break;
        }
        else if(i == 1)
            rank = 1;
    }

    if(rank <= MAX_SCORES && rank > 0)
    {
        for(i = MAX_SCORES - 1; i >= rank; i--)
        {
            fseek(scoreFiles, (i - 1) * 10, SEEK_SET);
            if(fscanf(scoreFiles, "%3s %05d", highScore.tag, &highScore.score) != 2)
            {
                printf("Error reading score file at position %d.\n", i);
                fclose(scoreFiles);
                return -1;
            }
            fseek(scoreFiles, i * 10, SEEK_SET);
            fprintf(scoreFiles, "%3s %05d\n", highScore.tag, highScore.score);
        }
        fseek(scoreFiles, (rank - 1) * 10, SEEK_SET);
        fprintf(scoreFiles, "%3s %05d\n", lastScore.tag, lastScore.score);
    }

    fclose(scoreFiles);

    return (rank <= MAX_SCORES && rank >= 1) ? rank : 0;
}

int getHighScores(score_t scores[MAX_SCORES])
{
    FILE *scoreFile = fopen(SCORE_FILE, "r");
    if (!scoreFile)
    {
        printf("Error opening score file.\n");
        return -1;
    }

    for (int i = 0; i < MAX_SCORES; i++)
    {
        if (fscanf(scoreFile, "%3s %05d", scores[i].tag, &scores[i].score) != 2)
        {
            // if there are less than 10 valid scores, fill with empty values
            for (; i < MAX_SCORES; i++) {
                strcpy(scores[i].tag, "---");
                scores[i].score = 0;
            }
            break;
        }
    }

    fclose(scoreFile);
    return 0;
}

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
