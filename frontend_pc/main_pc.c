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

#define TRUE 1
#define FALSE 0

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef unsigned short int bool_t;

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

static input_state_t inputState = {FALSE, FALSE};

/*******************************************************************************
 *******************************************************************************
						GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

int getRandomNumber(int min, int max)
{
	srand(time(NULL)); // Seed the random number generator with the current time
	return rand() % (max - min + 1) + min;
}

/*******************************************************************************
 *******************************************************************************
						LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

int main(void)
{

	// Init allegro
	initGraphics();
	initAudio();

	game_t game;
	game_t *pgame = &game; // Pointer to game variable
	gameInit(pgame);	   // Initialize the game

	// Allegro variables to manage the game loop and the inputs
	bool_t running = TRUE;
	ALLEGRO_EVENT ev;
	input_t currentInput = INPUT_NONE, releasedInput = INPUT_NONE;

	// Variables to manage sounds
	int currentPoints = 0, mothershipFlag = FALSE, shipDiedFlag = FALSE;

	while (running)
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
			if(game.mothership.entity.explosionTimer > 0 && !mothershipFlag)
			{	
				playMothershipSound();
				mothershipFlag = TRUE;
			}
			else if(!game.mothership.entity.isAlive && mothershipFlag)
				mothershipFlag = 0;

			// Check if the ship has died to reproduce larger explosion sound
			if(game.ship.entity.explosionTimer > 0 && !shipDiedFlag)
			{
				playShipDiedSound();
				shipDiedFlag = TRUE;
			}
			else if(!game.ship.entity.isAlive && shipDiedFlag)
				shipDiedFlag = FALSE;


			switch (game.status)
			{
			case GAME_MENU:
				playMenuMusic(); // Play the menu music when program starts
				renderMenu(game);
				break;

			case GAME_RUNNING:			
				if (inputState.leftPressed)
					manageInput(pgame, INPUT_LEFT);
				if (inputState.rightPressed)
					manageInput(pgame, INPUT_RIGHT);

				renderGame(game);
				break;

			case GAME_PAUSED:				
				stopGameplayMusic(); // Stop the music when the game is paused
				renderMenu(game);
				break;
			case GAME_END:
				stopGameplayMusic(); // Stop the music when the game ends
				playGameoverSound();
				renderGameOver(game);
				break;
			default:
				stopGameplayMusic();
				gameEnd(pgame);
				running = FALSE;
				break;
			}

			currentInput = INPUT_NONE; // Reset input after 1 frame
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) // Gets input from keyboard
		{
			currentInput = getInputFromKeycode(ev.keyboard.keycode);

			// Update flags of pressed keys
			if (currentInput == INPUT_LEFT)
				inputState.leftPressed = TRUE;
			else if (currentInput == INPUT_RIGHT)
				inputState.rightPressed = TRUE;

			switch (game.status)
			{
			case GAME_MENU:
				stopMenuMusic(); // Stop the menu music when game starts
				playGameplayMusic(); // Play the gameplay music when game starts

				// VER QUE FUNCIONES LLAMAR DE BACK PARA PASAR A GAME RUNNING		
				gameReset(pgame);
				break;

			case GAME_RUNNING:
				if (currentInput == INPUT_PAUSE)
					gamePause(pgame);
				else if(currentInput == INPUT_SHOOT)
					playShootSound(); // Play the shoot sound when the player shoots

				manageInput(pgame, currentInput);
				break;

			case GAME_PAUSED:
				if (currentInput == INPUT_RESUME)
				{
					playGameplayMusic(); // Play the gameplay music when game resumes
					gameResume(pgame);
				}
				else if (currentInput == INPUT_RESTART)
				{
					playGameplayMusic(); // Play the gameplay music when game restarts
					gameReset(pgame);
				}
				else if (currentInput == INPUT_EXIT)
				{
					gameEnd(pgame);
					running = FALSE;
				}

				break;

			case GAME_END:
				if (currentInput == INPUT_RESTART)
				{
					playGameplayMusic(); // Play the gameplay music when game restarts
					gameReset(pgame);
				}
				else if (currentInput == INPUT_EXIT)
				{
					gameEnd(pgame);
					running = FALSE;
				}
				break;

			default:
				gameEnd(pgame);
				running = FALSE;
				break;
			}
		}

		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			releasedInput = getInputFromKeycode(ev.keyboard.keycode);

			// Update flags of released keys
			if (releasedInput == INPUT_LEFT)
				inputState.leftPressed = FALSE;
			else if (releasedInput == INPUT_RIGHT)
				inputState.rightPressed = FALSE;
		}

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			gameEnd(pgame);
			running = FALSE;
		}
	}

	// Cleanup allegro when program ends
	cleanupGraphics();
	cleanupAudio();
	return 0;
}
