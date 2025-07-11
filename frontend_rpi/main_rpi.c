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
	// initAudio();

	// // For audio management
	// bool isMenuMusicPlaying = false;
	// bool isGameplayMusicPlaying = false;
	// bool isMothershipSoundPlaying = false;
	// bool gameoverSoundPlayed = false;
	// int currentPoints = 0;
	// int currentLives = SHIP_LIVES;

	while (programRunning)
	{
		switch (game.status)
		{
		case GAME_MENU:

			// if (!isMenuMusicPlaying) // Inits menu music only when game starts
			// {
			// 	playMenuMusic();
			// 	isMenuMusicPlaying = true;
			// }

			if (inputStatus.resumeKeyPressed)
			{
				// stopMenuMusic();
				// isMenuMusicPlaying = false;
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
				// playGameplayMusic();
				// isGameplayMusicPlaying = true;
				game.status = GAME_RUNNING;
			}

			break;

		case GAME_RUNNING:

			// if (currentPoints != game.score || currentLives != game.ship.livesLeft) // Updates points when score changes and plays explosion sound
			// {

			// 	playExplosionSound();
			// 	currentPoints = game.score;
			// 	currentLives = game.ship.livesLeft;
			// }

			// if (!isGameplayMusicPlaying)
			// {
			// 	resumeGameplayMusic();
			// 	isGameplayMusicPlaying = true;
			// }

			// if (inputStatus.shootKeyPressed && !game.shipBullet.entity.isAlive) // Plays shot sound when shoot key is pressed
			// 	playShootSound();
			// if (game.mothership.entity.isAlive && !isMothershipSoundPlaying) // Plays mothership sound when it appears
			// {
			// 	playMothershipSound();
			// 	isMothershipSoundPlaying = true;
			// }
			// else if (!game.mothership.entity.isAlive && isMothershipSoundPlaying)
			// {
			// 	stopMothershipSound();
			// 	isMothershipSoundPlaying = false;
			// }

			if (inputStatus.pauseKeyPressed)
			{
				game.status = GAME_PAUSED;
			}

			gameUpdate(&game, inputStatus);
			resetInputFlags(&inputStatus);

			break;

		case GAME_PAUSED:

			// if (isGameplayMusicPlaying) // Stops gameplay music when paused
			// {
			// 	stopGameplayMusic();
			// 	isGameplayMusicPlaying = false;
			// }

			// if (isMothershipSoundPlaying)
			// {
			// 	stopMothershipSound();
			// 	isMothershipSoundPlaying = false;
			// }

			if (inputStatus.resumeKeyPressed)
			{
				// Resumes game
				// resumeGameplayMusic();
				// isGameplayMusicPlaying = true;
				gameResume(&game);
				resetInputFlags(&inputStatus);
			}
			else if (inputStatus.restartKeyPressed)
			{
				// Restarts game
				// currentPoints = 0;
				// currentLives = SHIP_LIVES;
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

			// if (isGameplayMusicPlaying)
			// {
			// 	stopGameplayMusic(); // Stops gameplay music when game ends
			// 	isGameplayMusicPlaying = false;
			// }

			// if (isMothershipSoundPlaying)
			// {
			// 	stopMothershipSound();
			// 	isMothershipSoundPlaying = false;
			// }

			// if (!gameoverSoundPlayed) // Plays gameover sound only when game ends
			// {
			// 	playGameoverSound();
			// 	gameoverSoundPlayed = true;
			// }

			if (inputStatus.restartKeyPressed)
			{
				// currentPoints = 0;
				// currentLives = SHIP_LIVES;
				gameReset(&game);
				resetInputFlags(&inputStatus);

				// gameoverSoundPlayed = false;
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
		usleep(FRAME_TIME_MS * 1000); // convert to us
	}

	// Cleanup when program ends
	cleanupGraphics();
	// cleanupAudio();
	clearInput();
	printf("\nProgram finished successfully.\n\n");

	return 0;
}
