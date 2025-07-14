/***************************************************************************//**
	@audio_pc.h
    @Control of music and sound effects
    @Grupo_5
 ******************************************************************************/

#ifndef AUDIO_PC_H
#define AUDIO_PC_H

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
 * @brief function to initialize the allegro audio
 * @return 0 if the initialization was successful, -1 otherwise
*/
int initAudio(void);

/**
 * @brief function to free allegro audio
*/
void cleanupAudio(void);

/**
 * @brief functions to reproduce sounds and music
*/
void playShootSound(void);
void playExplosionSound(void);
void playMothershipSound(void);
void stopMothershipSound(void);
void playPowerUpSound(void);
void playShipDiedSound(void);
void playGameoverSound(void);
void playMenuMusic(void);
void stopMenuMusic(void);
void playGameplayMusic(void);
unsigned int stopGameplayMusic(void); // returns the position in which the music was stopped
void resumeGameplayMusic(unsigned int position); // param position is the moment where the music was stopped, used to reproduce it from there

/*******************************************************************************
 ******************************************************************************/

#endif // AUDIO_PC_H
