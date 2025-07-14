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

	// Initialize audio and check for errors
	if(initAudio())
	{
		printf("Error initializing audio\n");
		return -1;
	}
	
	// Initialize graphics and check for errors
	if(initGraphics())
	{
		printf("Error initializing graphics\n");
		return -1;
	}

	// Main loop control and input handling setup
	bool programRunning = true;
	ALLEGRO_EVENT ev;
	inputStatus_t inputStatus = {false, false, false, false, false, false, false, false};

	// Audio state variables
	bool isMenuMusicPlaying = false;
	bool isGameplayMusicPlaying = false;
	bool isMothershipSoundPlaying = false;
	bool gameoverSoundPlayed = false;
	unsigned int gameplayMusicPosition = 0;
	// Game state tracking
	int currentPoints = 0;
	int currentLives = SHIP_INITIAL_LIVES;
	int currentPowerUps[POWERUP_TYPES] = {0, 0, 0, 0};
	int i;

	// Main game loop
	while (programRunning)
	{
		// Process incoming events
		if (al_get_next_event(getEventQueue(), &ev))
		{
			// Timer event triggers game logic and rendering
			if (ev.type == ALLEGRO_EVENT_TIMER)
			{
				switch (game.status)
				{
				case GAME_MENU:

					if (!isMenuMusicPlaying) // Start menu music if not already playing
					{
						playMenuMusic();
						isMenuMusicPlaying = true;
					}
 
					// Render main menu
					renderMenu(game);

					// If resume key pressed, switch to gameplay
					if (inputStatus.resumeKeyPressed)
					{						
						stopMenuMusic();
						isMenuMusicPlaying = false;
						levelInit(&game);
					}

					break;

				case GAME_LOADING:

					// Show loading screen
					renderGame(game);

					// Countdown before starting gameplay
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

					// If score or lives changed, play explosion sound and update tracked values
					if (currentPoints != game.score || currentLives > game.ship.livesLeft)
					{
						playExplosionSound();
						currentPoints = game.score;
						currentLives = game.ship.livesLeft;
					}

					// Resume gameplay music if previously paused
					if (!isGameplayMusicPlaying)
					{
						resumeGameplayMusic(gameplayMusicPosition);
						isGameplayMusicPlaying = true;
					}

					// Check for newly collected power-ups and play powerup sound
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

					// Play shoot sound if the shoot key is pressed
					if (inputStatus.shootKeyPressed && !game.shipBullet.entity.isAlive)
						playShootSound();
					
					// Play mothership sound when it appears
					if (game.mothership.entity.isAlive && !isMothershipSoundPlaying)
					{
						playMothershipSound();
						isMothershipSoundPlaying = true;
					}
					else if (!game.mothership.entity.isAlive && isMothershipSoundPlaying)
					{
						stopMothershipSound();
						isMothershipSoundPlaying = false;
					}

					// Update and render game frame
					renderGame(game);
					gameUpdate(&game, inputStatus);
					
					break;

				case GAME_PAUSED:

					// Stop gameplay and mothership music
					if (isGameplayMusicPlaying)
					{
						gameplayMusicPosition = stopGameplayMusic();
						isGameplayMusicPlaying = false;
					}
					if (isMothershipSoundPlaying)
					{
						stopMothershipSound();
						isMothershipSoundPlaying = false;
					}

					// Render pause menu
					renderMenu(game);

					// Manage user inputs in pause menu
					if (inputStatus.resumeKeyPressed)
					{
						// Resume game
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
						// Exit game
						programRunning = false;
					}

					break;

				case GAME_END:

					// Stop gameplay and mothership sounds
					if (isGameplayMusicPlaying)
					{
						gameplayMusicPosition = stopGameplayMusic();
						isGameplayMusicPlaying = false;
					}
					if (isMothershipSoundPlaying)
					{
						stopMothershipSound();
						isMothershipSoundPlaying = false;
					}

					// Play game over sound once
					if (!gameoverSoundPlayed)
					{
						playGameoverSound();
						gameoverSoundPlayed = true;
					}

					// Manage input in game over screen
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

					// Show game over screen
					renderGameOver(game);

					break;
				case GAME_ERROR:
					// If game status was set to GAME_ERROR, it means the game needs to end
					programRunning = false;
					printf("Game status ERROR\n");
					return -1;
					break;
				default:
					// Handle unexpected state
					programRunning = false;
					printf("Error: Invalid game status.\n");
					return -1;
					break;
				}
			}
			else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				// Update input status when key is pressed
				setInput(&inputStatus, ev.keyboard.keycode);

				// Allow player to pause or quit game
				if (inputStatus.pauseKeyPressed && game.status == GAME_RUNNING)
					game.status = GAME_PAUSED;
				else if (inputStatus.exitKeyPressed)
					programRunning = false;
				else if (game.status == GAME_MENU)
					setUserName(&game, ev.keyboard.keycode);
				
			}
			else if (ev.type == ALLEGRO_EVENT_KEY_UP)
			{
				// Clear input status when key is released
				clearInput(&inputStatus, ev.keyboard.keycode);
			}
			else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				// Handle window close
				programRunning = false;
			}
		}
	}

	// Cleanup Allegro resources before exiting
	cleanupGraphics();
	cleanupAudio();
	printf("\nProgram finished.\n\n");
	return 0;
}

