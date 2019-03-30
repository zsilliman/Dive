#include "FishViewController.h"
#include "../Util.h"

void FishViewController::draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) {}

void FishViewController::update(shared_ptr<GameState> state) {
	state->_fish[_fish_index]->setLinearVelocity(Vec2(1, 0));
	state->_fish[_fish_index]->rotateEntity(state->_map->getMapRect());
	//new version:
	_oc_node->setPosition(state->_fish[_fish_index]->_box->getPosition() * _grid_size);
	_oc_node->setAngle(state->_fish[_fish_index]->_box->getAngle());

	//Set positions/rotations of duplicate according to duplicate physics object
	_dup_node->setPosition(state->_fish[_fish_index]->_box_dup->getPosition() * _grid_size);
	_dup_node->setAngle(state->_fish[_fish_index]->_box_dup->getAngle());
    
    animateFish();
}

void FishViewController::dispose() {}

void FishViewController::reset() {

}

shared_ptr<FishViewController> FishViewController::alloc(shared_ptr<GameState> init_state, shared_ptr<Texture> texture, Size display, int fish_index) {
	shared_ptr<FishViewController> fish_vc = make_shared<FishViewController>();
	fish_vc->_fish_index = fish_index;
	fish_vc->_grid_size = display.width / init_state->_map->getWidth();
	fish_vc->_node = Node::allocWithPosition(Vec2(0, 0));
	fish_vc->_oc_node = AnimationNode::alloc(texture,1,1,1);
	fish_vc->_oc_node->setPosition(init_state->_fish[fish_index]->getPosition());
	fish_vc->_oc_node->setScale(fish_vc->_grid_size / texture->getWidth(), fish_vc->_grid_size / texture->getHeight());
	fish_vc->_dup_node = AnimationNode::alloc(texture,1,1,1);
	fish_vc->_dup_node->setScale(fish_vc->_grid_size / texture->getWidth(), fish_vc->_grid_size / texture->getHeight());
	fish_vc->_dup_node->setPosition(init_state->_fish[fish_index]->getPosition());
	fish_vc->_display = display;

	fish_vc->_node->addChild(fish_vc->_oc_node);
	fish_vc->_node->addChild(fish_vc->_dup_node);
    
    fish_vc->_mainCycle  = true;
    
	return fish_vc;
}

void FishViewController::animateFish(){
    bool* cycle = &_mainCycle;
    
    if (_oc_node->getFrame() == 0 || _oc_node->getFrame() == 1) {
        *cycle = true;
    } else if (_oc_node->getFrame() == _oc_node->getSize()-1) {
        *cycle = false;
    }
    
//    if (*cycle) {
//        _oc_node->setFrame(_oc_node->getFrame()+1);
//    } else {
//        _oc_node->setFrame(_oc_node->getFrame()-1);
//    }
//
    
    if (_dup_node->getFrame() == 0 || _dup_node->getFrame() == 1) {
        *cycle = true;
    } else if (_dup_node->getFrame() == _dup_node->getSize()-1) {
        *cycle = false;
    }
    
//    if (*cycle) {
//        _dup_node->setFrame(_dup_node->getFrame()+1);
//    } else {
//        _dup_node->setFrame(_dup_node->getFrame()-1);
//    }
}

void FishViewController::setDead(Obstacle* body){
//gives mem error
    //from viewcontroller
    //getNode()->g;
    //setPosition(-50, -50);
    //setVisible(false);
    body->setPosition(Vec2(0,0));
    CULog("killing fish");
}
