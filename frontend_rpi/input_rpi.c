
/***************************************************************************/ /**
   @input_rpi.c
   @Management of user inputs
   @Grupo_5
  ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
// +Incluir el header propio (ej: #include "template.h")+

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include <SDL2/SDL.h> // to use delays
#include <time.h>

#include "../backend/config.h"
#include "../backend/game.h"
#include "joydrv.h"
#include "input_rpi.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define JOY_THRESHOLD 30    // range for movement
#define LONG_PRESS_MS 500

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

static void *updateInput(void *arg);        // private function to update user inputs
static unsigned int getCurrentTimeMs(void); // to get current time in ms

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+

static pthread_t inputThread;
static bool stopInputThread = false;

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static int temperaturas_actuales[4];+

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void initInput(inputStatus_t *inputStatus)
{
    joy_init(); // inits hardware
    pthread_create(&inputThread, NULL, updateInput, inputStatus); // creates thread for inputs
}

void cleanupInput()
{
    stopInputThread = true;
    pthread_join(inputThread, NULL); // waits for thread to finish
}

void resetInputFlags(inputStatus_t *inputStatus)
{
    inputStatus->pauseKeyPressed = false;
    inputStatus->resumeKeyPressed = false;
    inputStatus->restartKeyPressed = false;
    inputStatus->exitKeyPressed = false;
    inputStatus->leftKeyPressed = false;
    inputStatus->rightKeyPressed = false;
    inputStatus->shootKeyPressed = false;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void *updateInput(void *inputStatus)
{
    inputStatus_t *status = (inputStatus_t *)inputStatus;

    bool shootButtonPressed = false, gameStarted = false, gameRunning = false;
    unsigned int shootPressTime = 0, pressDuration;

    while (!stopInputThread)
    {
        joyinfo_t joy = joy_read();

        // process movement in x
        if (joy.x < -JOY_THRESHOLD)
        {

            status->leftKeyPressed = true;
        }
        else if (joy.x > JOY_THRESHOLD)
        {
            status->rightKeyPressed = true;
        }

        // process button
        if (joy.sw == J_PRESS && !shootButtonPressed)
        {
            shootButtonPressed = true;
            shootPressTime = getCurrentTimeMs();

            if (joy.y < -JOY_THRESHOLD)
            {
                // Combination: down + button → exit
                status->exitKeyPressed = true;
            }
            else if (joy.y > JOY_THRESHOLD)
            {
                // Combinación: up + button → restart
                status->restartKeyPressed = true;
            }
        }
        else if (joy.sw == J_NOPRESS && shootButtonPressed)
        {
            shootButtonPressed = false;
            pressDuration = getCurrentTimeMs() - shootPressTime; // time in milliseconds in which the button was pressed

            if (pressDuration < LONG_PRESS_MS)
            {
                // short duration press: shoot
                status->shootKeyPressed = true;
            }
            else
            {
                if (!gameStarted)
                {
                    // To start the game
                    status->resumeKeyPressed = true;
                    status->pauseKeyPressed = false;
                    gameStarted = true;
                    gameRunning = true;
                }
                else
                {
                    // pauses or resumes the game
                    if (!gameRunning)
                    {
                        status->resumeKeyPressed = true;
                        status->pauseKeyPressed = false;
                        gameRunning = true;
                    }
                    else
                    {
                        status->pauseKeyPressed = true;
                        status->resumeKeyPressed = false;
                        gameRunning = false;
                    }
                }
            }
        }
    }

    return NULL;
}

static unsigned int getCurrentTimeMs()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1e6);
}