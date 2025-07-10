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

#include "../backend/game.h"
#include "../backend/config.h"
#include "render_rpi.h"
#include "input_rpi.h"
#include "audio_rpi.h"

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
	game_t game;
	gameInit(&game);
	// initAudio();
	initGraphics(&game);

	bool programRunning = true;
	inputStatus_t inputStatus = {false, false, false, false, false, false, false, false};

	initInput(&inputStatus);

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
			printf("game menu");

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
			}
			break;

		case GAME_LOADING:
			printf("game loading");

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
			printf("game running");

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

			gameUpdate(&game, inputStatus);
			break;

		case GAME_PAUSED:
			printf("game paused");

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
			}
			else if (inputStatus.restartKeyPressed)
			{
				// Restarts game
				// currentPoints = 0;
				// currentLives = SHIP_LIVES;
				gameReset(&game);
			}
			else if (inputStatus.exitKeyPressed)
			{
				programRunning = false;
			}

			break;

		case GAME_END:

			printf("game end");

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
				// gameoverSoundPlayed = false;
			}
			else if (inputStatus.exitKeyPressed)
			{
				programRunning = false;
			}

			break;
		default:
			programRunning = false;
			printf("Error: Invalid game status.\n");
			break;
		}

		resetInputFlags(&inputStatus);
	}

	// Cleanup when program ends
	cleanupGraphics();
	// cleanupAudio();
	clearInput();
	printf("\nProgram finished successfully.\n\n");

	return 0;
}
