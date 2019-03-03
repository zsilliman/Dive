#pragma once

#include <cugl/cugl.h>
#include <cugl/2d/physics/CUBoxObstacle.h>
#include <cugl/2d/physics/CUCapsuleObstacle.h>
#include "World/PlatformMap.h"

using namespace cugl;
using namespace std;

#pragma mark Physics Constants

// Physics constants for initialization
#define ENEMY_FRICTION      1.0f
/** Density of non-urchin objects */
#define BASIC_DENSITY       1.0f
/** Density of the urchin objects */
#define URCHIN_DENSITY      1.0f
/** Friction of non-urchin objects */
#define BASIC_FRICTION      10.0f
/** Friction of the urchin objects */
#define URCHIN_FRICTION     0.7f
/** Angular damping of the urchin objects */
#define URCHIN_DAMPING      0.0f
/** Angular damping of the urchin objects */
#define URCHIN_INERTIA      0.001f
/** Collision restitution for all objects */
#define BASIC_RESTITUTION   0.1f

class Entity {

protected:
	shared_ptr<Obstacle> _body;
	float accelleration = 1.5f;
	float max_speed = 0.8f;

public:
	//This needs to be put into a separate View class but whatever
	shared_ptr<PolygonNode> _node;

	virtual void initPhysics(shared_ptr<ObstacleWorld> world);

	void setScale(float x, float y);

	void setPhysicsPosition(float x, float y);

	Vec2 getPhysicsPosition() { return _body->getPosition(); }

	void push(Vec2 force);

	void pushToDestination(Vec2 mouse_pos, shared_ptr<PlatformMap> platform_map);

	void pushToDestination(Vec2 destination);

	void pushDirection(Vec2 dir, shared_ptr<PlatformMap> platform_map);

	void updateNodePosition();

	string setBodyName(string name) { _body->setName(name); }

	string getBodyName() { return _body->getName(); }

	float getdX(float timestep) const { return _body->getLinearVelocity().x * timestep; }

};
