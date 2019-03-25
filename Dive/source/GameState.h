#pragma once

#include <cugl/cugl.h>
#include "Util.h"
#include "World/PlatformMap.h"
#include "Entities/Player.h"
#include "Entities/Urchin.h"
#include "Entities/Fish.h"
#include "World/Goal.h"

using namespace cugl;
using namespace std;

class GameState {

public:

	/** A reference to the map that all the platforms are put on */
	shared_ptr<PlatformMap> _map;
    /** A reference to the player */
    shared_ptr<Player> _player;
	/** A reference to each of the Urchins */
	vector<shared_ptr<Urchin>> _urchins = {};
	/** A reference to each of the Fish */
	vector<shared_ptr<Fish>> _fish = {};
    
    shared_ptr<Goal> _goal;


	/**
	A reference to the goal location 

	A reference to a list of all urchins 
	vector<shared_ptr<Urchin>> _urchins;

	A reference to a list of all enemies
	vector<shared_ptr<Enemy>> _enemies;*/

	/** Allocate gamestate, then manually set values */
	static shared_ptr<GameState> allocEmpty();

	/** Allocate gamestate from a file */
	static shared_ptr<GameState> allocWithLevel(string level_asset, shared_ptr<AssetManager> assets);

	void initPhysics(shared_ptr<ObstacleWorld> world);

	void dispose();
    
    void reset();
    
    bool GoalCollision();

};
