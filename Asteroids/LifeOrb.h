#pragma once

#include "SpaceObject.h"


class LifeOrb : public SpaceObject{

	float inv_radius, inv_x, inv_y;	// refering to the imaginary circle the orb is moving on every time
	
	float prev_inv_radius, prev_inv_x, prev_inv_y; // refering to the imaginary circle the orb was moving on previously
	
public:
	void init() override;
	void draw() override;
	void update() override;

	Disk getCollisionHull() const override;
	
	LifeOrb(const class Game& game);
	LifeOrb();
	~LifeOrb();

};