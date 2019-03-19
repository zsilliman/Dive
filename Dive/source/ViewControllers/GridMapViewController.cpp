#include "GridMapViewController.h"

void GridMapViewController::update(shared_ptr<GameState> state) {
	/*Keyboard* keyboard = Input::get<Keyboard>();
	if (keyboard->keyPressed(KeyCode::ARROW_LEFT)) {
		CULog("left");
		state->_map->translate(-1);
		for (int i = 0; i < _rows.size(); i++) {
			_rows[i]->update(state);
		}
	}
	else if (keyboard->keyPressed(KeyCode::ARROW_RIGHT)) {
		CULog("right");
		state->_map->translate(1);
		for (int i = 0; i < _rows.size(); i++) {
			_rows[i]->update(state);
		}
	}
	//Camera Controller
	Vec2 player_tile = state->_player->getPosition();
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
	_node->setPositionY(node_pos);*/
}

void GridMapViewController::updateRows(shared_ptr<GameState> state) {
	for (int i = 0; i < _rows.size(); i++) {
		_rows[i]->update(state);
	}
}

void GridMapViewController::draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) {
	for (int i = 0; i < _rows.size(); i++) {
		_rows[i]->draw(batch, state);
	}
}

void GridMapViewController::dispose() {

}

void GridMapViewController::reset() {

    _node->removeAllChildren();
}

shared_ptr<GridMapViewController> GridMapViewController::alloc(shared_ptr<GameState> init_state) {
	shared_ptr<GridMapViewController> map = make_shared<GridMapViewController>();
	return map;
}

shared_ptr<GridMapViewController> GridMapViewController::alloc(shared_ptr<GameState> init_state, shared_ptr<TiledTexture> tilesheet, Size display) {
	shared_ptr<GridMapViewController> map = make_shared<GridMapViewController>();
	map->init(init_state, tilesheet, display);
	return map;
}

void GridMapViewController::init(shared_ptr<GameState> init_state, shared_ptr<TiledTexture> tilesheet, Size display) {
	/*_node = Node::alloc();
	int rows = init_state->_map->getRowCount();
	int cols = init_state->_map->getColumnCount();
	float grid_size = display.width / cols;
	_grid_size = grid_size;
	_display = display;

	for (int row = 0; row < rows; row++) {
		shared_ptr<GridRowViewController> row_vc = GridRowViewController::alloc(init_state, tilesheet, row, grid_size);
		float y = init_state->_map->tileToMapCoords(row, 0, grid_size).y;
		row_vc->setPosition(0, y);
		row_vc->addViewToNode(_node);
		_rows.push_back(row_vc);
	}
	_node->setPosition(0, display.height);*/
}
