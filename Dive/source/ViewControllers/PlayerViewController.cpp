#include "PlayerViewController.h"
#include "Util.h"

void PlayerViewController::draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) {}

void PlayerViewController::update(shared_ptr<GameState> state) {
	CULog("Player Update:");
	Vec2 start_pos = state->_player->getPosition();
	CULog(std::to_string(canMove(state, Direction::DOWN)).c_str());
	if (canMove(state, Direction::DOWN))
		fall(state);
	else {
		state->_player->move(state->_player->getCurrentDirection(), state->_map->getColumnCount());
	}
	Vec2 tile_pos = state->_player->getPosition();
	CULog(tile_pos.toString().c_str());
	Vec2 map_pos = state->_map->tileToMapCoords(tile_pos.y, tile_pos.x, _node->getWidth()) + Vec2(0, _display.height);
	_node->setPosition(map_pos);
	CULog(_node->getPosition().toString().c_str());
}

void PlayerViewController::dispose() {}

void PlayerViewController::reset() {
    _node->removeAllChildren();
}

bool PlayerViewController::canMove(shared_ptr<GameState> state, Direction direction) {
	int width = state->_map->getColumnCount();
	int height = state->_map->getRowCount();

	Vec2 position = state->_player->getPosition();
	int new_x = 0;

	switch (direction) {
	case UP:
		if (position.y - 1 < 0) return false;
		return state->_map->getBlock(position.y - 1, position.x) <= -1;
	case DOWN:
		if (position.y + 1 >= height) return false;
		return state->_map->getBlock(position.y + 1, position.x) <= -1;
	case LEFT:
		new_x = Util::mod(position.x - 1, width);
		return state->_map->getBlock(position.y, new_x) <= -1;
	case RIGHT:
		new_x = Util::mod(position.x + 1, width);
		return state->_map->getBlock(position.y, new_x) <= -1;
	}

}

void PlayerViewController::fall(shared_ptr<GameState> state) {
	//Continuous downward force
	if (state->_player->canFloat()) return;
	if (canMove(state, Direction::DOWN))
		state->_player->move(Direction::DOWN, state->_map->getColumnCount());
}

shared_ptr<PlayerViewController> PlayerViewController::alloc(shared_ptr<GameState> init_state, shared_ptr<Texture> texture, Size display) {
	shared_ptr<PlayerViewController> player_vc = make_shared<PlayerViewController>();
	float grid_size = display.width / init_state->_map->getColumnCount();
	Vec2 start_pos = init_state->_player->getPosition();
	player_vc->_node = PolygonNode::allocWithTexture(texture);
	player_vc->_display = display;
	player_vc->_node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	float scalex = grid_size / texture->getWidth();
	float scaley = grid_size / texture->getHeight();
	player_vc->_node->setScale(scalex, scaley);
	player_vc->_node->setPosition(init_state->_map->tileToMapCoords(start_pos.y, start_pos.x, grid_size) + Vec2(0,display.height));

	return player_vc;
}
