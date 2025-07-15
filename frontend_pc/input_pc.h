/***************************************************************************//**
	@input_pc.h
    @Management of user inputs
    @Grupo_5
 ******************************************************************************/

#ifndef INPUT_PC_H
#define INPUT_PC_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "../backend/game.h"

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
 * @brief function to set the user name
 * @param game pointer to the game
 * @param keycode the key the user enter 
*/
void setUserName(game_t *game, int keycode);

/**
 * @brief function to set input when a key goes down
 * @param inputStatus pointer to the input status general struct
 * @param keycode the key to update 
*/
void setInput(inputStatus_t * inputStatus, int keycode);

/**
 * @brief function to clear input when a key goes up
 * @param inputStatus pointer to the input status general struct
 * @param keycode the key to update 
*/
void clearInput(inputStatus_t *inputStatus, int keycode);


/*******************************************************************************
 ******************************************************************************/

#endif // INPUT_PC_H
