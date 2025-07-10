
/***************************************************************************/ /**
   @input_rpi.c
   @Management of user inputs
   @Grupo_5
  ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
// +Incluir el header propio (ej: #include "template.h")+

#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include <SDL2/SDL.h> // to use delays
#include <time.h>

#include "joydrv.h"
#include "../backend/config.h"
#include "../backend/game.h"
#include "input_rpi.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define JOY_THRESHOLD 50 // range for movement
#define INPUT_REFRESH_MS 30  // refresh input every 30ms
#define LONG_PRESS_MS 1000  // 1 sec

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

static void * updateInput(void * arg); // private function to update user inputs
static unsigned int getCurrentTimeMs(void); // to get current time in ms

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+

static inputStatus_t *globalInputStatus = NULL;
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
    globalInputStatus = inputStatus; // to be used in the input thread

    joy_init();  // inits hardware

    pthread_create(&inputThread, NULL, updateInput, NULL); // creates thread for inputs
    pthread_join(inputThread, NULL); // waits for thread to finish
}

void clearInput()
{
    stopInputThread = true;
}

void resetInputFlags(inputStatus_t * inputStatus)
{
    inputStatus->pauseKeyPressed = false;
    inputStatus->resumeKeyPressed = false;
    inputStatus->restartKeyPressed = false;
    inputStatus->exitKeyPressed = false;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void * updateInput(void * arg)
{
    bool shootButtonPressed = false;
    unsigned int shootPressTime = 0;

    while (!stopInputThread)
    {
        joyinfo_t joy = joy_read();

        globalInputStatus->leftKeyPressed = false;
        globalInputStatus->rightKeyPressed = false;
        globalInputStatus->shootKeyPressed = false;

        // process movement in x
        if (joy.x < -JOY_THRESHOLD)
            globalInputStatus->leftKeyPressed = true;
        else if (joy.x > JOY_THRESHOLD)
            globalInputStatus->rightKeyPressed = true;

        // process button
        if (joy.sw == J_PRESS) {

            shootButtonPressed = true;
            shootPressTime = getCurrentTimeMs();

            if (joy.y < JOY_THRESHOLD) {
                // Combination: down + button → exit
                globalInputStatus->exitKeyPressed = true;
            }            
            else if (joy.y > JOY_THRESHOLD) {
                // Combinación: up + button → restart
                globalInputStatus->restartKeyPressed = true;
            }

        } else if (joy.sw == J_NOPRESS && shootButtonPressed) {
            uint64_t pressDuration = getCurrentTimeMs() - shootPressTime; // time in milliseconds in which the button was pressed

            if (pressDuration < LONG_PRESS_MS) {
                // short duration press: shoot
                globalInputStatus->shootKeyPressed = true;
            } else {
                // long duration press: show menu/pause
                if (!globalInputStatus->pauseKeyPressed) {
                    globalInputStatus->pauseKeyPressed = true;
                } else {
                    // resume with anoter long duration press
                    globalInputStatus->resumeKeyPressed = true;
                }
            }

            shootButtonPressed = false;
        }

        SDL_Delay(INPUT_REFRESH_MS); // waits to read the input again

    }

    return NULL;
}

static unsigned int getCurrentTimeMs() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (unsigned int)(ts.tv_sec * 1000 + ts.tv_nsec / 1e6);
}