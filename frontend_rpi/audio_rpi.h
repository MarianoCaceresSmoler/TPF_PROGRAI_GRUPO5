/***************************************************************************//**
  @file 	  audio_rpi.h
  @brief    Control of music and sound for RPI
  @author 	Grupo_5
 ******************************************************************************/

#ifndef AUDIO_RPI_H
#define AUDIO_RPI_H

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief function to initialize the audio
 * @return 0 if the initialization was successful, -1 otherwise
 */
int initializeAudio(void);

/**
 * @brief function to end and free the audio
 */
void cleanupAudio(void);

/**
 * @brief function to play explosion sound
 */
void playExplosionSound(void);

/**
 * @brief function to play power up sound
 */
void playPowerUpSound(void);

/**
 * @brief function to play mothership sound
 */
void playMothershipSound(void);

/**
 * @brief function to play ship shooting sound
 */
void playShootSound(void);

/**
 * @brief function to play game over sound
 */
void playGameoverSound(void);

/**
 * @brief function to play gameplay music
 */
void playGameMusic(void);

/**
 * @brief function to stop playing gameplay music
 */
void stopMusic(void);

/**
 * @brief function to resume gameplay music
 */
void resumeMusic(void);

/*******************************************************************************
 ******************************************************************************/

#endif // AUDIO_RPI_H
