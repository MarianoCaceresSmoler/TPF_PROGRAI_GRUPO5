
/***************************************************************************/ /**
   @input_pc.c
   @Management of user inputs
   @Grupo_5
  ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
// +Incluir el header propio (ej: #include "template.h")+

#include <allegro5/allegro.h>
#include "../backend/game.h"
#include "../backend/config.h"

#include "input_pc.h"

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

void setUserName(game_t *game, int keycode)
{
    static int charIndex = 0;

    // allows backspace
    if (keycode == ALLEGRO_KEY_BACKSPACE && charIndex > 0)
    {
        game->nameTag[--charIndex] = '_';
    }

    if (charIndex >= MAX_NAME_CHARS) // validates name max characters
        return;

    // only letters from A to Z
    if (keycode >= ALLEGRO_KEY_A && keycode <= ALLEGRO_KEY_Z)
    {
        game->nameTag[charIndex++] = 'A' + (keycode - ALLEGRO_KEY_A);
    }

}

void setInput(inputStatus_t *inputStatus, int keycode)
{

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
        break;
    }
}

void clearInput(inputStatus_t *inputStatus, int keycode)
{
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
        break;
    }
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
