#include <entities.h>

ship_t createShip(int x, int y)
{
	ship_t ship = {{x, y, SHIP_HEIGHT, SHIP_WIDTH, 1}, SHIP_LIVES, 1, 0, 0};
	return ship;
}

static alien_t createAlien(int x, int y, int type)
{
	alien_t alien = {{x, y, ALIEN_HEIGTH, ALIEN_WIDTH, 1}, type};
	return alien;
}

alienFormation_t createEnemies(int x, int y)
{
	alienFormation_t enemies;
	int i, j;
	for(i = 0; i < ALIENS_ROWS; i++)
	{
		for(j = 0; j < ALIENS_COLS; j++)
		{
			(enemies.alien[i][j]) = createAlien(x + j * 2 * ALIEN_WIDTH, y + i * 2 * ALIEN_HEIGTH, i);
		}
	}
	enemies.tickRate = ALIEN_MAX_MOVE_TICKRATE; 
	enemies.direction = 1;
	enemies.canShoot = 0;
	return enemies;
}

static barrierPixel_t createBarrierPixel(int x, int y, int isAlive)
{
	barrierPixel_t pixel = {{x, y, BARRIER_PIXEL_HEIGHT, BARRIER_PIXEL_WIDTH, isAlive}};
	return pixel;
}

barrier_t createBarrier(int x, int y)
{
	barrier_t barrier;
	int i, j;
	for(i = 0; i < BARRIER_HEIGHT; i++)
	{
		for(j = 0; j < BARRIER_WIDTH; j++)
		{
			(barrier.pixel[i][j]) = createBarrierPixel(x + j * BARRIER_PIXEL_WIDTH, y + i * BARRIER_PIXEL_HEIGHT, (i != 1 && (i == j || i == 3-j)) ? 0: 1);
		}
	}
	return barrier;
}

bullet_t createBullet(int x, int y)
{

}

mothership_t createMothership(int x, int y)
{
	
}

void moveShipLeft(ship_t * ship)
{

}

void moveShipRight(ship_t * ship);
void moveEnemiesLeft(alienFormation_t * enemies);
void moveEnemiesRight(alienFormation_t * enemies);
void moveEnemiesDown(alienFormation_t * enemies);
void moveMothership(mothership_t * mothership);
void moveBullet(bullet_t * bullet);
void shipShoot(ship_t * ship, bullet_t * bullet);
void alienShoot(alien_t * alien, bullet_t * bullet);
