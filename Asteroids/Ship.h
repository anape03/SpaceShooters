#pragma once

#include "SpaceObject.h"


class Ship : public SpaceObject {

	int stage = 0; // 0 = active, the rest is for explosion
	bool is_active = false;
	int frame = 1; // total of 2 frames
	float last_frame_change; // to change ship frames for fire animation
	
public:
	void init() override;
	void draw() override;
	void update() override;

	Disk getCollisionHull() const override;

	bool getIsActive() const { return is_active; }
	void setIsActive(bool value) { is_active = value; }

	int getStage() { return stage; }
	void increaseStage() { stage++; }

	Ship(const class Game& game);
	Ship();
	~Ship();
	
};