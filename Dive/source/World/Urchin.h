#pragma once

#include <cugl/cugl.h>
#include <cugl/2d/physics/CUBoxObstacle.h>
#include <cugl/2d/physics/CUCapsuleObstacle.h>
#include "PlatformMap.h"

using namespace cugl;
using namespace std;

class Urchin {

protected:
	shared_ptr<WheelObstacle> _body;
	Vec2 _destination;
	float accelleration = 60;
	float max_speed = 150;

public:
	//This needs to be put into a separate View class but whatever
	shared_ptr<PolygonNode> _node;

	static shared_ptr<Urchin> allocWithTexture(shared_ptr<Texture> texture);

	void setScale(float x, float y);

	void setPhysicsPosition(float x, float y);

	Vec2 getPhysicsPosition() { return _body->getPosition(); }

	Vec2 get_push_destination() { return _destination; }

	void push(Vec2 force);

	/*void setPushDestination(Vec2 mouse_pos, shared_ptr<PlatformMap> platform_map);

	void pushToDestination();

	void pushDirection(Vec2 dir, shared_ptr<PlatformMap> platform_map);
*/
	void initPhysics(shared_ptr<ObstacleWorld> world);

	void updatePosition();

	float getdX(float timestep) const { return _body->getLinearVelocity().x * timestep; }

};
