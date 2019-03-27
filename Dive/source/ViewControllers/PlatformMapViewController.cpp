#include "PlatformMapViewController.h"

void PlatformMapViewController::parallaxTranslateVisible(shared_ptr<PlatformMap> map, Rect visible_rect, float reference_dx) {
	for (int i = 0; i < map->getPlatforms().size(); i++) {
		Rect platform_rect = map->getPlatforms()[i]->getPlatformRect();
		Rect platform_dup_rect = map->getPlatformDups()[i]->getPlatformRect();
		if (visible_rect.doesIntersect(platform_rect) || visible_rect.doesIntersect(platform_dup_rect)) {
			map->parallaxTranslatePlatform(i, reference_dx);
		}
	}
	Rect goal_rect = map->getGoal()->getPlatformRect();
	Rect goal_dup_rect = map->getGoalDup()->getPlatformRect();
	if (visible_rect.doesIntersect(goal_rect) || visible_rect.doesIntersect(goal_dup_rect)) {
		map->parallaxTranslateGoal(reference_dx);
	}
}

void PlatformMapViewController::draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) {}

void PlatformMapViewController::update(shared_ptr<GameState> state) {
	//Camera Controller
	Vec2 player_pos = state->_player->getPosition() * _grid_size;
	float map_height = state->_map->getHeight() * _grid_size;
	float node_pos = -player_pos.y + _display.height / 2;
	if (player_pos.y < _display.height / 2) {
		node_pos = 0;
	}
	else if (player_pos.y > map_height - _display.height / 2) {
		//Top portion
		node_pos = -(map_height - _display.height);
	}
	_node->setPositionY(node_pos);

	Rect visible_rect = Rect(Vec2(0, -_node->getPosition().y / _grid_size), _display / _grid_size);

    #if defined CU_TOUCH_SCREEN
    Touchscreen* touch = Input::get<Touchscreen>();
    if (touch->touchCount() == 1){
		parallaxTranslateVisible(state->_map, visible_rect, 2);
    }else if (touch->touchCount() == 2){
		parallaxTranslateVisible(state->_map, visible_rect, -2);
    }
    if (_input -> didPressLeft()){
		parallaxTranslateVisible(state->_map, visible_rect, -2);
    }
    else if (_input -> didPressRight()){
		parallaxTranslateVisible(state->_map, visible_rect, 2);
    }
    #else
	//Input controlling
    //keyboard controlled movement
    Keyboard* keyboard = Input::get<Keyboard>();
    if (keyboard->keyDown(KeyCode::ARROW_LEFT)) {
		parallaxTranslateVisible(state->_map, visible_rect, -2);
    }
    else if (keyboard->keyDown(KeyCode::ARROW_RIGHT)) {
		parallaxTranslateVisible(state->_map, visible_rect, 2);
    }
    else {
		parallaxTranslateVisible(state->_map, visible_rect, 0);
    }
//    if (_input->didPressLeft()){
//        state->_map->parallaxTranslatePlatforms(-2);
//    }else if (_input->didPressRight()){
//        state->_map->parallaxTranslatePlatforms(2);
//    }
    #endif
    state->_map->rotatePlatforms();
    for (int i = 0; i < _platforms.size(); i++) {
        _platforms[i]->update(state);
    }
    _goal->setPosition(state->_map->getGoal()->getPosition() * _grid_size);
    _goal_dup->setPosition(state->_map->getGoalDup()->getPosition() * _grid_size);
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

shared_ptr<PlatformMapViewController> PlatformMapViewController::alloc(shared_ptr<GameState> init_state, shared_ptr<InputController> _input, shared_ptr<TiledTexture> tilesheet, shared_ptr<Texture> goal_texture, Size display) {
	shared_ptr<PlatformMapViewController> map = make_shared<PlatformMapViewController>();
    map->_input = _input;
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
	
	//Setup goal nodes
	map->_goal = PolygonNode::allocWithTexture(goal_texture);
	map->_goal->setScale(map->_grid_size / goal_texture->getWidth());
	map->_goal->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	position = init_state->_map->getGoal()->getMinCorner() * map->_grid_size;
	map->_goal->setPosition(position);
	map->_node->addChild(map->_goal);

	map->_goal_dup = PolygonNode::allocWithTexture(goal_texture);
	map->_goal_dup->setScale(map->_grid_size / goal_texture->getWidth());
	map->_goal_dup->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	position = init_state->_map->getGoalDup()->getMinCorner() * map->_grid_size;
	map->_goal_dup->setPosition(position);
	map->_node->addChild(map->_goal_dup);

	return map;
}
