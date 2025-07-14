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
        printf("Error opening score file.\n"); // returns -1 if there was an error opening the file
        return -1;
    }

    int i, rank = -1;
    score_t highScore;

    // Search in which position needs to be inserted the new score
    for (i = MAX_SCORES; i > 0; i--)
    {
        fseek(scoreFiles, (i - 1) * 10, SEEK_SET); // positions in the file
        if(fscanf(scoreFiles, "%3s %05d", highScore.tag, &highScore.score) != 2) // reads score position to position
        {
            printf("Error reading score file at position %d.\n", i); // returns -1 if there was an error reading file
            fclose(scoreFiles);
            return -1;
        }

        // If the new score is higher than the current one, update the rank
        if(lastScore.score <= highScore.score)
        {
            rank = i + 1;
            break;
        }
        else if(i == 1) // If the new score is higher than the first one, it will be the first
            rank = 1;
    }

    // If the new score enters the top, update the corresponding positions
    if(rank <= MAX_SCORES && rank > 0)
    {
        // Moves down the scores 
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

        // Inserts the new score
        fseek(scoreFiles, (rank - 1) * 10, SEEK_SET);
        fprintf(scoreFiles, "%3s %05d\n", lastScore.tag, lastScore.score);
    }

    fclose(scoreFiles); // close highscores file

    // Returns the score ranking, or 0 if it did not enter the top
    return (rank <= MAX_SCORES && rank >= 1) ? rank : 0;
}

int getHighScores(score_t scores[MAX_SCORES])
{
    FILE *scoreFile = fopen(SCORE_FILE, "r");
    if (!scoreFile)
    {
        printf("Error opening score file.\n"); // returns -1 if there was an error opening the file
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

    fclose(scoreFile); // close highscores file

    return 0;
}

int getAlienPoints(alien_t alien)
{
    // Returns the points for the alien depending on its type
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
		return 0; // if alien type is not recognized, returns 0
        break;
    }
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
