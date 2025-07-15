/***************************************************************************//**
  @file 	audio_rpi.c
  @brief    Control of music and sound for RPI
  @author 	Grupo_5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdio.h>
#include <SDL2/SDL.h>

#include "audio.h"
#include "audio_rpi.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// Paths to the sound effects and music files
#define EXPLOSION "frontend_rpi/assets/audio/explosion.wav"
#define POWER_UP "frontend_rpi/assets/audio/powerup.wav"
#define GAME_OVER "frontend_rpi/assets/audio/gameover.wav"
#define MOTHERSHIP "frontend_rpi/assets/audio/mothership.wav"
#define SHOOT "frontend_rpi/assets/audio/shoot.wav"
#define GAME_MUSIC "frontend_rpi/assets/audio/gamemusic.wav"

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// Audio variables
static Audio *explosion = NULL;
static Audio *powerUp = NULL;
static Audio *mothership = NULL;
static Audio *shoot = NULL;
static Audio *gameOver = NULL;
static Audio *gameMusic = NULL;

/*******************************************************************************
 *******************************************************************************
						GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

int initializeAudio(void)
{
	// Init Simple-SDL2-Audio
	if (initAudio() == NO_INIT)
	{
		printf("Audio not initilized.\n");
		endAudio();
		return -1;
	}

	// Create sound audios
	explosion = createAudio(EXPLOSION, 0, SDL_MIX_MAXVOLUME / 2);
	if(!explosion)
		printf("Error creating explosion audio.\n");
	powerUp = createAudio(POWER_UP, 0, SDL_MIX_MAXVOLUME / 2);
	if(!powerUp)
		printf("Error creating powerUp audio.\n");	
	mothership = createAudio(MOTHERSHIP, 0, SDL_MIX_MAXVOLUME);
	if(!mothership)
		printf("Error creating mothership audio.\n");
	shoot = createAudio(SHOOT, 0, SDL_MIX_MAXVOLUME / 2);
	if(!shoot)
		printf("Error creating shoot audio.\n");
	gameOver = createAudio(GAME_OVER, 0, SDL_MIX_MAXVOLUME / 2);
	if(!gameOver)
		printf("Error creating gameOver audio.\n");

	// Create music audio
	gameMusic = createAudio(GAME_MUSIC, 1, SDL_MIX_MAXVOLUME);
	if(!gameMusic)
		printf("Error creating gameMusic audio.\n");

	// If there is an error creating audios, end audio and return -1
	if(!explosion || !powerUp || !mothership || !shoot || !gameOver || !gameMusic)
	{
		cleanupAudio();
		return -1;
	}

	return 0;
}

void cleanupAudio(void)
{
	// End Simple-SDL2-Audio
	endAudio();

	// Important to free audio after ending Simple-SDL2-Audio because they might be referenced still
	if (explosion)
		freeAudio(explosion);
	if (powerUp)
		freeAudio(powerUp);
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
		playSoundFromMemory(explosion, SDL_MIX_MAXVOLUME); // If the player is playing music, play explosion sound 
}

void playPowerUpSound(void)
{
	if (playerStatus() == READY && musicStatus() == PLAYING)
		playSoundFromMemory(powerUp, SDL_MIX_MAXVOLUME); // If the player is playing music, play power up sound
}

void playMothershipSound(void)
{
	if (playerStatus() == READY && musicStatus() == PLAYING)
		playSoundFromMemory(mothership, SDL_MIX_MAXVOLUME); // If the player is playing music, play mothership sound
}

void playShootSound(void)
{
	if (playerStatus() == READY && musicStatus() == PLAYING)
		playSoundFromMemory(shoot, SDL_MIX_MAXVOLUME); // If the player is playing music, play shoot sound
}

void playGameoverSound(void)
{
	if (playerStatus() == READY && musicStatus() == PLAYING)
		playSoundFromMemory(gameOver, SDL_MIX_MAXVOLUME); // If the player is playing music, play game over sound
}

void playGameMusic(void)
{
	if (playerStatus() == READY && musicStatus() == PLAYING)
		playMusicFromMemory(gameMusic, SDL_MIX_MAXVOLUME); // If the player is playing music, play game music
}

void stopMusic(void)
{
	if (playerStatus() == READY && musicStatus() == PLAYING)
		pauseAudio(); // If the player is playing music, stop it
}

void resumeMusic(void)
{
	if (playerStatus() == READY && musicStatus() == PAUSED)
		unpauseAudio(); // If the music is paused, resume it
}
