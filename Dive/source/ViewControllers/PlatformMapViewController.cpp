#include "PlatformMapViewController.h"

void PlatformMapViewController::draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) {}

void PlatformMapViewController::update(shared_ptr<GameState> state) {
    ///New changes--
    Keyboard* keyboard = Input::get<Keyboard>();
    if (keyboard->keyDown(KeyCode::ARROW_LEFT)) {
        CULog("left");
        state->_map->parallaxTranslatePlatforms(-1);
        for (int i = 0; i < _platforms.size(); i++) {
            _platforms[i]->update(state);
        }
    }
    else if (keyboard->keyDown(KeyCode::ARROW_RIGHT)) {
        CULog("right");
        state->_map->parallaxTranslatePlatforms(1);
        for (int i = 0; i < _platforms.size(); i++) {
            _platforms[i]->update(state);
        }
    }
    else{
        state->_map->parallaxTranslatePlatforms(0);
        for (int i = 0; i < _platforms.size(); i++) {
            _platforms[i]->update(state);
        }
    }
    //previous code--
//    state->_map->parallaxTranslatePlatforms(1);
//    for (int i = 0; i < _platforms.size(); i++) {
//        _platforms[i]->update(state);
//    }
	state->_map->rotatePlatforms();

	//Camera Controller
	Vec2 player_pos = state->_player->getPosition() * _grid_size;
	float map_height = state->_map->getHeight() * _grid_size;
	float node_pos = -player_pos.y + _display.height / 2;
	if (player_pos.y < _display.height/2) {
		node_pos = 0;
		CULog("a");
	}
	else if (player_pos.y > map_height - _display.height / 2) {
		//Top portion
		node_pos = -(map_height - _display.height);
		CULog("b");
	}
	_node->setPositionY(node_pos);

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
	map->_display = display;
	map->_grid_size = display.width / init_state->_map->getWidth();
	for (int i = 0; i < init_state->_map->getPlatformDups().size(); i++) {
		shared_ptr<PlatformViewController> platform = PlatformViewController::alloc(init_state, tilesheet, map->_grid_size, i);
		map->_node->addChild(platform->getNode(), 1);
		map->_platforms.push_back(platform);
	}
	Vec2 position = Vec2(0, -init_state->_map->getHeight() * map->_grid_size + display.height);
	map->_node->setPosition(position);
	return map;
}
