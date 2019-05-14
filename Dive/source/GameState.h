#pragma once

#include <cugl/cugl.h>
#include "Util.h"
#include "World/PlatformMap.h"
#include "Entities/Player.h"
#include "Entities/Urchin.h"
#include "Entities/Fish.h"
#include "Entities/Angler.h"

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
	/** A reference to each of the Angler fish */
	vector<shared_ptr<Angler>> _anglers = {};

	/** Allocate gamestate, then manually set values */
	static shared_ptr<GameState> allocEmpty();

	/** Allocate gamestate from a file */
	static shared_ptr<GameState> allocFromJson(shared_ptr<JsonValue> json);

	void initPhysics(shared_ptr<ObstacleWorld> world);

	void dispose();
    
    void reset();
    
    void setLevel(string level);

};

