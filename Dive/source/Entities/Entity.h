// Entity.h
// A general class tha represents an in-game entity.
// An entity has a position, and a boolean that represents whether it falls while not on a platform.

#pragma once

#include <cugl/cugl.h>

using namespace cugl;
using namespace std;

enum Direction {
	UP, DOWN, LEFT, RIGHT
};

class Entity {

protected:
	//Whether entity falls if nothing is below it
	//Fall once per world step
	bool _can_float = false;
	Vec2 _position = Vec2();

public:

	bool canFloat();

	void setCanFloat(bool floats);

	Vec2 getPosition();

	void move(Direction direction, int map_width);

};
