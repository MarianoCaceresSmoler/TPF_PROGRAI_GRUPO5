
/***************************************************************************//**
  @render_pc.c
  @Render frontend
  @Grupo_5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
// +Incluir el header propio (ej: #include "template.h")+

#include "../backend/config.h"
#include "render_pc.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define SCALE_X (SCREEN_WIDTH / LOGICAL_WIDTH)
#define SCALE_Y (SCREEN_HEIGHT / LOGICAL_HEIGHT)

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/



/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static void falta_envido (int);+

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static int temperaturas_actuales[4];+

static ALLEGRO_DISPLAY *display = NULL;
static ALLEGRO_FONT *font = NULL;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


int get_random_number(int min, int max) 
{
    srand(time(NULL)); // Seed the random number generator with the current time
    return rand() % (max - min + 1) + min;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



// render_pc.c
#include <stdio.h>
#include "config.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

int scale_x(int logic_x) {
    return logic_x * SCREEN_WIDTH / LOGICAL_WIDTH;
}

int scale_y(int logic_y) {
    return logic_y * SCREEN_HEIGHT / LOGICAL_HEIGHT;
}

// Demo
int main() {
    int logic_x = 10;
    int logic_y = 5;

    int px = scale_x(logic_x);
    int py = scale_y(logic_y);

    printf("Allegro: lógica (%d, %d) → píxel (%d, %d)\n", logic_x, logic_y, px, py);

    return 0;
}
