#include "GoalViewController.h"
#include "Util.h"

void GoalViewController::draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) {}

void GoalViewController::update(shared_ptr<GameState> state) {
    Keyboard* keyboard = Input::get<Keyboard>();
    if (keyboard->keyPressed(KeyCode::ARROW_LEFT)) {
        CULog("left");
        state->_goal_door->move(RIGHT_G, state->_map->getColumnCount());
        Vec2 tile_pos = state->_goal_door->getPosition();
        Vec2 map_pos = state->_map->tileToMapCoords(tile_pos.y, tile_pos.x, _node->getWidth());
        _node->setPosition(map_pos);
    }
    else if (keyboard->keyPressed(KeyCode::ARROW_RIGHT)) {
        CULog("right");
        state->_goal_door->move(LEFT_G, state->_map->getColumnCount());
        Vec2 tile_pos = state->_goal_door->getPosition();
        Vec2 map_pos = state->_map->tileToMapCoords(tile_pos.y, tile_pos.x, _node->getWidth());
        _node->setPosition(map_pos);
    }
}

void GoalViewController::dispose() {}

void GoalViewController::reset() {
	
}

shared_ptr<GoalViewController> GoalViewController::alloc(shared_ptr<GameState> init_state, shared_ptr<Texture> texture, Size display) {
    shared_ptr<GoalViewController> goal_vc = make_shared<GoalViewController>();
    float grid_size = display.width / init_state->_map->getColumnCount();
    Vec2 start_pos = init_state->_goal_door->getPosition();
    goal_vc->_node = PolygonNode::allocWithTexture(texture);
    goal_vc->_display = display;
    goal_vc->_node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    float scalex = grid_size / texture->getWidth();
    float scaley = grid_size / texture->getHeight();
    goal_vc->_node->setScale(scalex, scaley);
    goal_vc->_node->setPosition(init_state->_map->tileToMapCoords(start_pos.y, start_pos.x, grid_size));
    
    return goal_vc;
}
