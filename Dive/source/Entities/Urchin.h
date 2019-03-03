#pragma once

#include <cugl/cugl.h>
#include <cugl/2d/physics/CUBoxObstacle.h>
#include <cugl/2d/physics/CUCapsuleObstacle.h>
#include "Entity.h"

using namespace cugl;
using namespace std;

class Urchin : public Entity {

public:

	static shared_ptr<Urchin> allocWithTexture(shared_ptr<Texture> texture);

	void initPhysics(shared_ptr<ObstacleWorld> world);

};
