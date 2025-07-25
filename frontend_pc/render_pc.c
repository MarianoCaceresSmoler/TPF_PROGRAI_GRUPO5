/***************************************************************************//**
  @file 	render_pc.c
  @brief 	Render functions for allegro display
  @author 	Grupo_5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_video.h>

#include "../backend/config.h"
#include "../backend/entities.h"
#include "render_pc.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define CROSSFADE_TIME 0.5 // background video crossfade time in seconds
#define BACKGROUND_VIDEO_DURATION 5 // duration of background video between fades

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/**
 * @brief private function to load sprites
 * @return 0 if success, -1 if failure loading images
 */
static int loadImages(void);

/**
 * @brief function to draw background on display
 */
static void renderBackgroundVideo();

/**
 * @brief function to draw ship on display
 * @param ship copy of the ship object
 */
static void drawShip(ship_t ship);

/**
 * @brief function to draw aliens on display
 * @param alienFormation structure of aliens of the game
 * @param activePowerUp array with the status of every power up in the game
 */
static void drawAliens(alienFormation_t alienFormation, int activePowerUp[POWERUP_TYPES]);

/**
 * @brief function to draw aliens loading animation on display
 * @param alienFormation structure of aliens of the game
 * @param aliensToDraw number of aliens to draw on display
 */
static void drawAliensLoading(alienFormation_t alienFormation, int aliensToDraw);

/**
 * @brief function to draw mothership on display
 * @param mothership structure with the mothership information
 */
static void drawMothership(mothership_t mothership);

/**
 * @brief function to draw bullets on display
 * @param shipBullet structure of the ship bullet to draw
 * @param alienBullet structure of the aliens bullet to draw
 */
static void drawBullets(bullet_t shipBullet, bullet_t alienBullet);

/**
 * @brief function to draw barriers on display
 * @param barriers array of structures with the barriers to draw
 */
static void drawBarriers(barrier_t barriers[BARRIERS]);

/**
 * @brief function to draw power ups on display
 * @param powerUp array with every power up on the game 
 */
static void drawPowerUps(powerUp_t powerUp[POWERUP_TYPES]);

/**
 * @brief private function to draw aliens/mothership points in menu
 * @return 0 if success, -1 if failure drawing on display
 */
static void drawAliensPoints();

/**
 * @brief private function to draw HUD during gameplay
 * @param score the actual score
 * @param lives the lives left
 * @param level the actual level
 * @return 0 if success, -1 if failure drawing on display
 */
static void drawHUD(int score, int lives, int level);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// General control variables
static ALLEGRO_DISPLAY *display = NULL;
static ALLEGRO_EVENT_QUEUE *eventQueue = NULL;
static ALLEGRO_TIMER *timer = NULL;

// Fonts
static ALLEGRO_FONT *fontGameplay = NULL;
static ALLEGRO_FONT *fontRetro = NULL;

// Videos
static ALLEGRO_VIDEO *backgroundVideo1;
static ALLEGRO_VIDEO *backgroundVideo2;

// Bitmaps for rendering elements on display
static ALLEGRO_BITMAP *alien0BitMap = NULL;
static ALLEGRO_BITMAP *alien1BitMap = NULL;
static ALLEGRO_BITMAP *alien2BitMap = NULL;
static ALLEGRO_BITMAP *alien3BitMap = NULL;
static ALLEGRO_BITMAP *alien4BitMap = NULL;
static ALLEGRO_BITMAP *alien0MoveBitMap = NULL;
static ALLEGRO_BITMAP *alien1MoveBitMap = NULL;
static ALLEGRO_BITMAP *alien2MoveBitMap = NULL;
static ALLEGRO_BITMAP *alien3MoveBitMap = NULL;
static ALLEGRO_BITMAP *alien4MoveBitMap = NULL;
static ALLEGRO_BITMAP *shipBitMap = NULL;
static ALLEGRO_BITMAP *barrierPixelBitmap = NULL;
static ALLEGRO_BITMAP *bulletBitmap = NULL;
static ALLEGRO_BITMAP *shipBulletBitmap = NULL;
static ALLEGRO_BITMAP *alienBulletBitmap = NULL;
static ALLEGRO_BITMAP *powerUpBitmaps[POWERUP_TYPES] = {NULL};
static ALLEGRO_BITMAP *mothershipBitmap = NULL;
static ALLEGRO_BITMAP *explosionBitmap = NULL;
static ALLEGRO_BITMAP *titleBitmap = NULL;

static int drawMenu = 1; // to only draw menu once

// To control the background video state
static bool crossfading = false;
static double crossfadeStartTime = 0.0;

/*******************************************************************************
 *******************************************************************************
						GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

int initGraphics(void)
{
	// Initialize Allegro
	if (!al_init())
	{
		printf("Failed to initialize allegro!\n");
		return -1;
	}

	// Initialize Allegro keyboard
	if (!al_install_keyboard())
	{
		printf("Failed to initialize the keyboard!\n");
		return -1;
	}

	// Creates display
	display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!display)
	{
		printf("Unable to create display \n");
		return -1;
	}

	// Creates the timer to control the game rendering speed
	timer = al_create_timer(1.0 / FPS);
	if (!timer) // 1 frame per second timer
	{
		printf("Unable to create timer \n");
		return -1;
	}

	// Initialize allegro images controller
	if (!al_init_image_addon())
	{
		printf("Unable to start image addon \n");
		return -1;
	}

	// Initialize allegro primitives addon
	if (!al_init_primitives_addon())
	{
		printf("Unable to start primitives addon \n");
		return -1;
	}

	// Initialize allegro video addon
	if (!al_init_video_addon())
	{
		printf("Unable to start video addon \n");
		return -1;
	}

	// Create bitmaps for objects
	alien0BitMap = al_create_bitmap(ALIEN_WIDTH, ALIEN_HEIGHT);
	alien1BitMap = al_create_bitmap(ALIEN_WIDTH, ALIEN_HEIGHT);
	alien2BitMap = al_create_bitmap(ALIEN_WIDTH, ALIEN_HEIGHT);
	alien3BitMap = al_create_bitmap(ALIEN_WIDTH, ALIEN_HEIGHT);
	alien4BitMap = al_create_bitmap(ALIEN_WIDTH, ALIEN_HEIGHT);

	alien0MoveBitMap = al_create_bitmap(ALIEN_WIDTH, ALIEN_HEIGHT);
	alien1MoveBitMap = al_create_bitmap(ALIEN_WIDTH, ALIEN_HEIGHT);
	alien2MoveBitMap = al_create_bitmap(ALIEN_WIDTH, ALIEN_HEIGHT);
	alien3MoveBitMap = al_create_bitmap(ALIEN_WIDTH, ALIEN_HEIGHT);
	alien4MoveBitMap = al_create_bitmap(ALIEN_WIDTH, ALIEN_HEIGHT);

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
	if (!(display && timer && alien0BitMap && alien1BitMap && alien2BitMap && alien3BitMap && alien4BitMap && shipBitMap && barrierPixelBitmap && bulletBitmap && mothershipBitmap && eventQueue))
	{
		printf("Failed to load assets.\n");
		cleanupGraphics(); // cleanup allegro graphics if there was an error
		return -1;
	}

	// Calls private function to load sprites
	if(loadImages())
	{
		printf("Failed to load images.\n");
		cleanupGraphics(); // cleanup allegro graphics if there was an error
		return -1;
	}

	// Start background video
	al_start_video(backgroundVideo1, al_get_default_mixer());

	// Registers the event sources
	al_register_event_source(eventQueue, al_get_display_event_source(display));
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));
	al_register_event_source(eventQueue, al_get_video_event_source(backgroundVideo1));
	al_register_event_source(eventQueue, al_get_video_event_source(backgroundVideo2));
	al_register_event_source(eventQueue, al_get_keyboard_event_source());

	// Clears the screen and starts the timer
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();
	al_start_timer(timer);

	return 0;
}

void cleanupGraphics(void)
{
	// Destroys all the allegro elements created 
	if (display)
		al_destroy_display(display);
	if (timer)
		al_destroy_timer(timer);
	if (backgroundVideo1)
		al_close_video(backgroundVideo1);
	if (backgroundVideo2)
		al_close_video(backgroundVideo2);
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
	if (alien0MoveBitMap)
		al_destroy_bitmap(alien0MoveBitMap);
	if (alien1MoveBitMap)
		al_destroy_bitmap(alien1MoveBitMap);
	if (alien2MoveBitMap)
		al_destroy_bitmap(alien2MoveBitMap);
	if (alien3MoveBitMap)
		al_destroy_bitmap(alien3MoveBitMap);
	if (alien4MoveBitMap)
		al_destroy_bitmap(alien4MoveBitMap);
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

ALLEGRO_EVENT_QUEUE *getEventQueue(void)
{
	// Returns allegro event queue
	return eventQueue;
}

void renderGame(game_t game)
{
	al_clear_to_color(al_map_rgb(0, 0, 0)); // first clears the screen to black

	drawMenu = 1; // to draw the menu only once

	if (game.status == GAME_LOADING)
	{
		// draw loading screen (with more FPS)
		if (game.loadingTimer == LOADING_TIME)
			al_set_timer_speed(timer, 1.0 / (FPS * 2));
		else if (game.loadingTimer == 1)
			al_set_timer_speed(timer, 1.0 / FPS);

		renderBackgroundVideo(); // renders background video
		drawAliensLoading(game.aliens, LOADING_TIME - game.loadingTimer); 
		// draw the animation of aliens setting
	}
	else
	{
		renderBackgroundVideo(); // renders background video

		// Draw elements on display
		drawShip(game.ship);
		drawAliens(game.aliens, game.activePowerUp);
		drawBullets(game.shipBullet, game.alienBullet);
		drawBarriers(game.barriers);
		drawPowerUps(game.powerUp);
		drawMothership(game.mothership);
		drawHUD(game.score, game.ship.livesLeft, game.currentLevel);
	}

	al_flip_display(); // updates the display
}

void renderMenu(game_t game)
{
	gameStatus_t status = game.status;

	if (status == GAME_MENU) // If player is at menu
	{
		renderBackgroundVideo(); // renders the background video

		// Draws game title
		int titleWidth = al_get_bitmap_width(titleBitmap);
		int titleHeight = al_get_bitmap_height(titleBitmap);
		float destX = (SCREEN_WIDTH - titleWidth) / 2;
		float destY = SCREEN_HEIGHT / 11;

		al_draw_scaled_bitmap(
			titleBitmap,
			0, 0,
			titleWidth, titleHeight,
			destX, destY,
			titleWidth, titleHeight,
			0);

		drawAliensPoints(); // draws aliens points table

		// Draws input space to enter name tag
		al_draw_text(fontRetro, al_map_rgb(200, 200, 200), SCREEN_WIDTH / 3, SCREEN_HEIGHT * 0.85, 0, "Enter a name to play:");

		// Coords and dims for input box
		float boxWidth = 200;
		float boxHeight = 50;
		float boxX = SCREEN_WIDTH / 3 + boxWidth * 2.3;
		float boxY = SCREEN_HEIGHT * 0.845;

		al_draw_filled_rectangle(boxX, boxY, boxX + boxWidth, boxY + boxHeight, al_map_rgb(30, 30, 30)); // input box background
		al_draw_rectangle(boxX, boxY, boxX + boxWidth, boxY + boxHeight, al_map_rgb(255, 255, 255), 2);	 // input box border

		// draws the text from name tag updated
		al_draw_text(
			fontRetro,
			al_map_rgb(255, 255, 255),
			boxX + boxWidth / 2,
			boxY + (boxHeight - al_get_font_line_height(fontRetro)) / 2,
			ALLEGRO_ALIGN_CENTRE,
			game.nameTag);
	}
	else if (status == GAME_PAUSED) // If game was paused
	{
		if (drawMenu) // renders pause menu only one time
		{
			al_draw_filled_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, al_map_rgba(0, 0, 0, 128));

			al_draw_text(fontRetro, al_map_rgb(255, 255, 0),
						 SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3,
						 ALLEGRO_ALIGN_CENTER, "GAME PAUSED");

			al_draw_text(fontRetro, al_map_rgb(200, 200, 200),
						 SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,
						 ALLEGRO_ALIGN_CENTER, "Press ENTER to resume, ESC to quit, R to restart the game");
		}
		drawMenu = 0;
	}

	al_flip_display(); // updates the display
}

void renderGameOver(game_t game)
{
	int score = game.score;
	int i;
	char highScoreBuffer[100]; // buffer to save player's name tag and scores

	al_draw_filled_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, al_map_rgba(0, 0, 0, 20)); // shadowing the background gameplay

	al_draw_text(fontRetro, al_map_rgb(255, 0, 0),
				 SCREEN_WIDTH / 2, SCREEN_HEIGHT / 6,
				 ALLEGRO_ALIGN_CENTER, "GAME OVER");

	char scoreText[64];
	snprintf(scoreText, sizeof(scoreText), "Final Score: %d", score);
	al_draw_text(fontRetro, al_map_rgb(255, 255, 255),
				 SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4,
				 ALLEGRO_ALIGN_CENTER, scoreText);

	// Highscores table
	for (i = 0; i < MAX_SCORES; i++)
	{
		sprintf(highScoreBuffer, "%s %d", game.highScores[i].tag, game.highScores[i].score);

		if (game.scoreRank == i + 1) // if the player entered the top, highlights it
		{
			al_draw_text(fontRetro, al_map_rgb(255, 255, 0),
						 SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4 + 60 + i * 50,
						 ALLEGRO_ALIGN_CENTER, highScoreBuffer);
		}
		else // draws the other players records in gray
		{
			al_draw_text(fontRetro, al_map_rgb(200, 200, 200),
						 SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4 + 60 + i * 50,
						 ALLEGRO_ALIGN_CENTER, highScoreBuffer);
		}
	}

	al_draw_text(fontRetro, al_map_rgb(255, 255, 255),
				 SCREEN_WIDTH / 2, SCREEN_HEIGHT - 200,
				 ALLEGRO_ALIGN_CENTER, "Enter ESC to quit, R to restart the game");

	al_flip_display(); //updates the display
}

/*******************************************************************************
 *******************************************************************************
						LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static int loadImages(void)
{
	// Load the bitmaps with the images stored

	alien0BitMap = al_load_bitmap("frontend_pc/assets/img/alien0.png");
	if (!alien0BitMap)
	{
		printf("Failed to load alien0.png\n");
		return -1;
	}

	alien1BitMap = al_load_bitmap("frontend_pc/assets/img/alien1.png");
	if (!alien1BitMap)
	{
		printf("Failed to load alien1.png\n");
		return -1;
	}

	alien2BitMap = al_load_bitmap("frontend_pc/assets/img/alien2.png");
	if (!alien2BitMap)
	{
		printf("Failed to load alien2.png\n");
		return -1;
	}

	alien3BitMap = al_load_bitmap("frontend_pc/assets/img/alien3.png");
	if (!alien3BitMap)
	{
		printf("Failed to load alien3.png\n");
		return -1;
	}

	alien4BitMap = al_load_bitmap("frontend_pc/assets/img/alien4.png");
	if (!alien4BitMap)
	{
		printf("Failed to load alien4.png\n");
		return -1;
	}

	alien0MoveBitMap = al_load_bitmap("frontend_pc/assets/img/alien0move.png");
	if (!alien0MoveBitMap)
	{
		printf("Failed to load alien0move.png\n");
		return -1;
	}

	alien1MoveBitMap = al_load_bitmap("frontend_pc/assets/img/alien1move.png");
	if (!alien1MoveBitMap)
	{
		printf("Failed to load alien1move.png\n");
		return -1;
	}

	alien2MoveBitMap = al_load_bitmap("frontend_pc/assets/img/alien2move.png");
	if (!alien2MoveBitMap)
	{
		printf("Failed to load alien2move.png\n");
		return -1;
	}

	alien3MoveBitMap = al_load_bitmap("frontend_pc/assets/img/alien3move.png");
	if (!alien3MoveBitMap)
	{
		printf("Failed to load alien3move.png\n");
		return -1;
	}

	alien4MoveBitMap = al_load_bitmap("frontend_pc/assets/img/alien4move.png");
	if (!alien4MoveBitMap)
	{
		printf("Failed to load alien4move.png\n");
		return -1;
	}

	shipBitMap = al_load_bitmap("frontend_pc/assets/img/ship.png");
	if (!shipBitMap)
	{
		printf("Failed to load ship.png\n");
		return -1;
	}

	mothershipBitmap = al_load_bitmap("frontend_pc/assets/img/mothership.png");
	if (!mothershipBitmap)
	{
		printf("Failed to load mothership.png\n");
		return -1;
	}

	shipBulletBitmap = al_load_bitmap("frontend_pc/assets/img/shipBullet.png");
	if (!shipBulletBitmap)
	{
		printf("Failed to load shipBullet.png\n");
		return -1;
	}

	alienBulletBitmap = al_load_bitmap("frontend_pc/assets/img/alienBullet.png");
	if (!alienBulletBitmap)
	{
		printf("Failed to load alienBullet.png\n");
		return -1;
	}

	int i;
	for (i = 0; i < POWERUP_TYPES; i++)
	{
		char filename[64];
		snprintf(filename, sizeof(filename), "frontend_pc/assets/img/powerUp%d.png", i);
		powerUpBitmaps[i] = al_load_bitmap(filename); // load each power up individually
		if (!powerUpBitmaps[i])
		{
			printf("Failed to load powerup%d.png\n", i);
			return -1;
		}
	}

	explosionBitmap = al_load_bitmap("frontend_pc/assets/img/explosion.png");
	if (!explosionBitmap)
	{
		printf("Failed to load explosion.png\n");
		return -1;
	}

	titleBitmap = al_load_bitmap("frontend_pc/assets/img/title.png");
	if (!titleBitmap)
	{
		printf("Failed to load title.png\n");
		return -1;
	}

	backgroundVideo1 = al_open_video("frontend_pc/assets/img/background.ogv");
	if (!backgroundVideo1)
	{
		printf("Failed to load background.ogv\n");
		return -1;
	}

	backgroundVideo2 = al_open_video("frontend_pc/assets/img/background.ogv");
	if (!backgroundVideo2)
	{
		printf("Failed to load background.ogv\n");
		return -1;
	}

	return 0;
}

static void renderBackgroundVideo()
{
	// Get the current position of the first background video (in seconds)
	double pos = al_get_video_position(backgroundVideo1, 0);
	double dur = BACKGROUND_VIDEO_DURATION;

	// If not already crossfading and the first video is near its end
	if (!crossfading && pos >= dur - CROSSFADE_TIME)
	{
        // Start playing the second video with the default audio mixer
		al_start_video(backgroundVideo2, al_get_default_mixer());

		// Activate crossfade mode
		crossfading = true;
		crossfadeStartTime = al_get_time(); // Mark the start time of the crossfad
	}

	// If currently crossfading between video
	if (crossfading)
	{
		double elapsed = al_get_time() - crossfadeStartTime; // Time passed since crossfade began
        float alpha1 = 1.0f - (elapsed / CROSSFADE_TIME);    // Fade-out factor for video1
        float alpha2 = elapsed / CROSSFADE_TIME;             // Fade-in factor for video2

	 	// Clamp alpha values between 0 and 1
		if (alpha1 < 0.0f)
			alpha1 = 0.0f;
		if (alpha2 > 1.0f)
			alpha2 = 1.0f;

        // --- Draw both video frames with their respective transparency ---

		// Get the current frame of the first video
		ALLEGRO_BITMAP *currentFrame1 = al_get_video_frame(backgroundVideo1);
		if (currentFrame1)
		{
			al_draw_tinted_scaled_bitmap(
				currentFrame1,
				al_map_rgba_f(1, 1, 1, alpha1), // Apply alpha1 transparency
				0, 0,
				al_get_bitmap_width(currentFrame1), al_get_bitmap_height(currentFrame1),
				0, 0,
				SCREEN_WIDTH, SCREEN_HEIGHT,
				0);
		}		

		// Get the current frame of the second video
		ALLEGRO_BITMAP *currentFrame2 = al_get_video_frame(backgroundVideo2);
		if (currentFrame2)
		{
			al_draw_tinted_scaled_bitmap(
				currentFrame2,
				al_map_rgba_f(1, 1, 1, alpha2), // Apply alpha2 transparency
				0, 0,
				al_get_bitmap_width(currentFrame2), al_get_bitmap_height(currentFrame2),
				0, 0,
				SCREEN_WIDTH, SCREEN_HEIGHT,
				0);
		}

		// If the crossfade is complete
		if (elapsed >= CROSSFADE_TIME)
		{
            // Swap video pointers: backgroundVideo2 becomes the new main video
			ALLEGRO_VIDEO *temp = backgroundVideo1;
			backgroundVideo1 = backgroundVideo2;
			backgroundVideo2 = temp;

			// Rewind the old video to use it again in the next crossfade
			al_seek_video(backgroundVideo2, 0);
			// End the crossfade mode
			crossfading = false;
		}
	}
	else
	{
        // If not crossfading, simply draw the current frame of the main video
		ALLEGRO_BITMAP *currentFrame = al_get_video_frame(backgroundVideo1);
		if (currentFrame)
		{
			al_draw_scaled_bitmap(
				currentFrame,
				0, 0,
				al_get_bitmap_width(currentFrame), al_get_bitmap_height(currentFrame),
				0, 0,
				SCREEN_WIDTH, SCREEN_HEIGHT,
				0);
		}
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
	else if (ship.invencibilityTicks % 2 == 0) // After the ship was hit
	{
		const float PI = 3.14159265f;
		float angleRads = 0, angleGrads = 20;

		// Angles the bitmap if the ship is moving
		if (ship.direction == MOVING_RIGHT)
		{
			angleRads = angleGrads * PI / 180.0f;
		}
		else if (ship.direction == MOVING_LEFT)
		{
			angleRads = -angleGrads * PI / 180.0f;
		}

		// Centres of original bitmap
		float bitmap_cx = al_get_bitmap_width(shipBitMap) / 2.0f;
		float bitmap_cy = al_get_bitmap_height(shipBitMap) / 2.0f;

		// Position in display (centre of the desire sprite)
		float draw_x = ship.entity.x + SHIP_WIDTH / 2.0f;
		float draw_y = ship.entity.y + SHIP_HEIGHT / 2.0f;

		// Escalated
		float scale_x = SHIP_WIDTH / (float)al_get_bitmap_width(shipBitMap);
		float scale_y = SHIP_HEIGHT / (float)al_get_bitmap_height(shipBitMap);

		// Final draw
		al_draw_scaled_rotated_bitmap(
			shipBitMap,
			bitmap_cx, bitmap_cy,
			draw_x, draw_y,
			scale_x, scale_y,
			angleRads,
			0);
	}
}

static void drawAliens(alienFormation_t aliens, int activePowerUp[POWERUP_TYPES])
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

					// Gets the alien bitmap based on the alien type
					ALLEGRO_BITMAP *bitmap = NULL;

					if (!aliens.alien[i][j].isMoving)
					{
						switch (alienType)
						{
						case 0:
							bitmap = alien0BitMap;
							break;
						case 1:
							bitmap = alien1BitMap;
							break;
						case 2:
							bitmap = alien2BitMap;
							break;
						case 3:
							bitmap = alien3BitMap;
							break;
						case 4:
							bitmap = alien4BitMap;
							break;
						default:							
							break;
						}
					}
					else
					{
						switch (alienType)
						{
						case 0:
							bitmap = alien0MoveBitMap;
							break;
						case 1:
							bitmap = alien1MoveBitMap;
							break;
						case 2:
							bitmap = alien2MoveBitMap;
							break;
						case 3:
							bitmap = alien3MoveBitMap;
							break;
						case 4:
							bitmap = alien4MoveBitMap;
							break;
						default:
							break;
						}
					}

					if (activePowerUp[FREEZE_POWERUP]) // if the aliens are frozen, draw them "turned off" with a gray tinted
					{
						al_draw_tinted_scaled_bitmap(
							bitmap,
							al_map_rgba_f(0.5, 0.5, 0.0, 1.0),
							0, 0,
							al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap),
							alienEntity.x, alienEntity.y,
							ALIEN_WIDTH, ALIEN_HEIGHT,
							0);
					}
					else // if not, draw the aliens normally
					{
						al_draw_scaled_bitmap(
							bitmap,
							0, 0,
							al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap),
							alienEntity.x, alienEntity.y,
							ALIEN_WIDTH, ALIEN_HEIGHT,
							0);
					}
				}
			}
		}
	}

}

static void drawAliensLoading(alienFormation_t aliens, int aliensToDraw)
{
	int i, j, drawn = 0;

	for (i = ALIENS_ROWS - 1; i >= 0; i--)
	{
		for (j = 0; j < ALIENS_COLS; j++)
		{
			if (drawn >= aliensToDraw)
			{
				// aliensToDraw++;
				return; // finish when reaches the limit of aliens to draw (per frame)
			}

			entity_t alienEntity = aliens.alien[i][j].entity;
			unsigned char alienType = aliens.alien[i][j].alienType;

			// gets and draws the alien bitmap based on its type
			ALLEGRO_BITMAP *bitmap = NULL;
			switch (alienType)
			{
			case 0:
				bitmap = alien0BitMap;
				break;
			case 1:
				bitmap = alien1BitMap;
				break;
			case 2:
				bitmap = alien2BitMap;
				break;
			case 3:
				bitmap = alien3BitMap;
				break;
			case 4:
				bitmap = alien4BitMap;
				break;
			default:
				continue;
			}

			al_draw_scaled_bitmap(
				bitmap,
				0, 0,
				al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap),
				alienEntity.x, alienEntity.y,
				ALIEN_WIDTH, ALIEN_HEIGHT,
				0);

			drawn++; // increases the counter of drawn aliens
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
		al_draw_scaled_bitmap(
			shipBulletBitmap,
			0, 0,
			al_get_bitmap_width(shipBulletBitmap), al_get_bitmap_height(shipBulletBitmap),
			shipBullet.entity.x, shipBullet.entity.y,
			BULLET_WIDTH, BULLET_HEIGHT,
			0);
	}

	// Alien bullet
	// Only draws if the alien bullet is alive
	if (alienBullet.entity.isAlive)
	{
		al_draw_scaled_bitmap(
			alienBulletBitmap,
			0, 0,
			al_get_bitmap_width(alienBulletBitmap), al_get_bitmap_height(alienBulletBitmap),
			alienBullet.entity.x, alienBullet.entity.y,
			BULLET_WIDTH, BULLET_HEIGHT,
			0);
	}
}

static void drawBarriers(barrier_t barriers[BARRIERS])
{
	int i, x, y;

	// Draw each barrier pixel by pixel
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

static void drawPowerUps(powerUp_t powerUps[POWERUP_TYPES])
{
	int i;

	for (i = 0; i < POWERUP_TYPES; i++)
	{
		if (powerUps[i].entity.isAlive) // Only draws if the power-up is alive
		{
			al_draw_scaled_bitmap(
				powerUpBitmaps[powerUps[i].type],
				0, 0,
				al_get_bitmap_width(powerUpBitmaps[powerUps[i].type]),
				al_get_bitmap_height(powerUpBitmaps[powerUps[i].type]),
				powerUps[i].entity.x, powerUps[i].entity.y,
				POWERUP_WIDTH, POWERUP_HEIGHT,
				0);
		}
	}
}

static void drawHUD(int score, int lives, int level)
{
	char hudText[128];
	int i;

	// Score
	snprintf(hudText, sizeof(hudText), "Score: %d", score);
	al_draw_text(fontRetro, al_map_rgb(255, 255, 255), 150, 20, 0, hudText);

	// Level
	snprintf(hudText, sizeof(hudText), "Level: %d", level);
	al_draw_text(fontRetro, al_map_rgb(255, 255, 255), SCREEN_WIDTH - 300, 20, 0, hudText);

	// Lives
	snprintf(hudText, sizeof(hudText), "Lives: %d", lives);
	al_draw_text(fontRetro, al_map_rgb(255, 255, 255), 800, 20, 0, hudText);

	for (i = 0; i < lives; i++)
	{
		al_draw_text(fontGameplay, al_map_rgb(66, 141, 255), 975 + i * 50, 17, 0, "W");
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
