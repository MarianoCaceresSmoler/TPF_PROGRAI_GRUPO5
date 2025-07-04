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

static int top_scores[MAX_SCORES];

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void incrementScore(game_t * game, int points)
{
	game->score += points;
}

void scores_init()
{
    FILE *f = fopen(SCORE_FILE, "r+");
    if (!f)
    {
        int i;
        // if file does not exist, initialize with ceros
        for (i = 0; i < MAX_SCORES; i++)
        {
            top_scores[i] = 0;
        }
        return;
    }

    // read scores from file (scores are stored from highest to lowest)
    if(fread(top_scores, sizeof(int), MAX_SCORES, f) != MAX_SCORES){
        // error reading file
        fprintf(stderr, "Error reading scores file\n");
    }
    fclose(f);
}

int * get_top_scores()
{
    return top_scores;
}

void add_score(int score)
{
    // if top score does not enter the top, exit
    if (score <= top_scores[MAX_SCORES - 1])
        return;

    // insert new score in correct position
    int i, j, scoreInserted;
    for (i = 0, scoreInserted = 0; i < MAX_SCORES && !scoreInserted; i++)
    {
        if (score > top_scores[i])
        {
            // move scores down
            for (j = MAX_SCORES - 1; j > i; j--)
            {
                top_scores[j] = top_scores[j - 1];
            }

            // insert new score
            top_scores[i] = score;
            scoreInserted = 1;
        }
    }
}

void save_scores()
{
    FILE *f = fopen(SCORE_FILE, "w+"); // if file does not exist, create it
    if (!f) 
       return;
    fwrite(top_scores, sizeof(int), MAX_SCORES, f);
    fclose(f);
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
