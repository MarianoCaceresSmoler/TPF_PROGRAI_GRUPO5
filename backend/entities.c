#include <entities.h>

ship_t createShip()
{
	ship_t ship = {{}};
	
}

alienFormation_t createEnemies(int x, int y)
{

}

barrier_t createBarrier(int x, int y)
{

}

bullet_t createBullet(int x, int y)
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
