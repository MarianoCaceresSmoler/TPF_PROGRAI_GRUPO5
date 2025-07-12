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
*/
void initAudio(void);

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
void playShipDiedSound(void);
void playGameoverSound(void);
void playMenuMusic(void);
void playGameplayMusic(void);
void stopMenuMusic(void);
unsigned int stopGameplayMusic(void);
void stopMothershipSound(void);
void resumeGameplayMusic(unsigned int position);

/*******************************************************************************
 ******************************************************************************/

#endif // AUDIO_PC_H
