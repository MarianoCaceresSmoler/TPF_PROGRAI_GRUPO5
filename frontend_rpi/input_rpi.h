/***************************************************************************//**
	@input_rpi.h
    @Management of user inputs
    @Grupo_5
 ******************************************************************************/

#ifndef INPUT_RPI_H
#define INPUT_RPI_H

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

 // To control the movement of the ship
// typedef struct {
//     int leftPressed;
//     int rightPressed;
// } input_state_t;

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
 * @brief function to init input system
 * @param pointer to input status used in main
*/
void initInput(inputStatus_t *inputStatus);

/**
 * @brief function to finish inputs loop
*/
void clearInput(void);

/*******************************************************************************
 ******************************************************************************/

#endif // INPUT_RPI_H
