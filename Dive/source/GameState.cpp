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