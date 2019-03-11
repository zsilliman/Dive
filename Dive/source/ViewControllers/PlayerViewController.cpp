#include "PlayerViewController.h"
#include "Util.h"

void PlayerViewController::draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) {}

void PlayerViewController::update(shared_ptr<GameState> state) {
	Vec2 start_pos = state->_player->getPosition();
	//CULog("start_pos x, y: %d %d", start_pos.x, start_pos.y);
	if (isBlocked(state)) {
		CULog("should move but won't");
		state->_player->move(Direction::RIGHT, state->_map->getColumnCount());
	}

	else if (canMove(state, Direction::DOWN)) {
		fall(state);
	}
	else {
		int row = start_pos.y;
		//CULog("current row %d row count %d", row, state->_map->getRowCount());
		if (canMove(state, state->_player->getCurrentDirection()))
		{
			state->_player->move(state->_player->getCurrentDirection(), state->_map->getColumnCount());
		}
	}
	Vec2 tile_pos = state->_player->getPosition();
	//CULog(tile_pos.toString().c_str());
	Vec2 map_pos = state->_map->tileToMapCoords(tile_pos.y, tile_pos.x, _node->getWidth());
	_node->setPosition(map_pos);
	//CULog(_node->getPosition().toString().c_str());
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

bool PlayerViewController::isBlocked(shared_ptr<GameState> state) {
	int x = state->_player->getPosition().x;
	int y = state->_player->getPosition().y;
	CULog("x: %d y: %d", x, y);
	CULog("block: %d", state->_map->getBlock(y, x));
	bool val = state->_map->getBlock(y, x) >= 0;
	CULog("VAL: %d", val);
	return val;
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
	player_vc->_node->setPosition(init_state->_map->tileToMapCoords(start_pos.y, start_pos.x, grid_size));

	return player_vc;
}
