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

	initGraphics();
	initAudio(); // HACER

	game_t game;
	game_t *pgame = &game; // pointer to game variable
	gameInit(pgame);	   // Initialize the game

	bool_t running = TRUE;
	ALLEGRO_EVENT ev;
	input_t currentInput = INPUT_NONE;

	while (running)
	{
		al_wait_for_event(getEventQueue(), &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) // Draws updated game
		{
			switch (game.status) // Shows different game states and gets different inputs (1 time per frame)
			{				
			case GAME_MENU:
				renderMenu(game);
				break;

			case GAME_RUNNING:
				gameUpdate(pgame, currentInput);
				renderGame(game);
				break;

			case GAME_PAUSED:
				renderMenu(game);
				break;
			case GAME_END:
				renderGameOver(game);
				break;
			}

			currentInput = INPUT_NONE; // Reset input after 1 frame
			
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			currentInput = getInputFromKeycode(ev.keyboard.keycode);

			switch (game.status)
			{
			case GAME_MENU:
				gameReset(pgame);
				break;

			case GAME_RUNNING:
				if (currentInput == INPUT_PAUSE)
					gamePause(pgame);
				break;

			case GAME_PAUSED:
				if (currentInput == INPUT_RESUME)
					gameResume(pgame);
				else if (currentInput == INPUT_RESTART)
					gameReset(pgame);
				else if (currentInput == INPUT_EXIT)
				{
					gameEnd(pgame);
					running = FALSE;
				}

				break;

			case GAME_END:
				if (currentInput == INPUT_RESTART)
					gameReset(pgame);
				else if (currentInput == INPUT_EXIT)
				{
					gameEnd(pgame);
					running = FALSE;
				}
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			gameEnd(pgame);
			running = FALSE;
		}
	}

	cleanupGraphics(); // cleanup allegro when program ends
	return 0;
}
