#include "PlatformMapViewController.h"

void PlatformMapViewController::draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) {}

void PlatformMapViewController::update(shared_ptr<GameState> state) {
	for (int i = 0; i < _platforms.size(); i++) {
		_platforms[i]->update(state);
	}

	//Camera Controller
	/*Vec2 player_tile = state->_player->getPosition();
	Vec2 player_pos = state->_map->tileToMapCoords(player_tile.y, player_tile.x, _grid_size);
	float map_height = state->_map->getRowCount() * _grid_size;
	float node_pos = -player_pos.y + _display.height / 2;
	if (player_pos.y < -map_height + _display.height / 2) {
		node_pos = map_height;
	}
	else if (player_pos.y > -_display.height / 2) {
		//Top portion
		node_pos = _display.height;
	}
	_node->setPositionY(node_pos); */

}

void PlatformMapViewController::dispose() {
	for (int i = 0; i < _platforms.size(); i++) {
		_platforms[i]->dispose();
	}
}

void PlatformMapViewController::reset() {
	for (int i = 0; i < _platforms.size(); i++) {
		_platforms[i]->reset();
	}
}

shared_ptr<PlatformMapViewController> PlatformMapViewController::alloc(shared_ptr<GameState> init_state, shared_ptr<TiledTexture> tilesheet, Size display) {
	shared_ptr<PlatformMapViewController> map = make_shared<PlatformMapViewController>();
	map->_node = Node::alloc();
	map->_platforms = {};
	map->_grid_size = display.width / init_state->_map->getWidth();
	for (int i = 0; i < init_state->_map->getPlatforms().size(); i++) {
		shared_ptr<PlatformViewController> platform = PlatformViewController::alloc(init_state->_map->getPlatforms()[i], tilesheet, map->_grid_size);
		map->_node->addChild(platform->getNode(), 1);
		map->_platforms.push_back(platform);
	}
	Vec2 position = Vec2(0, -init_state->_map->getHeight() * map->_grid_size + display.height);
	map->_node->setPosition(position);
	return map;
}

