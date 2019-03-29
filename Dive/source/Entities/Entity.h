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
	bool _can_float = false, _alive = true, _active_box = 0;
	Vec2 _position = Vec2();
	Vec2 _start_pos = Vec2();
	Size _dimensions = Size(0, 0);

	bool overlapsLeftEdge(Rect entity_rect, Rect map_rect);

	bool overlapsRightEdge(Rect entity_rect, Rect map_rect);

	Rect getBoxRect(shared_ptr<Obstacle> box);

public:

	shared_ptr<Obstacle> _box, _box_dup;

	void rotateEntity(Rect map_rect);

	bool canFloat();

	void setCanFloat(bool floats);

	void setName(string name);

	bool isAlive();
	virtual void kill();
	virtual void revive();

	string getName();

	Vec2 getPosition();

	void setPosition(Vec2 position);

	void setLinearVelocity(Vec2 velocity) { 
		_box->setLinearVelocity(velocity);
		_box_dup->setLinearVelocity(velocity);
	}

	void initPhysics(shared_ptr<ObstacleWorld> world) {
		world->addObstacle(_box);
		world->addObstacle(_box_dup);
	}

	void initEntity(Vec2 pos, Vec2 size, Rect map_rect);

	void reset();
    
};
