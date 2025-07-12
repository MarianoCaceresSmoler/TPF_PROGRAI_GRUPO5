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

	bool programRunning = true;
	game_t game;
	inputStatus_t inputStatus = {false, false, false, false, false, false, false, false};

	gameInit(&game);
	initGraphics(&game);
	initInput(&inputStatus);
	initializeAudio();

	// For audio management
	bool isGameplayMusicPlaying = false;
	bool isMothershipSoundPlaying = false;
	bool gameoverSoundPlayed = false;
	bool isFirstTry = true;
	int currentPoints = 0;
	int currentLives = SHIP_INITIAL_LIVES;
	int currentPowerUps[POWERUP_TYPES] = {0, 0, 0, 0};
	int i;

	while (programRunning)
	{
		switch (game.status)
		{
		case GAME_MENU:

			if (inputStatus.resumeKeyPressed)
			{
				levelInit(&game);
				resetInputFlags(&inputStatus);
			}
			else if (inputStatus.exitKeyPressed)
			{
				programRunning = false;
			}

			break;

		case GAME_LOADING:

			if (game.loadingTimer > 0)
				game.loadingTimer--;
			else
			{
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

			if (!isGameplayMusicPlaying)  // resumes game music if it is paused
			{
				resumeMusic();
				isGameplayMusicPlaying = true;
			}

			for (i = 0; i < POWERUP_TYPES; i++) // check if a powerup was collected to play powerup sound
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

			if (currentPoints != game.score || currentLives != game.ship.livesLeft) // Updates points when score changes and plays explosion sound
			{
				playExplosionSound();
				currentPoints = game.score;
				currentLives = game.ship.livesLeft;
			}

			if (inputStatus.shootKeyPressed && !game.shipBullet.entity.isAlive) // Plays shot sound when shoot key is pressed
				playShootSound();

			if (game.mothership.entity.isAlive && !isMothershipSoundPlaying) // Plays mothership sound when it appears
			{
				playMothershipSound();
				isMothershipSoundPlaying = true;
			}
			else if (!game.mothership.entity.isAlive && isMothershipSoundPlaying)
			{
				isMothershipSoundPlaying = false;
			}


			if (inputStatus.pauseKeyPressed)
			{
				game.status = GAME_PAUSED;
			}
			else
			{
				gameUpdate(&game, inputStatus);
				resetInputFlags(&inputStatus);
			}

			break;

		case GAME_PAUSED:

			stopMusic();
			isGameplayMusicPlaying = false;

			if (isMothershipSoundPlaying)
				isMothershipSoundPlaying = false;

			if (inputStatus.resumeKeyPressed)
			{
				// Resumes game
				if (game.mothership.entity.isAlive)
					isMothershipSoundPlaying = true;

				gameResume(&game);
				resetInputFlags(&inputStatus);
			}
			else if (inputStatus.restartKeyPressed)
			{
				// Restarts game
				currentPoints = 0;
				currentLives = SHIP_INITIAL_LIVES;
				gameReset(&game);
				resetInputFlags(&inputStatus);
			}
			else if (inputStatus.exitKeyPressed)
			{
				programRunning = false;
				resetInputFlags(&inputStatus);
			}

			break;

		case GAME_END:

			if (isGameplayMusicPlaying)
			{
				stopMusic(); // Stops gameplay music when game ends
				isGameplayMusicPlaying = false;

				if (isMothershipSoundPlaying)
					isMothershipSoundPlaying = false;
			}

			if (!gameoverSoundPlayed) // Plays gameover sound only when game ends
			{
				playGameoverSound();
				gameoverSoundPlayed = true;
			}

			if (inputStatus.restartKeyPressed)
			{
				currentPoints = 0;
				currentLives = SHIP_INITIAL_LIVES;

				gameReset(&game);
				resetInputFlags(&inputStatus);

				gameoverSoundPlayed = false;
			}
			else if (inputStatus.exitKeyPressed)
			{
				programRunning = false;
				resetInputFlags(&inputStatus);
			}

			break;

		default:
			programRunning = false;
			printf("Error: Invalid game status.\n");
			break;
		}

		// Delay to keep the FPS
		usleep(FRAME_TIME_US); // convert to us to delay
	}

	// Cleanup when program ends
	cleanupGraphics();
	cleanupAudio();
	cleanupInput();
	printf("\nProgram finished successfully.\n\n");

	return 0;
}
