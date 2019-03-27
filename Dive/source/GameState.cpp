#include "GameState.h"

using namespace cugl;
using namespace std;

shared_ptr<GameState> GameState::allocEmpty() {
	shared_ptr<GameState> state = make_shared<GameState>();
	return state;
}

shared_ptr<GameState> GameState::allocFromJson(shared_ptr<JsonValue> json) {
	shared_ptr<GameState> state = make_shared<GameState>();
	state->_player == nullptr;
	state->_map = nullptr;
	state->_fish = {};
	state->_urchins = {};
	state->_map = PlatformMap::parseFromJSON(json);

	shared_ptr<JsonValue> obj_layer = json->get("layers")->get(1);
	Size dimen = Size(state->_map->getWidth(), state->_map->getHeight());
	vector<int> data = obj_layer->get("data")->asIntArray();

	for (int y = 0; y < dimen.height; y++) {
		for (int x = 0; x < dimen.width; x++) {
			int index = (dimen.height - y - 1) * dimen.width + x;
			//If a block exists
			if (data[index] == PLAYER_TILE_ID && state->_player==nullptr) {
				//Player
				state->_player = Player::alloc(Vec2(x,y), state->_map->getMapRect());
			} else if (data[index] == URCHIN_TILE_ID) {
				//Urchin
				shared_ptr<Urchin> urchin = Urchin::alloc(Vec2(x, y), state->_map->getMapRect());
				state->_urchins.push_back(urchin);
			} else if (data[index] == FISH_TILE_ID) {
				//Fish
				shared_ptr<Fish> fish = Fish::alloc(Vec2(x, y), state->_map->getMapRect());
				state->_fish.push_back(fish);
			} else if (data[index] == JELLY_TILE_ID) {
				//Jellyfish (tbd)
				//...
            }else if (data[index] == GOAL_TILE_ID) {
                //Goal (this is generated within the PlatformMap class since they move like platforms not entities)
            }
		}
	}
	

	return state;
}

void GameState::dispose() {
		
}

void GameState::reset() {
	_map->reset();
	_player->reset();
	for (int i = 0; i < _urchins.size(); i++) {
		_urchins[i]->reset();
	}
	for (int i = 0; i < _fish.size(); i++) {
		_fish[i]->reset();
	}
}

void GameState::initPhysics(const shared_ptr<ObstacleWorld> world) {
	_map->initPhysics(world);
	_player->initPhysics(world);
    
	for (int i = 0; i < _urchins.size(); i++) {
		_urchins[i]->initPhysics(world);
	}
	for (int i = 0; i < _fish.size(); i++) {
		_fish[i]->initPhysics(world);
	}
}
