
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

#define JOY_THRESHOLD 30  // range for movement
#define LONG_PRESS_MS 500 // to control long pressing of joystick button

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

/**
 * @brief function to update user inputs in a separate thread
 * @param inputStatus pointer to input status used in main
*/
static void *updateInput(void *inputStatus);

/**
 * @brief function to get current time in ms
 * @return current time in ms
*/
static unsigned int getCurrentTimeMs(void);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+

// Variables to control the input thread
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

int initInput(inputStatus_t *inputStatus)
{
    if(!inputStatus)
    {
        printf("Error: inputStatus param is NULL");
        return -1; // return an error code the inputStatus received is NULL
    }
    
    // Inits joystick hardware
    joy_init();
    // Creates thread for inputs
    if(pthread_create(&inputThread, NULL, updateInput, inputStatus))
    {
        printf("Error creating inputs thread");
        return -1; // return an error code if the thread creation fails
    } 
    
    return 0;
}

void cleanupInput()
{
    stopInputThread = true; // to stop the loop in updateInput
    pthread_join(inputThread, NULL); // waits for thread to finish
}

void resetInputFlags(inputStatus_t *inputStatus)
{
    // Reset all input flags
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
    // Cast the generic pointer to the proper type
    inputStatus_t *status = (inputStatus_t *)inputStatus;

    // Internal state variables to handle button logic
    bool shootButtonPressed = false;      // Tracks if the button is currently held down
    bool gameStarted = false;             // Tracks whether the game has been started at least once
    bool gameRunning = false;             // Tracks whether the game is currently running or paused
    unsigned int shootPressTime = 0;      // Time when the button was pressed
    unsigned int pressDuration;           // How long the button was held down

    // Loop until external flag signals to stop the thread
    while (!stopInputThread)
    {
        // Read the current state of the joystick
        joyinfo_t joy = joy_read();

        // Handle horizontal movement input
        if (joy.x < -JOY_THRESHOLD)
        {
            status->leftKeyPressed = true;  // Move left
        }
        else if (joy.x > JOY_THRESHOLD)
        {
            status->rightKeyPressed = true; // Move right
        }

        // Handle button press logic
        if (joy.sw == J_PRESS && !shootButtonPressed)
        {
            // Button was just pressed
            shootButtonPressed = true;
            shootPressTime = getCurrentTimeMs();  // Save the time the button was pressed

            // Check for button + direction combinations
            if (joy.y < -JOY_THRESHOLD)
            {
                // DOWN + Button → exit the game
                status->exitKeyPressed = true;
            }
            else if (joy.y > JOY_THRESHOLD)
            {
                // UP + Button → restart the game
                status->restartKeyPressed = true;
            }
        }
        else if (joy.sw == J_NOPRESS && shootButtonPressed)
        {
            // Button was just released
            shootButtonPressed = false;
            pressDuration = getCurrentTimeMs() - shootPressTime; // Calculate how long the button was pressed

            if (pressDuration < LONG_PRESS_MS)
            {
                // Short press → shoot
                status->shootKeyPressed = true;
            }
            else
            {
                // Long press logic: Start, pause, or resume the game
                if (!gameStarted)
                {
                    // First long press starts the game
                    status->resumeKeyPressed = true;
                    status->pauseKeyPressed = false;
                    gameStarted = true;
                    gameRunning = true;
                }
                else
                {
                    // Toggle between pause and resume
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

    // Return NULL when the thread exits
    return NULL;
}


static unsigned int getCurrentTimeMs()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);  // Read a high-precision monotonic timer
    return (uint64_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1e6); // Convert to milliseconds
}