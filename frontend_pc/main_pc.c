/***************************************************************************/ /**
   @main_pc.c
   @Main program for the PC version of the game.
   @Grupo_5
  ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
// +Incluir el header propio (ej: #include "template.h")+

#include <stdio.h>
#include <allegro5/allegro.h>
#include "../backend/game.h"
#include "../backend/config.h"
#include "render_pc.h"
#include "input_pc.h"
#include "audio_pc.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

enum inputKeys
{
	KEY_LEFT,
	KEY_RIGHT,
	KEY_UP,
	KEY_SHOOT,
	KEY_PAUSE,
	KEY_RESUME,
	KEY_RESTART,
	KEY_EXIT,
};

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

// int getRandomNumber(int min, int max)
// {
// 	srand(time(NULL)); // Seed the random number generator with the current time
// 	return rand() % (max - min + 1) + min;
// }

/*******************************************************************************
 *******************************************************************************
						LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

// REESCRIBI MAIN PARA HACER MEJOR LA LOGICA
int main(void)
{
	// Allegro and game initialization
	initGraphics();
	initAudio();
	game_t game;
	gameInit(&game);

	// Allegro variables to manage the game loop and the inputs
	bool programRunning = true;
	ALLEGRO_EVENT ev;
	inputStatus_t inputStatus = {false, false, false, false, false, false, false, false};
	bool anyKeyPressed = false;

	// For audio management
	bool isMenuMusicPlaying = false;
	bool isGameplayMusicPlaying = false;
	bool gameoverSoundPlayed = false;
	int currentPoints = 0;
	int currentLives = SHIP_LIVES;

	while (programRunning)
	{
		if (al_get_next_event(getEventQueue(), &ev))
		{
			if (ev.type == ALLEGRO_EVENT_TIMER)
			{
				if (currentPoints != game.score || currentLives != game.ship.livesLeft) // Updates points when score changes and plays explosion sound
				{
					playExplosionSound();
					currentPoints = game.score;
					currentLives = game.ship.livesLeft;
				}

				switch (game.status)
				{
				case GAME_MENU:
					if (!isMenuMusicPlaying) // Inits menu music only when game starts
					{
						playMenuMusic();
						isMenuMusicPlaying = true;
					}

					renderMenu(game);
					if (anyKeyPressed)
					{
						// Changes background music and inits level
						stopMenuMusic();
						isMenuMusicPlaying = false;
						playGameplayMusic();
						isGameplayMusicPlaying = true;

						levelInit(&game);
					}
					break;

				case GAME_RUNNING:

					if (inputStatus.shootKeyPressed && !game.shipBullet.entity.isAlive) // Plays shot sound when shoot key is pressed
						playShootSound();

					gameUpdate(&game, inputStatus);
					renderGame(game);
					break;

				case GAME_PAUSED:

					if (isGameplayMusicPlaying) // Stops gameplay music when paused
					{
						stopGameplayMusic();
						isGameplayMusicPlaying = false;
					}

					renderMenu(game);
					if (inputStatus.resumeKeyPressed)
					{
						// Resumes game
						resumeGameplayMusic();
						isGameplayMusicPlaying = true;
						gameResume(&game);
					}
					else if (inputStatus.restartKeyPressed)
					{
						// Restarts game
						playGameplayMusic();
						isGameplayMusicPlaying = true;
						gameReset(&game);
					}
					else if (inputStatus.exitKeyPressed)
					{
						gameEnd(&game);
						programRunning = false;
					}

					break;

				case GAME_END:

					if(isGameplayMusicPlaying)
					{
						stopGameplayMusic(); // Stops gameplay music when game ends
						isGameplayMusicPlaying = false;
					}

					if (!gameoverSoundPlayed) // Plays gameover sound only when game ends
					{
						playGameoverSound();
						gameoverSoundPlayed = true;
					}
					renderGameOver(game);

					if (inputStatus.restartKeyPressed)
					{
						playGameplayMusic(); // Play the gameplay music if game restarts
						gameReset(&game);
						gameoverSoundPlayed = false;
					}
					else if (inputStatus.exitKeyPressed)
					{
						gameEnd(&game);
						programRunning = false;
					}
					break;
				default:
					gameEnd(&game);
					programRunning = false;
					printf("Error: Invalid game status.\n");
					break;
				}
			}
			else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				anyKeyPressed = true;

				setInput(&inputStatus, ev.keyboard.keycode);

				// Manages if the player pauses or exits the game
				if (inputStatus.pauseKeyPressed)
					game.status = GAME_PAUSED;
				else if (inputStatus.exitKeyPressed)
					game.status = GAME_END;
			}
			else if (ev.type == ALLEGRO_EVENT_KEY_UP)
			{
				clearInput(&inputStatus, ev.keyboard.keycode);
			}
			else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				gameEnd(&game);
				programRunning = false;
			}
		}
	}

	// Cleanup allegro when program ends
	cleanupGraphics();
	cleanupAudio();
	printf("\nProgram finished successfully.\n\n");
	return 0;
}

/*
int main(void)
{
	// Init allegro
	initGraphics();
	initAudio();

	game_t game;
	gameInit(&game);

	// Allegro variables to manage the game loop and the inputs
	int programRunning = true;
	ALLEGRO_EVENT ev;
	input_t currentInput = INPUT_NONE, releasedInput = INPUT_NONE;

	// Variables to manage sounds
	int currentPoints = 0, mothershipFlag = false, shipDiedFlag = false;

	while (programRunning)
	{
		al_wait_for_event(getEventQueue(), &ev);
		if (ev.type == ALLEGRO_EVENT_TIMER) // Draws updated game frame to frame
		{
			if (currentPoints != game.score) // Check if the score has changed to reproduce explosion sound
			{
				playExplosionSound();
				currentPoints = game.score;
			}

			// Check if the mothership has appeared to reproduce mothership sound
			if (game.mothership.entity.isAlive && !mothershipFlag)
			{
				playMothershipSound();
				mothershipFlag = true;
			}
			else if (!game.mothership.entity.isAlive && mothershipFlag)
				mothershipFlag = 0;

			// Check if the ship has died to reproduce larger explosion sound
			if (game.ship.entity.explosionTimer > 0 && !shipDiedFlag)
			{
				playShipDiedSound();
				shipDiedFlag = true;
			}
			else if (!game.ship.entity.isAlive && shipDiedFlag)
				shipDiedFlag = false;

			switch (game.status)
			{
			case GAME_MENU:
				playMenuMusic(); // Play the menu music when program starts
				renderMenu(game);
				if (currentInput)
				{
					stopMenuMusic();
					playGameplayMusic();
					levelInit(&game);
				}

				break;

			case GAME_RUNNING:
				if (inputState.leftPressed)
					gameUpdate(&game, INPUT_LEFT);
				else if (inputState.rightPressed)
					gameUpdate(&game, INPUT_RIGHT);

				if (currentInput == INPUT_PAUSE)
					gamePause(&game);
				else if (currentInput == INPUT_SHOOT)
					playShootSound(); // Play the shoot sound when the player shoots

				gameUpdate(&game, currentInput);

				renderGame(game);
				break;

			case GAME_PAUSED:
				stopGameplayMusic(); // Stop the music when the game is paused
				renderMenu(game);
				if (currentInput == INPUT_RESUME)
				{
					playGameplayMusic(); // Play the gameplay music when game resumes
					gameResume(&game);
				}
				else if (currentInput == INPUT_RESTART)
				{
					playGameplayMusic(); // Play the gameplay music when game restarts
					levelInit(&game);	 // CAMBIAR A GAMERESET (RESET VIDAS, ETC)
				}
				else if (currentInput == INPUT_EXIT)
				{
					gameEnd(&game);
					programRunning = false;
				}
				break;

			case GAME_END:
				stopGameplayMusic(); // Stop the music when the game ends
				playGameoverSound();
				renderGameOver(game);
				if (currentInput == INPUT_RESTART)
				{
					playGameplayMusic(); // Play the gameplay music when game restarts
					levelInit(&game);	 // CAMBIAR A GAMERESET (RESET VIDAS, ETC)
				}
				else if (currentInput == INPUT_EXIT)
				{
					gameEnd(&game);
					programRunning = false;
				}
				break;

			default:
				stopGameplayMusic();
				gameEnd(&game);
				programRunning = false;
				break;
			}

			currentInput = INPUT_NONE; // Reset input after 1 frame
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) // Gets input from keyboard
		{
			currentInput = getInputFromKeycode(ev.keyboard.keycode);

			// Update flags of pressed keys
			if (currentInput == INPUT_LEFT)
				inputState.leftPressed = true;
			else if (currentInput == INPUT_RIGHT)
				inputState.rightPressed = true;
		}

		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			releasedInput = getInputFromKeycode(ev.keyboard.keycode);

			// Update flags of released keys
			if (releasedInput == INPUT_LEFT)
				inputState.leftPressed = false;
			else if (releasedInput == INPUT_RIGHT)
				inputState.rightPressed = false;
		}

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			gameEnd(&game);
			programRunning = false;
		}
	}

	// Cleanup allegro when program ends
	cleanupGraphics();
	cleanupAudio();
	printf("\nProgram finished successfully.\n\n");
	return 0;
}
*/