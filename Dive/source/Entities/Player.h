#pragma once

#include <cugl/cugl.h>
#include "Entity.h"

using namespace cugl;
using namespace std;

class Player : public Entity {

protected:

	Direction _current_direction = Direction::RIGHT;

public:
	
	Direction getCurrentDirection();

	Direction swapCurrentDirection();

	static shared_ptr<Player> alloc(Vec2 start_pos);

};
