#include "graphics.h"
#include "Bullet.h"
#include "Game.h"
#include "config.h"


void Bullet::init() {
	Ship* ship = game.getShip();

	angle = -ship->getAngle() - 90.0f;
	center_x = ship->getX() + ship->getRadius() * cos((double)angle * (atan(1) * 4) / 180.0f);
	center_y = ship->getY() + ship->getRadius() * sin((double)angle * (atan(1) * 4) / 180.0f);
	speed = (!game.isSlow()) ? 1.8f : 1.3f;
	radius = 3.0f;
}


void Bullet::draw() {
	graphics::setOrientation(angle);

	graphics::Brush br;
	br.fill_color[0] = 0.8f;
	br.fill_color[1] = 0.0f;
	br.fill_color[2] = 1.0f;
	graphics::drawRect(center_x, center_y, radius, radius, br);
	graphics::resetPose();

	if (game.getIsDebug()) {
		// draw Collision Disk during Debug Mode ==============
		br.outline_opacity = 1.0f;
		br.texture = "";
		br.fill_color[0] = 0.3f;
		br.fill_color[1] = 0.3f;
		br.fill_color[2] = 1.0f;
		br.fill_opacity = 0.3f;
		Disk col_hull = getCollisionHull();
		graphics::drawDisk(col_hull.cx, col_hull.cy, col_hull.radius, br);
	}
}


void Bullet::update() {
	center_x += speed * cos((double)angle * (atan(1) * 4) / 180.0f);
	center_y += speed * sin((double)angle * (atan(1) * 4) / 180.0f);
}


Disk Bullet::getCollisionHull() const {
	Disk disk;
	disk.cx = center_x;
	disk.cy = center_y;
	disk.radius = radius;
	return disk;
}


Bullet::Bullet(const Game& game) : SpaceObject(game) {
	init();
}


Bullet::Bullet() {
}


Bullet::~Bullet() {
}
