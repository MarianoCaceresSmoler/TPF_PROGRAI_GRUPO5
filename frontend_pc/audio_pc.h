/***************************************************************************//**
  @file 	audio_pc.h
  @brief 	Audio and sound functions for allegro
  @author 	Grupo_5
 ******************************************************************************/

#ifndef AUDIO_PC_H
#define AUDIO_PC_H

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief function to initialize the allegro audio
 * @return 0 if the initialization was successful, -1 otherwise
*/
int initAudio(void);

/**
 * @brief function to free allegro audio
*/
void cleanupAudio(void);

/**
 * @brief function to play the ship shooting sound
*/
void playShootSound(void);

/**
 * @brief function to play the entity explosion sound
*/
void playExplosionSound(void);

/**
 * @brief function to play the mothership sound
*/
void playMothershipSound(void);

/**
 * @brief function to stop playing mothership sound
*/
void stopMothershipSound(void);

/**
 * @brief function to play power up sound
*/
void playPowerUpSound(void);

/**
 * @brief function to play ship explosion sound
*/
void playShipDiedSound(void);

/**
 * @brief function to play game over sound
*/
void playGameoverSound(void);

/**
 * @brief function to start playing menu music
*/
void playMenuMusic(void);

/**
 * @brief function to stop playing menu music
*/
void stopMenuMusic(void);

/**
 * @brief function to start playing gameplay music from the beginning
*/
void playGameplayMusic(void);

/**
 * @brief function to stop playing gameplay music
 * @return position in which the music was stopped
*/
unsigned int stopGameplayMusic(void);

/**
 * @brief functions to resume gameplay music from a certain position
 * @param position is the moment where the music will begin playing
*/
void resumeGameplayMusic(unsigned int position);

/*******************************************************************************
 ******************************************************************************/

#endif // AUDIO_PC_H
