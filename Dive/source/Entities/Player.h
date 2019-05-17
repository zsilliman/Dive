// Player.h
// Represents the player.
// The player's default behavior is: dives (when possible).

#pragma once

#include <cugl/cugl.h>
#include "Entity.h"

#define PLAYER_HORIZONTAL_SPEED 2.2 //1.8
#define PLAYER_VERTICAL_SPEED 2

using namespace cugl;
using namespace std;

class Player : public Entity {

protected:

	Direction _current_direction = Direction::RIGHT;
    
public:

	shared_ptr<BoxObstacle> _sensor_left;
	shared_ptr<BoxObstacle> _sensor_left_dup;

	shared_ptr<BoxObstacle> _sensor_right;
	shared_ptr<BoxObstacle> _sensor_right_dup;
	
	Direction getCurrentDirection();

	Direction swapCurrentDirection();

	void initPhysics(shared_ptr<ObstacleWorld> world) override;

	void setLinearVelocity(Vec2 velocity) override;

	void updateSensors();

	static shared_ptr<Player> alloc(Vec2 start_pos, Rect map_rect);
    
    void reset();
};
