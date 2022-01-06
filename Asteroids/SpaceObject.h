#pragma once

struct Disk {
	float cx, cy, radius;
};

class SpaceObject {

protected:
	const class Game& game;

	float center_x;
	float center_y;
	float angle;
	float radius;
	float speed;

	/// <summary>* for ship: used for timing between explosion stages,
	/// <para>and in the begining for cooldown</para>
	/// <para>* for asteroids: used for timing between explosion stages</para>
	/// <para>* for lifeorb: used for changing circle it's moving on,</para>
	/// <para>and in first appearance for flickering in place</para>
	/// </summary>
	float last_time_change;

public:
	virtual void init() = 0;
	virtual void draw() = 0;
	virtual void update() = 0;

	float getAngle() { return angle; }
	float getX() { return center_x; }
	float getY() { return center_y; }
	float getRadius() { return radius; }
	float getLastTime() { return last_time_change; }

	virtual Disk getCollisionHull() const = 0;

	SpaceObject(const class Game& mygame);
	SpaceObject();
};
