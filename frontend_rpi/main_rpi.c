/***************************************************************************/ /**
   @main_rpi.c
   @Main program for the RPI version of the game.
   @Grupo_5
  ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
// +Incluir el header propio (ej: #include "template.h")+

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>	 // usleep()
#include <pthread.h> // Threads

#include "../backend/game.h"
#include "../backend/config.h"
#include "render_rpi.h"
#include "input_rpi.h"
#include "audio_rpi.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define FRAME_TIME_MS (1000 / FPS) // ~33 ms por frame
#define FRAME_TIME_US (FRAME_TIME_MS * 1000)

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

// static input_state_t inputState = {false, false};

/*******************************************************************************
 *******************************************************************************
						GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
						LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

int main(void)
{
    // Main program loop control
    bool programRunning = true;

    // Game state structure
    game_t game;

    // Input status flags for keys
    inputStatus_t inputStatus = {false, false, false, false, false, false, false, false};

    // Initialize the game structure
    gameInit(&game);	

    // Initialize graphics; exit if failed
    if(initGraphics(&game))
    {
        printf("Error initializing graphics");
        return -1;
    }

    // Initialize input; exit if failed
    if(initInput(&inputStatus))
    {
        printf("Error initializing input");
        return -1;
    }

    // Initialize audio; exit if failed
    if(initializeAudio())
    {
        printf("Error initializing audio");
        return -1;
    }

    // Flags and variables for managing audio and game events
    bool isGameplayMusicPlaying = false;
    bool isMotherShipPlaying = false;
    bool gameoverSoundPlayed = false;
    bool isFirstTry = true;
    int currentPoints = 0;
    int currentLives = SHIP_INITIAL_LIVES;
    int currentPowerUps[POWERUP_TYPES] = {0, 0, 0, 0}; // Tracks current powerup status
    int i;

    // Main game loop
    while (programRunning)
    {
        switch (game.status)
        {
        case GAME_MENU:
            // Start the game if resume key is pressed
            if (inputStatus.resumeKeyPressed)
            {
                levelInit(&game);
                resetInputFlags(&inputStatus);
            }
            // Exit game if exit key is pressed
            else if (inputStatus.exitKeyPressed)
            {
                programRunning = false;
            }
            break;

        case GAME_LOADING:
            // Countdown loading timer before entering the game
            if (game.loadingTimer > 0)
                game.loadingTimer--;
            else
            {
                // Play music depending on whether it's the first try or a resume
                if (isFirstTry)
                {
                    playGameMusic();
                    isFirstTry = false;
                }
                else
                {
                    resumeMusic();
                }
                isGameplayMusicPlaying = true;
                game.status = GAME_RUNNING;
            }
            break;

        case GAME_RUNNING:
            // Resume music if it was stopped
            if (!isGameplayMusicPlaying)
            {
                resumeMusic();
                isGameplayMusicPlaying = true;
            }

            // Check for new powerups collected and play sound
            for (i = 0; i < POWERUP_TYPES; i++)
            {
                if (currentPowerUps[i] != game.activePowerUp[i])
                {
                    currentPowerUps[i] = game.activePowerUp[i];
                    if (currentPowerUps[i] == 1)
                    {
                        playPowerUpSound();
                    }
                }
            }

            // Play explosion sound if score or lives changed
            if (currentPoints != game.score || currentLives != game.ship.livesLeft)
            {
                playExplosionSound();
                currentPoints = game.score;
                currentLives = game.ship.livesLeft;
            }

            // Play shoot sound when player fires
            if (inputStatus.shootKeyPressed && !game.shipBullet.entity.isAlive)
                playShootSound();

            // Play mothership sound only when it's alive
            if (game.mothership.entity.isAlive && !isMotherShipPlaying)
            {
                playMothershipSound();
                isMotherShipPlaying = true;
            }
            else if (!game.mothership.entity.isAlive && isMotherShipPlaying)
            {
                isMotherShipPlaying = false;
            }

            // Pause the game
            if (inputStatus.pauseKeyPressed)
            {
                game.status = GAME_PAUSED;
            }
            else
            {
                // Regular game update cycle
                gameUpdate(&game, inputStatus);
                resetInputFlags(&inputStatus);
            }
            break;

        case GAME_PAUSED:
            // Stop music while paused
            stopMusic();
            isGameplayMusicPlaying = false;

            // Resume game
            if (inputStatus.resumeKeyPressed)
            {
                gameResume(&game);
                resetInputFlags(&inputStatus);
            }
            // Restart game from beginning
            else if (inputStatus.restartKeyPressed)
            {
                currentPoints = 0;
                currentLives = SHIP_INITIAL_LIVES;
                gameReset(&game);
                resetInputFlags(&inputStatus);
            }
            // Exit game
            else if (inputStatus.exitKeyPressed)
            {
                programRunning = false;
                game.status = GAME_END;
                resetInputFlags(&inputStatus);
            }
            break;

        case GAME_END:
            // Stop music if it's still playing
            if (isGameplayMusicPlaying)
            {
                stopMusic();
                isGameplayMusicPlaying = false;
            }

            // Play game over sound only once
            if (!gameoverSoundPlayed)
            {
                playGameoverSound();
                gameoverSoundPlayed = true;
            }

            // Restart game after game over
            if (inputStatus.restartKeyPressed)
            {
                currentPoints = 0;
                currentLives = SHIP_INITIAL_LIVES;

                gameReset(&game);
                resetInputFlags(&inputStatus);

                gameoverSoundPlayed = false;
            }
            // Exit game
            else if (inputStatus.exitKeyPressed)
            {
                programRunning = false;
                resetInputFlags(&inputStatus);
            }
            break;

        case GAME_ERROR:
            // Handle unexpected error status
            programRunning = false;
            printf("Error: game error status.\n");
            break;

        default:
            // Handle unknown status values
            programRunning = false;
            printf("Error: Invalid game status.\n");
            break;
        }

        // Delay to maintain a consistent frame rate
        usleep(FRAME_TIME_US);
    }

    // Clean up all resources before exiting
    cleanupGraphics();
    cleanupAudio();
    cleanupInput();
    printf("\nProgram finished.\n\n");

    return 0;
}
