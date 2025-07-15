/***************************************************************************//**
  @file 	inputs_rpi.h
  @brief    Management of user inputs for RPI
  @author 	Grupo_5
 ******************************************************************************/

#ifndef INPUT_RPI_H
#define INPUT_RPI_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "../backend/game.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief function to init input system
 * @param inputStatus pointer to input status used in main
 * @return 0 if input init is successful, -1 otherwise
*/
int initInput(inputStatus_t *inputStatus);

/**
 * @brief function to finish inputs loop
*/
void cleanupInput(void);

/**
 * @brief function to reset input input flags
 * @param inputStatus pointer to input status used in main
*/
void resetInputFlags(inputStatus_t * inputStatus);


/*******************************************************************************
 ******************************************************************************/

#endif // INPUT_RPI_H
