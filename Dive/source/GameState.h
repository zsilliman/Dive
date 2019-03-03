#pragma once

#include <cugl/cugl.h>
#include "World/PlatformMap.h"
#include "Entities/Player.h"
#include "Entities/Urchin.h"
#include "World/Goal.h"
#include "Entities/Enemy.h"

using namespace cugl;
using namespace std;

class GameState {

public:
	static shared_ptr<GameState> allocEmpty();

	static shared_ptr<GameState> allocWithLevel(string file);

	/** A scene graph, used to display our 2D scenes */
	shared_ptr<cugl::Scene> _scene;
	/** A 3152 style SpriteBatch to render the scene */
	shared_ptr<cugl::SpriteBatch>  _batch;

	/** A reference to the map that all the platforms are put on */
	shared_ptr<PlatformMap> _platform_map;

	/** A reference to the physics world that all the platforms and player are put on */
	shared_ptr<cugl::ObstacleWorld> _world;

	/** A reference to the player */
	shared_ptr<Player> _player;

	shared_ptr<Goal> _goalDoor;

	/** A reference to a list of all urchins */
	vector<shared_ptr<Urchin>> _urchins;

	/** A reference to a list of all urchins */
	vector<shared_ptr<Enemy>> _enemies;

};