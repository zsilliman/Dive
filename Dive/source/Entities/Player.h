// Player.h
// Represents the player.
// The player's default behavior is: dives (when possible).

#pragma once

#include <cugl/cugl.h>
#include "Entity.h"

#define PLAYER_HORIZONTAL_SPEED 1.8
#define PLAYER_VERTICAL_SPEED 2

using namespace cugl;
using namespace std;

class Player : public Entity {

protected:

	Direction _current_direction = Direction::RIGHT;
    
    float startingAngle;

public:
	
	Direction getCurrentDirection();

	Direction swapCurrentDirection();

	static shared_ptr<Player> alloc(Vec2 start_pos, Rect map_rect);
    
    void reset();
};
