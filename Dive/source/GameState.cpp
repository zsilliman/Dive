#include "GameState.h"

using namespace cugl;
using namespace std;

shared_ptr<GameState> GameState::allocEmpty() {
	shared_ptr<GameState> state = make_shared<GameState>();
	return state;
}

shared_ptr<GameState> GameState::allocWithLevel(string level_asset, shared_ptr<AssetManager> assets) {
	shared_ptr<GameState> state = make_shared<GameState>();
	state->_player == nullptr;
	// todo...
	std::shared_ptr<JsonReader> reader = JsonReader::allocWithAsset(level_asset);
	shared_ptr<JsonValue> json = reader->readJson();
	state->_map = PlatformMap::parseFromJSON(json, assets);

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
	//_goal_door->reset();
	for (int i = 0; i < _urchins.size(); i++) {
		_urchins[i]->reset();
	}
	for (int i = 0; i < _fish.size(); i++) {
		_fish[i]->reset();
	}
}

bool GameState::GoalCollision(){
	return false;//_player->getPosition() == _goal_door->getPosition();
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
