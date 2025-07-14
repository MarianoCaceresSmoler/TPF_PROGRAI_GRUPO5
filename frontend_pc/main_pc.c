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
	// Allegro and game initialization
	game_t game;
	gameInit(&game);

	initAudio();
	initGraphics();

	// Allegro variables to manage the game loop and the inputs
	bool programRunning = true;
	ALLEGRO_EVENT ev;
	inputStatus_t inputStatus = {false, false, false, false, false, false, false, false};

	// For audio management
	bool isMenuMusicPlaying = false;
	bool isGameplayMusicPlaying = false;
	bool isMothershipSoundPlaying = false;
	bool gameoverSoundPlayed = false;
	unsigned int gameplayMusicPosition = 0;
	int currentPoints = 0;
	int currentLives = SHIP_INITIAL_LIVES;
	int currentPowerUps[POWERUP_TYPES] = {0, 0, 0, 0};
	int i;

	while (programRunning)
	{
		if (al_get_next_event(getEventQueue(), &ev))
		{
			if (ev.type == ALLEGRO_EVENT_TIMER)
			{
				switch (game.status)
				{
				case GAME_MENU:

					if (!isMenuMusicPlaying) // Inits menu music only when game starts
					{
						playMenuMusic();
						isMenuMusicPlaying = true;
					}

					renderMenu(game);

					if (inputStatus.resumeKeyPressed)
					{
						// Changes background music and inits level
						stopMenuMusic();
						isMenuMusicPlaying = false;

						levelInit(&game);
					}
					break;

				case GAME_LOADING:

					renderGame(game);
					if (game.loadingTimer > 0)
						game.loadingTimer--;
					else
					{
						playGameplayMusic();
						isGameplayMusicPlaying = true;
						game.status = GAME_RUNNING;
					}

					break;

				case GAME_RUNNING:

					if (currentPoints != game.score || currentLives > game.ship.livesLeft) // Updates points when score changes and plays explosion sound
					{
						playExplosionSound();
						currentPoints = game.score;
						currentLives = game.ship.livesLeft;
					}

					if (!isGameplayMusicPlaying) // resumes game music if it is paused
					{
						resumeGameplayMusic(gameplayMusicPosition);
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

					if (inputStatus.shootKeyPressed && !game.shipBullet.entity.isAlive) // Plays shot sound when shoot key is pressed
						playShootSound();
					if (game.mothership.entity.isAlive && !isMothershipSoundPlaying) // Plays mothership sound when it appears
					{
						playMothershipSound();
						isMothershipSoundPlaying = true;
					}
					else if (!game.mothership.entity.isAlive && isMothershipSoundPlaying)
					{
						stopMothershipSound();
						isMothershipSoundPlaying = false;
					}

					renderGame(game);
					gameUpdate(&game, inputStatus);
					
					break;

				case GAME_PAUSED:

					if (isGameplayMusicPlaying) // Stops gameplay music when paused
					{
						gameplayMusicPosition = stopGameplayMusic();
						isGameplayMusicPlaying = false;
					}

					if (isMothershipSoundPlaying)
					{
						stopMothershipSound();
						isMothershipSoundPlaying = false;
					}

					renderMenu(game);

					if (inputStatus.resumeKeyPressed)
					{
						// Resumes game
						resumeGameplayMusic(gameplayMusicPosition);
						isGameplayMusicPlaying = true;
						gameResume(&game);
					}
					else if (inputStatus.restartKeyPressed)
					{
						// Restarts game
						currentPoints = 0;
						currentLives = SHIP_INITIAL_LIVES;
						gameReset(&game);
					}
					else if (inputStatus.exitKeyPressed)
					{
						programRunning = false;
					}

					break;

				case GAME_END:

					if (isGameplayMusicPlaying)
					{
						gameplayMusicPosition = stopGameplayMusic(); // Stops gameplay music when game ends
						isGameplayMusicPlaying = false;
					}

					if (isMothershipSoundPlaying)
					{
						stopMothershipSound();
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
						gameoverSoundPlayed = false;
					}
					else if (inputStatus.exitKeyPressed)
					{
						programRunning = false;
					}

					renderGameOver(game);

					break;
				default:
					programRunning = false;
					printf("Error: Invalid game status.\n");
					break;
				}
			}
			else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				setInput(&inputStatus, ev.keyboard.keycode);

				// Manages if the player pauses or exits the game
				if (inputStatus.pauseKeyPressed && game.status == GAME_RUNNING)
					game.status = GAME_PAUSED;
				else if (inputStatus.exitKeyPressed)
					programRunning = false;
				else if (game.status == GAME_MENU)
					setUserName(&game, ev.keyboard.keycode);
			}
			else if (ev.type == ALLEGRO_EVENT_KEY_UP)
			{
				clearInput(&inputStatus, ev.keyboard.keycode);
			}
			else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				programRunning = false;
			}
		}
	}

	// Cleanup allegro when program ends
	cleanupGraphics();
	cleanupAudio();
	printf("\nProgram finished.\n\n");
	return 0;
}
