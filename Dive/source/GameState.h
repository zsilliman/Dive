#pragma once

#include <cugl/cugl.h>
#include "World/PlatformMap.h"
#include "Entities/Player.h"
using namespace cugl;
using namespace std;

class GameState {

public:

	/** A reference to the map that all the platforms are put on */
	shared_ptr<PlatformMap> _map;
    /** A reference to the player */
    shared_ptr<Player> _player;
	/**

	A reference to the goal location 
	shared_ptr<Goal> _goal_door;

	A reference to a list of all urchins 
	vector<shared_ptr<Urchin>> _urchins;

	A reference to a list of all enemies
	vector<shared_ptr<Enemy>> _enemies;*/

	/** Allocate gamestate, then manually set values */
	static shared_ptr<GameState> allocEmpty();

	/** Allocate gamestate from a file */
	static shared_ptr<GameState> allocWithLevel(string level_asset, shared_ptr<AssetManager> assets);

	void dispose();
    
    void reset();
    
    bool GoalCollision();

};
