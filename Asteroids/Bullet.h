#pragma once

#include "SpaceObject.h"

class Bullet : public SpaceObject {

public:
	void init() override;
	void draw() override;
	void update() override;

	Disk getCollisionHull() const override;

	Bullet(const class Game& game);
	Bullet();
	~Bullet();

};