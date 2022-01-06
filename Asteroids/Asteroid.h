#pragma once

#include "SpaceObject.h"


class Asteroid : public SpaceObject {

	float rotation;
	int stage = 0; // 0 = active, the rest is for explosion
	
public:
	void init() override;
	//void init(Asteroid* ast, int num); ///////////////////
	void draw() override;
	void update() override;

	Disk getCollisionHull() const override;

	int getStage() const { return stage; }
	void increaseStage() { stage++; }
	float getRotation() const { return rotation; }
	bool isBig() { return this->radius > 30 ? true : false; }

	Asteroid(const class Game& game);
	//Asteroid(const class Game& game, Asteroid* ast, int num); //////////////////////
	Asteroid();
	~Asteroid();
};