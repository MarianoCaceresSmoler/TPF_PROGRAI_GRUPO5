
/***************************************************************************/ /**
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
#include <stdio.h>
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

/**
 * @brief private functions to draw objects in display
 */
static void drawShip(ship_t ship);
static void drawAliens(alienFormation_t alienFormation);
static void drawBullet(bullet_t bullet);
static void drawBarriers(barrier_t barriers[]);
static void drawScore(int score);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static int temperaturas_actuales[4];+

static ALLEGRO_DISPLAY *display = NULL;
static ALLEGRO_EVENT_QUEUE *event_queue = NULL;
static ALLEGRO_TIMER *timer = NULL;
static ALLEGRO_DISPLAY *display = NULL;
static ALLEGRO_FONT *fontGameplay = NULL;
static ALLEGRO_FONT *fontRetro = NULL;

static ALLEGRO_BITMAP *alien0BitMap = NULL;
static ALLEGRO_BITMAP *alien1BitMap = NULL;
static ALLEGRO_BITMAP *alien2BitMap = NULL;
static ALLEGRO_BITMAP *alien3BitMap = NULL;
static ALLEGRO_BITMAP *alien4BitMap = NULL;
static ALLEGRO_BITMAP *shipBitMap = NULL;
static ALLEGRO_BITMAP *barrierPixelBitmap = NULL;
static ALLEGRO_BITMAP *bulletBitmap = NULL;
static ALLEGRO_BITMAP *mothershipBitmap = NULL;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void initGraphics()
{
    if (!al_init())
    {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    if (!al_install_keyboard())
    {
        fprintf(stderr, "failed to initialize the keyboard!\n");
        return -1;
    }

    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    timer = al_create_timer(1.0 / FPS);

    alien0BitMap = al_create_bitmap(ALIEN_WIDTH, ALIEN_HEIGTH);
    alien1BitMap = al_create_bitmap(ALIEN_WIDTH, ALIEN_HEIGTH);
    alien2BitMap = al_create_bitmap(ALIEN_WIDTH, ALIEN_HEIGTH);
    alien3BitMap = al_create_bitmap(ALIEN_WIDTH, ALIEN_HEIGTH);
    alien4BitMap = al_create_bitmap(ALIEN_WIDTH, ALIEN_HEIGTH);

    shipBitMap = al_create_bitmap(SHIP_WIDTH, SHIP_HEIGHT);
    barrierPixelBitmap = al_create_bitmap(BARRIER_PIXEL_WIDTH, BARRIER_PIXEL_HEIGHT);
    bulletBitmap = al_create_bitmap(BULLET_WIDTH, BULLET_HEIGHT);
    mothershipBitmap = al_create_bitmap(MOTHERSHIP_WIDTH, MOTHERSHIP_HEIGHT);

    event_queue = al_create_event_queue();

    al_init_font_addon();
    al_init_ttf_addon();
    fontGameplay = al_load_ttf_font("assets/fonts/font_gameplay.ttf", 36, 0);
    fontRetro = al_load_ttf_font("assets/fonts/font_retro.ttf.ttf", 36, 0);

    if (!(display && timer && alien0BitMap && alien1BitMap && alien2BitMap && alien3BitMap && alien4BitMap && shipBitMap && barrierPixelBitmap && bulletBitmap && mothershipBitmap && event_queue))
    {
        fprintf(stderr, "Failed to load assets.\n");
        if (display)
            al_destroy_display(display);
        if (timer)
            al_destroy_timer(timer);
        if (alien0BitMap)
            al_destroy_bitmap(alien0BitMap);
        if (alien1BitMap)
            al_destroy_bitmap(alien1BitMap);
        if (alien2BitMap)
            al_destroy_bitmap(alien2BitMap);
        if (alien3BitMap)
            al_destroy_bitmap(alien3BitMap);
        if (alien4BitMap)
            al_destroy_bitmap(alien4BitMap);
        if (shipBitMap)
            al_destroy_bitmap(shipBitMap);
        if (barrierPixelBitmap)
            al_destroy_bitmap(barrierPixelBitmap);
        if (bulletBitmap)
            al_destroy_bitmap(bulletBitmap);
        if (mothershipBitmap)
            al_destroy_bitmap(mothershipBitmap);
        if (event_queue)
            al_destroy_event_queue(event_queue);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source()); 

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
    al_start_timer(timer);


    // FALTARIA INICIALIZAR Y DESTRUIR IMAGENES
}

void cleanupGraphics()
{
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_bitmap(alien0BitMap);
        al_destroy_bitmap(alien1BitMap);
        al_destroy_bitmap(alien2BitMap);
        al_destroy_bitmap(alien3BitMap);
        al_destroy_bitmap(alien4BitMap);
        al_destroy_bitmap(shipBitMap);
        al_destroy_bitmap(barrierPixelBitmap);
        al_destroy_bitmap(bulletBitmap);
        al_destroy_bitmap(mothershipBitmap);
        al_destroy_event_queue(event_queue);
}

/**
 * @brief function to render the game state
 * @param game copy of the game object
 */
void renderGame(game_t game);

/**
 * @brief function to render the menu
 * @param game copy of the game object
 */
void renderMenu(game_t game);

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/**
 * @brief private functions to draw objects in display
 */
static void drawShip(ship_t ship);
static void drawAliens(alienFormation_t alienFormation);
static void drawBullet(bullet_t bullet);
static void drawBarriers(barrier_t barriers[]);
static void drawScore(int score);

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

int scale_x(int logic_x)
{
    return logic_x * SCREEN_WIDTH / LOGICAL_WIDTH;
}

int scale_y(int logic_y)
{
    return logic_y * SCREEN_HEIGHT / LOGICAL_HEIGHT;
}

// Demo
int main()
{
    int logic_x = 10;
    int logic_y = 5;

    int px = scale_x(logic_x);
    int py = scale_y(logic_y);

    printf("Allegro: lógica (%d, %d) → píxel (%d, %d)\n", logic_x, logic_y, px, py);

    return 0;
}
