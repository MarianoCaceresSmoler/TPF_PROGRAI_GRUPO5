/***************************************************************************//**
  @file 	input_pc.c
  @brief 	Management of user inputs
  @author 	Grupo_5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <allegro5/allegro.h>
#include <stdio.h>

#include "../backend/game.h"
#include "../backend/config.h"
#include "input_pc.h"

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void setUserName(game_t *game, int keycode)
{
    if(!game)
    {
        printf("Error: game param is NULL\n");
        return; // check that the game pointer is not NULL
    }

    static int charIndex = 0;

    // allows backspace
    if (keycode == ALLEGRO_KEY_BACKSPACE && charIndex > 0)
    {
        game->nameTag[--charIndex] = '_';
    }

    if(charIndex < MAX_NAME_CHARS) // validates name max characters
    {
        // only letters from A to Z
        if (keycode >= ALLEGRO_KEY_A && keycode <= ALLEGRO_KEY_Z)
        {
            game->nameTag[charIndex++] = 'A' + (keycode - ALLEGRO_KEY_A);
        }
    }

}

void setInput(inputStatus_t *inputStatus, int keycode)
{
    // Change the input status based on the key pressed
    switch (keycode)
    {
    case ALLEGRO_KEY_LEFT:
        inputStatus->leftKeyPressed = true;
        break;
    case ALLEGRO_KEY_RIGHT:
        inputStatus->rightKeyPressed = true;
        break;
    case ALLEGRO_KEY_SPACE:
        inputStatus->shootKeyPressed = true;
        break;
    case ALLEGRO_KEY_P:
        inputStatus->pauseKeyPressed = true;
        break;
    case ALLEGRO_KEY_ENTER:
        inputStatus->resumeKeyPressed = true;
        break;
    case ALLEGRO_KEY_R:
        inputStatus->restartKeyPressed = true;
        break;
    case ALLEGRO_KEY_ESCAPE:
        inputStatus->exitKeyPressed = true;
        break;
    default:
        break; // does nothing if the key is not contempled
    }
}

void clearInput(inputStatus_t *inputStatus, int keycode)
{
    // Change the input status based on the key released
    switch (keycode)
    {
    case ALLEGRO_KEY_LEFT:
        inputStatus->leftKeyPressed = false;
        break;
    case ALLEGRO_KEY_RIGHT:
        inputStatus->rightKeyPressed = false;
        break;
    case ALLEGRO_KEY_SPACE:
        inputStatus->shootKeyPressed = false;
        break;
    case ALLEGRO_KEY_P:
        inputStatus->pauseKeyPressed = false;
        break;
    case ALLEGRO_KEY_ENTER:
        inputStatus->resumeKeyPressed = false;
        break;
    case ALLEGRO_KEY_R:
        inputStatus->restartKeyPressed = false;
        break;
    case ALLEGRO_KEY_ESCAPE:
        inputStatus->exitKeyPressed = false;
        break;
    default:
        break; // does nothing if the key is not contempled
    }
}