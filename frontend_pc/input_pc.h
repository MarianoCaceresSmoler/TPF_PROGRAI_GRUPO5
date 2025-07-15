/***************************************************************************//**
  @file 	input_pc.h
  @brief 	Management of user inputs
  @author 	Grupo_5
 ******************************************************************************/

#ifndef INPUT_PC_H
#define INPUT_PC_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "../backend/game.h"

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
