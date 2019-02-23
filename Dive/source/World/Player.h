#pragma once

#include <cugl/cugl.h>

using namespace cugl;
using namespace std;

class Player {

protected:
	shared_ptr<BoxObstacle> _body;
	Vec2 linear_velocity;
	float accelleration = 30;
	float max_speed = 150;

public:
	shared_ptr<PolygonNode> _node;

	static shared_ptr<Player> allocWithTexture(shared_ptr<Texture> texture);

	void setScale(float x, float y);

	void setPosition(float x, float y);

	void push_direction(Vec2 dir);

	void initPhysics(shared_ptr<ObstacleWorld> world);

	void updatePosition();

	float getdX(float timestep) const { return _body->getLinearVelocity().x * timestep; }

};