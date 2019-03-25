#include "GoalViewController.h"
#include "../Util.h"

void GoalViewController::update(shared_ptr<GameState> state) {
    state->_goal->rotateEntity(state->_map->getMapRect());
    
    _oc_node->setPosition(state->_goal->_box->getPosition() * _grid_size);
    _oc_node->setAngle(state->_goal->_box->getAngle());
    
    //Set positions/rotations of duplicate according to duplicate physics object
    _dup_node->setPosition(state->_goal->_box_dup->getPosition() * _grid_size);
    _dup_node->setAngle(state->_goal->_box_dup->getAngle());
}

void GoalViewController::draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) {}

void GoalViewController::dispose() {}

void GoalViewController::reset() {
    
}

shared_ptr<GoalViewController> GoalViewController::alloc(shared_ptr<GameState> init_state, shared_ptr<Texture> texture, Size display) {
    shared_ptr<GoalViewController> goal_vc = make_shared<GoalViewController>();
    goal_vc->_grid_size = display.width / init_state->_map->getWidth();
    goal_vc->_node = Node::allocWithPosition(Vec2(0, 0));
    goal_vc->_oc_node = PolygonNode::allocWithTexture(texture);
    goal_vc->_oc_node->setPosition(init_state->_goal->getPosition());
    goal_vc->_oc_node->setScale(goal_vc->_grid_size / texture->getWidth(), goal_vc->_grid_size / texture->getHeight());
    goal_vc->_dup_node = PolygonNode::allocWithTexture(texture);
    goal_vc->_dup_node->setScale(goal_vc->_grid_size / texture->getWidth(), goal_vc->_grid_size / texture->getHeight());
    goal_vc->_dup_node->setPosition(init_state->_goal->getPosition());
    goal_vc->_display = display;
    
    goal_vc->_node->addChild(goal_vc->_oc_node);
    goal_vc->_node->addChild(goal_vc->_dup_node);
    
    return goal_vc;
}
