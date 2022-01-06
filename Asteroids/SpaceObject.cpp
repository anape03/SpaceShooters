#include "SpaceObject.h"
#include "Game.h"

SpaceObject::SpaceObject(const class Game& mygame) : game(mygame) {}
SpaceObject::SpaceObject() : game(Game()) {}