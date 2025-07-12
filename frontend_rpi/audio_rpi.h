/***************************************************************************/ /**
     @audio_rpi.h
     @Control of music and sound effects
     @Grupo_5
  ******************************************************************************/

#ifndef AUDIO_RPI_H
#define AUDIO_RPI_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: extern unsigned int anio_actual;+

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief completar descripcion
 * @param param1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
 * @return Descripcion valor que devuelve
 */
// +ej: char lcd_goto (int fil, int col);+

/**
 * @brief function to initialize the audio
 */
void initializeAudio(void);

/**
 * @brief function to end and free the audio
 */
void cleanupAudio(void);

/**
 * @brief functions to reproduce sounds and music
 */
void playExplosionSound(void);
void playMothershipSound(void);
void playShootSound(void);
void playGameoverSound(void);
void playGameMusic(void);
void stopMusic(void);
void resumeMusic(void);

/*******************************************************************************
 ******************************************************************************/

#endif // AUDIO_RPI_H
