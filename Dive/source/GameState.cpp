#include "GameState.h"

using namespace cugl;
using namespace std;

shared_ptr<GameState> GameState::allocEmpty() {
	shared_ptr<GameState> state = make_shared<GameState>();
	return state;
}

shared_ptr<GameState> GameState::allocWithLevel(string level_asset, shared_ptr<AssetManager> assets) {
	shared_ptr<GameState> state = make_shared<GameState>();
	// todo...
	return state;
}

void GameState::dispose() {
		
}

void GameState::reset(){
	_map->reset();
	/*_player->reset();
	_goal_door->reset();
	for (int i = 0; i < _urchins.size(); i++) {
		_urchins[i]->reset();
	}*/
}

bool GameState::GoalCollision(){
	return false;//_player->getPosition() == _goal_door->getPosition();
}
