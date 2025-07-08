
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
#include "../backend/entities.h"
#include "render_pc.h"
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
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
 * @brief private function to load sprites
 */
static void loadImages(void);

/**
 * @brief private functions to draw objects in display
 */
static void drawShip(ship_t ship);
static void drawAliens(alienFormation_t alienFormation);
static void drawMothership(mothership_t mothership);
static void drawBullets(bullet_t shipBullet, bullet_t alienBullet);
static void drawBarriers(barrier_t barriers[BARRIERS]);
static void drawScore(int score);

/**
 * @brief private function to draw aliens/mothership points in menu
 */
static void drawAliensPoints();

/**
 * @brief private function to draw HUD during gameplay
 * @param score the actual score
 * @param lives the lives left
 * @param level the actual level
 */
static void drawHUD(int score, int lives, int level);


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static int temperaturas_actuales[4];+

static ALLEGRO_DISPLAY *display = NULL;
static ALLEGRO_EVENT_QUEUE *eventQueue = NULL;
static ALLEGRO_TIMER *timer = NULL;

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
static ALLEGRO_BITMAP *explosionBitmap = NULL;
static ALLEGRO_BITMAP *titleBitmap = NULL;

/*******************************************************************************
 *******************************************************************************
						GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void initGraphics(void)
{
	// Initialize Allegro
	if (!al_init())
	{
		fprintf(stderr, "Failed to initialize allegro!\n");
	}

	if (!al_install_keyboard())
	{
		fprintf(stderr, "Failed to initialize the keyboard!\n");
	}

	// al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	if(!display)
	{
		fprintf(stderr, "Unable to create display \n");
	}

	timer = al_create_timer(1.0 / FPS);
	if(!timer) // 1 frame per second timer
	{
		fprintf(stderr, "Unable to create timer \n");
	}

	// Initialize allegro images controller
	if (!al_init_image_addon())
	{
		fprintf(stderr, "Unable to start image addon \n");
	}

	// Initialize allegro primitives addon
	if (!al_init_primitives_addon())
	{
		fprintf(stderr, "Unable to primitives addon \n");
	}

	// Create bitmaps for objects
	alien0BitMap = al_create_bitmap(ALIEN_WIDTH, ALIEN_HEIGHT);
	alien1BitMap = al_create_bitmap(ALIEN_WIDTH, ALIEN_HEIGHT);
	alien2BitMap = al_create_bitmap(ALIEN_WIDTH, ALIEN_HEIGHT);
	alien3BitMap = al_create_bitmap(ALIEN_WIDTH, ALIEN_HEIGHT);
	alien4BitMap = al_create_bitmap(ALIEN_WIDTH, ALIEN_HEIGHT);

	shipBitMap = al_create_bitmap(SHIP_WIDTH, SHIP_HEIGHT);
	barrierPixelBitmap = al_create_bitmap(BARRIER_PIXEL_WIDTH, BARRIER_PIXEL_HEIGHT);
	bulletBitmap = al_create_bitmap(BULLET_WIDTH, BULLET_HEIGHT);
	mothershipBitmap = al_create_bitmap(MOTHERSHIP_WIDTH, MOTHERSHIP_HEIGHT);
	explosionBitmap = al_create_bitmap(EXPLOSION_WIDTH, EXPLOSION_HEIGHT);
	titleBitmap = al_create_bitmap(TITLE_WIDTH, TITLE_HEIGHT);

	// Create event qeue
	eventQueue = al_create_event_queue();

	// Register fonts
	al_init_font_addon();
	al_init_ttf_addon();
	fontGameplay = al_load_ttf_font("frontend_pc/assets/fonts/font_gameplay.ttf", FONT_GAME_SIZE, 0);
	fontRetro = al_load_ttf_font("frontend_pc/assets/fonts/font_retro.ttf", FONT_TEXT_SIZE, 0);
	
	// Error managing for elements created
	if(!(display && timer && alien0BitMap && alien1BitMap && alien2BitMap && alien3BitMap && alien4BitMap && shipBitMap && barrierPixelBitmap && bulletBitmap && mothershipBitmap && eventQueue))
	{
		fprintf(stderr, "Failed to load assets.\n");
		cleanupGraphics();
	}

	// Calls private function to load sprites
	loadImages();

	// Registers the event sources
	al_register_event_source(eventQueue, al_get_display_event_source(display));
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));
	al_register_event_source(eventQueue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();
	al_start_timer(timer);
}

void cleanupGraphics(void)
{
	// Destroys all elements at the end of the program
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
	if (explosionBitmap)
		al_destroy_bitmap(explosionBitmap);
	if (titleBitmap)
		al_destroy_bitmap(titleBitmap);
	if (eventQueue)
		al_destroy_event_queue(eventQueue);
	if (fontGameplay)
		al_destroy_font(fontGameplay);
	if (fontRetro)
		al_destroy_font(fontRetro);
}

void renderGame(game_t game)
{
	al_clear_to_color(al_map_rgb(0, 0, 0));

	drawShip(game.ship);
	drawAliens(game.aliens);
	drawBullets(game.shipBullet, game.alienBullet);
	drawBarriers(game.barriers);
	drawMothership(game.mothership);
	drawHUD(game.score, game.ship.livesLeft, game.currentLevel);

	al_flip_display();
}

void renderMenu(game_t game)
{
	gameStatus_t status = game.status;

	al_clear_to_color(al_map_rgb(0, 0, 0));

	if (status == GAME_MENU) // If player is at menu
	{
		int titleWidth = al_get_bitmap_width(titleBitmap);
		int titleHeight = al_get_bitmap_height(titleBitmap);

		float destX = (SCREEN_WIDTH - titleWidth) / 2;
		float destY = SCREEN_HEIGHT / 11; // Relative height

		// Draws title
		al_draw_scaled_bitmap(
			titleBitmap,
			0, 0,
			titleWidth, titleHeight,
			destX, destY,
			titleWidth, titleHeight,
			0);

		al_draw_text(fontRetro, al_map_rgb(200, 200, 200), SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.85, ALLEGRO_ALIGN_CENTER, "Press SPACE to play");

		drawAliensPoints();
	}
	else if (status == GAME_PAUSED) // if game was paused
	{
		al_draw_text(fontRetro, al_map_rgb(255, 255, 0),
					 SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3,
					 ALLEGRO_ALIGN_CENTER, "GAME PAUSED");

		al_draw_text(fontRetro, al_map_rgb(200, 200, 200),
					 SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,
					 ALLEGRO_ALIGN_CENTER, "Press ENTER to resume, ESC to quit, R to restart the game");
	}

	al_flip_display();
}

void renderGameOver(game_t game)
{
	int score = game.score;

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_draw_text(fontRetro, al_map_rgb(255, 0, 0),
				 SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3,
				 ALLEGRO_ALIGN_CENTER, "GAME OVER");

	char scoreText[64];
	snprintf(scoreText, sizeof(scoreText), "Final Score: %d", score);
	al_draw_text(fontRetro, al_map_rgb(255, 255, 255),
				 SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50,
				 ALLEGRO_ALIGN_CENTER, scoreText);

	al_draw_text(fontRetro, al_map_rgb(200, 200, 200),
				 SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 150,
				 ALLEGRO_ALIGN_CENTER, "Enter ESC to quit, R to restart the game");

	al_flip_display();
}

ALLEGRO_EVENT_QUEUE *getEventQueue(void)
{
	return eventQueue;
}

/*******************************************************************************
 *******************************************************************************
						LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void loadImages(void)
{
	// Load the images

	alien0BitMap = al_load_bitmap("frontend_pc/assets/img/alien0.png");
	if (!alien0BitMap)
	{
		fprintf(stderr, "Failed to load alien0.png\n");
	}

	alien1BitMap = al_load_bitmap("frontend_pc/assets/img/alien1.png");
	if (!alien1BitMap)
	{
		fprintf(stderr, "Failed to load alien1.png\n");
	}

	alien2BitMap = al_load_bitmap("frontend_pc/assets/img/alien2.png");
	if (!alien2BitMap)
	{
		fprintf(stderr, "Failed to load alien2.png\n");
	}

	alien3BitMap = al_load_bitmap("frontend_pc/assets/img/alien3.png");
	if (!alien3BitMap)
	{
		fprintf(stderr, "Failed to load alien3.png\n");
	}

	alien4BitMap = al_load_bitmap("frontend_pc/assets/img/alien4.png");
	if (!alien4BitMap)
	{
		fprintf(stderr, "Failed to load alien4.png\n");
	}

	shipBitMap = al_load_bitmap("frontend_pc/assets/img/ship.png");
	if (!shipBitMap)
	{
		fprintf(stderr, "Failed to load ship.png\n");
	}

	mothershipBitmap = al_load_bitmap("frontend_pc/assets/img/mothership.png");
	if (!mothershipBitmap)
	{
		fprintf(stderr, "Failed to load mothership.png\n");
	}

	explosionBitmap = al_load_bitmap("frontend_pc/assets/img/explosion.png");
	if (!explosionBitmap)
	{
		fprintf(stderr, "Failed to load explosion.png\n");
	}

	titleBitmap = al_load_bitmap("frontend_pc/assets/img/title.png");
	if (!titleBitmap)
	{
		fprintf(stderr, "Failed to load title.png\n");
	}
}

static void drawShip(ship_t ship)
{
	// Draws the ship if the explosion timer is not set. Otherwise, draws the explosion.
	if (ship.entity.explosionTimer > 0)
	{
		if (ship.entity.explosionTimer % 2) // To animate explosion sprite
		{
			al_draw_scaled_bitmap(
				explosionBitmap,
				0, 0,
				al_get_bitmap_width(explosionBitmap), al_get_bitmap_height(explosionBitmap),
				ship.entity.x + SHIP_WIDTH * 0.1, ship.entity.y + SHIP_HEIGHT * 0.1,
				SHIP_WIDTH * 0.6, SHIP_HEIGHT * 0.6,
				0);
		}
		else
		{
			al_draw_scaled_bitmap(
				explosionBitmap,
				0, 0,
				al_get_bitmap_width(explosionBitmap), al_get_bitmap_height(explosionBitmap),
				ship.entity.x, ship.entity.y,
				SHIP_WIDTH, SHIP_HEIGHT,
				0);
		}
	}
	else
	{
		if(ship.invencibilityTicks % 2 == 0)
		{
			al_draw_scaled_bitmap(
				shipBitMap,
				0, 0,
				al_get_bitmap_width(shipBitMap), al_get_bitmap_height(shipBitMap),
				ship.entity.x, ship.entity.y,
				SHIP_WIDTH, SHIP_HEIGHT,
				0);
		}
	}
}

static void drawAliens(alienFormation_t aliens)
{
	int i, j;

	for (i = 0; i < ALIENS_ROWS; i++)
	{
		for (j = 0; j < ALIENS_COLS; j++)
		{
			entity_t alienEntity = aliens.alien[i][j].entity;
			unsigned char alienType = aliens.alien[i][j].alienType;

			// Only draws if the alien is alive
			if (alienEntity.isAlive)
			{
				// Draws the alien if the explosion timer is not set. Otherwise, draws the explosion.
				if (alienEntity.explosionTimer > 0)
				{
					if (alienEntity.explosionTimer % 2) // To animate explosion sprite
					{
						al_draw_scaled_bitmap(
							explosionBitmap,
							0, 0,
							al_get_bitmap_width(explosionBitmap), al_get_bitmap_height(explosionBitmap),
							alienEntity.x + ALIEN_WIDTH * 0.1, alienEntity.y + ALIEN_HEIGHT * 0.1,
							ALIEN_WIDTH * 0.6, ALIEN_HEIGHT * 0.6,
							0);
					}
					else
					{
						al_draw_scaled_bitmap(
							explosionBitmap,
							0, 0,
							al_get_bitmap_width(explosionBitmap), al_get_bitmap_height(explosionBitmap),
							alienEntity.x, alienEntity.y,
							ALIEN_WIDTH, ALIEN_HEIGHT,
							0);
					}
				}
				else
				{
					switch (alienType)
					{
					case 0:
						al_draw_scaled_bitmap(
							alien0BitMap,
							0, 0,
							al_get_bitmap_width(alien0BitMap), al_get_bitmap_height(alien0BitMap),
							alienEntity.x, alienEntity.y,
							ALIEN_WIDTH, ALIEN_HEIGHT,
							0);
						break;
					case 1:
						al_draw_scaled_bitmap(
							alien1BitMap,
							0, 0,
							al_get_bitmap_width(alien1BitMap), al_get_bitmap_height(alien1BitMap),
							alienEntity.x, alienEntity.y,
							ALIEN_WIDTH, ALIEN_HEIGHT,
							0);
						break;
					case 2:
						al_draw_scaled_bitmap(
							alien2BitMap,
							0, 0,
							al_get_bitmap_width(alien2BitMap), al_get_bitmap_height(alien2BitMap),
							alienEntity.x, alienEntity.y,
							ALIEN_WIDTH, ALIEN_HEIGHT,
							0);
						break;
					case 3:
						al_draw_scaled_bitmap(
							alien3BitMap,
							0, 0,
							al_get_bitmap_width(alien3BitMap), al_get_bitmap_height(alien3BitMap),
							alienEntity.x, alienEntity.y,
							ALIEN_WIDTH, ALIEN_HEIGHT,
							0);
						break;
					case 4:
						al_draw_scaled_bitmap(
							alien4BitMap,
							0, 0,
							al_get_bitmap_width(alien4BitMap), al_get_bitmap_height(alien4BitMap),
							alienEntity.x, alienEntity.y,
							ALIEN_WIDTH, ALIEN_HEIGHT,
							0);
						break;

					default:
						break;
					}
				}
			}
		}
	}
}

static void drawMothership(mothership_t mothership)
{
	// Only draws if the mothership is alive
	if (mothership.entity.isAlive)
	{
		// Draws the mothership if the explosion timer is not set. Otherwise, draws the explosion.
		if (mothership.entity.explosionTimer > 0)
		{
			if (mothership.entity.explosionTimer % 2)
			{
				al_draw_scaled_bitmap(
					explosionBitmap,
					0, 0,
					al_get_bitmap_width(explosionBitmap), al_get_bitmap_height(explosionBitmap),
					mothership.entity.x + 0.1 * MOTHERSHIP_WIDTH, mothership.entity.y + 0.1 * MOTHERSHIP_HEIGHT,
					MOTHERSHIP_WIDTH * 0.6, MOTHERSHIP_HEIGHT * 0.6,
					0);
			}
			else
			{
				al_draw_scaled_bitmap(
					explosionBitmap,
					0, 0,
					al_get_bitmap_width(explosionBitmap), al_get_bitmap_height(explosionBitmap),
					mothership.entity.x, mothership.entity.y,
					MOTHERSHIP_WIDTH, MOTHERSHIP_HEIGHT,
					0);
			}
		}
		else
		{
			al_draw_scaled_bitmap(
				mothershipBitmap,
				0, 0,
				al_get_bitmap_width(mothershipBitmap), al_get_bitmap_height(mothershipBitmap),
				mothership.entity.x, mothership.entity.y,
				MOTHERSHIP_WIDTH, MOTHERSHIP_HEIGHT,
				0);
		}
	}
}

static void drawBullets(bullet_t shipBullet, bullet_t alienBullet)
{
	// Ship bullet
	// Only draws if the ship bullet is alive
	if (shipBullet.entity.isAlive)
	{
		al_draw_filled_rectangle(
			shipBullet.entity.x,
			shipBullet.entity.y,
			shipBullet.entity.x + BULLET_WIDTH,
			shipBullet.entity.y + BULLET_HEIGHT,
			al_map_rgb(0, 128, 255));
	}

	// Alien bullet
	// Only draws if the alien bullet is alive
	if (alienBullet.entity.isAlive)
	{
		al_draw_filled_rectangle(
			alienBullet.entity.x,
			alienBullet.entity.y,
			alienBullet.entity.x + BULLET_WIDTH,
			alienBullet.entity.y + BULLET_HEIGHT,
			al_map_rgb(0, 128, 255));
	}
}

static void drawBarriers(barrier_t barriers[BARRIERS])
{
	int i, x, y;

	for (i = 0; i < BARRIERS; i++)
	{
		for (x = 0; x < BARRIER_HEIGHT; x++)
		{
			for (y = 0; y < BARRIER_WIDTH; y++)
			{
				entity_t pixel = barriers[i].pixel[x][y].entity;

				// Only draws if the pixel is alive
				if (pixel.isAlive)
				{
					al_draw_filled_rectangle(
						pixel.x,
						pixel.y,
						pixel.x + BARRIER_PIXEL_WIDTH,
						pixel.y + BARRIER_PIXEL_HEIGHT,
						al_map_rgb(0, 128, 255));
				}
			}
		}
	}
}

static void drawScore(int score)
{
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "Score: %d", score);
	al_draw_text(fontRetro, al_map_rgb(255, 255, 255), SCORE_INITIAL_X, SCORE_INITIAL_Y, 0, buffer);
}

static void drawHUD(int score, int lives, int level)
{
    char hudText[128];
	int i;
    
    // Score
    snprintf(hudText, sizeof(hudText), "Score: %d", score);
    al_draw_text(fontRetro, al_map_rgb(255, 255, 255), SCREEN_WIDTH-500, SCREEN_HEIGHT/5, 0, hudText);

	 // Level
    snprintf(hudText, sizeof(hudText), "Level: %d", level);
    al_draw_text(fontRetro, al_map_rgb(255, 255, 255), SCREEN_WIDTH-500, SCREEN_HEIGHT/5 + 100, 0, hudText);

	// Lives
    snprintf(hudText, sizeof(hudText), "Lives: %d", lives);
	al_draw_text(fontRetro, al_map_rgb(255, 255, 255), SCREEN_WIDTH-500, SCREEN_HEIGHT/5 + 200, 0, hudText);

	for(i = 0; i < lives; i++)
	{
		al_draw_text(fontGameplay, al_map_rgb(66,141,255), SCREEN_WIDTH-(320 - i * 50), SCREEN_HEIGHT/5 + 195, 0, "W");
	}    
    
    
}

static void drawAliensPoints()
{
	char buffer[64]; // buffer to store points strings

	// Title
	al_draw_text(fontRetro, al_map_rgb(200, 200, 200),
				 SCREEN_WIDTH / 2 + 30, SCREEN_HEIGHT / 2,
				 ALLEGRO_ALIGN_CENTER, "*SCORE ADVANCE TABLE*");

	// Ubication data
	const float startY = SCREEN_HEIGHT / 2 + 60;
	const float rowSpacing = 40;
	const float spriteX = SCREEN_WIDTH / 2 - 150;
	const float textX = SCREEN_WIDTH / 2 - 100;
	const float spriteScale = 0.5;

	// Line 1 - Alien 0
	al_draw_scaled_bitmap(
		alien0BitMap,
		0, 0,
		al_get_bitmap_width(alien0BitMap), al_get_bitmap_height(alien0BitMap),
		spriteX, startY,
		ALIEN_WIDTH * spriteScale, ALIEN_HEIGHT * spriteScale,
		0);

	sprintf(buffer, "= %d POINTS", ALIEN_TYPE_0_POINTS); // saves alien points in buffer

	al_draw_text(fontRetro, al_map_rgb(200, 200, 200),
				 textX, startY,
				 ALLEGRO_ALIGN_LEFT, buffer);

	// Line 2 - Alien 1
	al_draw_scaled_bitmap(
		alien1BitMap,
		0, 0,
		al_get_bitmap_width(alien1BitMap), al_get_bitmap_height(alien1BitMap),
		spriteX, startY + rowSpacing,
		ALIEN_WIDTH * spriteScale, ALIEN_HEIGHT * spriteScale,
		0);

	sprintf(buffer, "= %d POINTS", ALIEN_TYPE_1_POINTS); // saves alien points in buffer

	al_draw_text(fontRetro, al_map_rgb(200, 200, 200),
				 textX, startY + rowSpacing,
				 ALLEGRO_ALIGN_LEFT, buffer);

	// Line 3 - Alien 2
	al_draw_scaled_bitmap(
		alien2BitMap,
		0, 0,
		al_get_bitmap_width(alien2BitMap), al_get_bitmap_height(alien2BitMap),
		spriteX, startY + 2 * rowSpacing,
		ALIEN_WIDTH * spriteScale, ALIEN_HEIGHT * spriteScale,
		0);

	sprintf(buffer, "= %d POINTS", ALIEN_TYPE_2_POINTS); // saves alien points in buffer

	al_draw_text(fontRetro, al_map_rgb(200, 200, 200),
				 textX, startY + 2 * rowSpacing,
				 ALLEGRO_ALIGN_LEFT, buffer);

	// Line 4 - Alien 3
	al_draw_scaled_bitmap(
		alien3BitMap,
		0, 0,
		al_get_bitmap_width(alien3BitMap), al_get_bitmap_height(alien3BitMap),
		spriteX, startY + 3 * rowSpacing,
		ALIEN_WIDTH * spriteScale, ALIEN_HEIGHT * spriteScale,
		0);
	
	sprintf(buffer, "= %d POINTS", ALIEN_TYPE_3_POINTS); // saves alien points in buffer

	al_draw_text(fontRetro, al_map_rgb(200, 200, 200),
				 textX, startY + 3 * rowSpacing,
				 ALLEGRO_ALIGN_LEFT, buffer);

	// Line 5 - Alien 4
	al_draw_scaled_bitmap(
		alien4BitMap,
		0, 0,
		al_get_bitmap_width(alien4BitMap), al_get_bitmap_height(alien4BitMap),
		spriteX, startY + 4 * rowSpacing,
		ALIEN_WIDTH * spriteScale, ALIEN_HEIGHT * spriteScale,
		0);
	
	sprintf(buffer, "= %d POINTS", ALIEN_TYPE_4_POINTS); // saves alien points in buffer

	al_draw_text(fontRetro, al_map_rgb(200, 200, 200),
				 textX, startY + 4 * rowSpacing,
				 ALLEGRO_ALIGN_LEFT, buffer);

	// Line 6 - Mystery mothership
	al_draw_scaled_bitmap(
		mothershipBitmap,
		0, 0,
		al_get_bitmap_width(mothershipBitmap), al_get_bitmap_height(mothershipBitmap),
		spriteX - 15, startY + 5 * rowSpacing,
		MOTHERSHIP_WIDTH * spriteScale, MOTHERSHIP_HEIGHT * spriteScale,
		0);
	
	al_draw_text(fontRetro, al_map_rgb(200, 200, 200),
				 textX, startY + 5 * rowSpacing,
				 ALLEGRO_ALIGN_LEFT, "= ??? MYSTERY");

}
