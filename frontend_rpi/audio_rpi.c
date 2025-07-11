
/***************************************************************************/ /**
	@audio_rpi.h
	@Control of music and sound effects
	@Grupo_5
  ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
// +Incluir el header propio (ej: #include "template.h")+

#include <stdio.h>
#include <SDL2/SDL.h>

#include "audio.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define EXPLOSION "frontend_rpi/assets/audio/explosion.wav"
#define GAME_OVER "frontend_rpi/assets/audio/gameover.wav"
#define MOTHERSHIP "frontend_rpi/assets/audio/mothership.wav"
#define SHOOT "frontend_rpi/assets/audio/shoot.wav"
#define GAME_MUSIC "frontend_rpi/assets/audio/gamemusic.wav"

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

static Audio *explosion = NULL;
static Audio *mothership = NULL;
static Audio *shoot = NULL;
static Audio *gameOver = NULL;
static Audio *gameMusic = NULL;

/*******************************************************************************
 *******************************************************************************
						GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void initializeAudio(void)
{
	// Init Simple-SDL2-Audio
	if (initAudio() == NO_INIT)
	{
		fprintf(stderr, "Audio not initilized.\n");
		endAudio();
	}

	// Create sound audios
	explosion = createAudio(EXPLOSION, 0, SDL_MIX_MAXVOLUME / 2);
	mothership = createAudio(MOTHERSHIP, 0, SDL_MIX_MAXVOLUME / 2);
	shoot = createAudio(SHOOT, 0, SDL_MIX_MAXVOLUME / 2);
	gameOver = createAudio(GAME_OVER, 0, SDL_MIX_MAXVOLUME / 2);

	// Create music audios
	gameMusic = createAudio(GAME_MUSIC, 1, SDL_MIX_MAXVOLUME);
}

void cleanupAudio(void)
{
	// End Simple-SDL2-Audio
	endAudio();

	// Important to free audio after ending Simple-SDL2-Audio because they might be referenced still
	if (explosion)
		freeAudio(explosion);
	if (mothership)
		freeAudio(mothership);
	if (shoot)
		freeAudio(shoot);
	if (gameOver)
		freeAudio(gameOver);
	if (gameMusic)
		freeAudio(gameMusic);
}

/*******************************************************************************
 *******************************************************************************
						LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void playExplosionSound(void)
{
	if (playerStatus() == READY && musicStatus() == PLAYING)
		playSoundFromMemory(explosion, SDL_MIX_MAXVOLUME);
}

void playMothershipSound(void)
{
	if (playerStatus() == READY && musicStatus() == PLAYING)
		playSoundFromMemory(mothership, SDL_MIX_MAXVOLUME);
}

void playShootSound(void)
{
	if (playerStatus() == READY && musicStatus() == PLAYING)
		playSoundFromMemory(shoot, SDL_MIX_MAXVOLUME);
}

void playGameoverSound(void)
{
	if (playerStatus() == READY && musicStatus() == PLAYING)
		playSoundFromMemory(gameOver, SDL_MIX_MAXVOLUME);
}

void playGameMusic(void)
{
	if (playerStatus() == READY && musicStatus() == PLAYING)
		playMusicFromMemory(gameMusic, SDL_MIX_MAXVOLUME);
}

void stopMusic(void)
{
	if (playerStatus() == READY && musicStatus() == PLAYING)
		pauseAudio();
}

void resumeMusic(void)
{
	if (playerStatus() == READY && musicStatus() == PAUSED)
		unpauseAudio();
}
