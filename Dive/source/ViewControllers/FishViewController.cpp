#include "FishViewController.h"
#include "../Util.h"

void FishViewController::draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) {}

void FishViewController::update(shared_ptr<GameState> state) {
    
    if (_dead == true && _exp_node->getFrame() == _exp_node->getSize()-1){
        _exp_node->setVisible(false);
        _exp_dup_node->setVisible(false);
        _dead_fish->kill();
        _dead = false;
    }
    
	_node->setVisible(state->_fish[_fish_index]->isAlive());
	if (!state->_fish[_fish_index]->isAlive())
		return;

	Vec2 vel = Vec2(1, 0);
	if (state->_fish[_fish_index]->isLeft()) {
		vel = vel * -1;
	}
	_oc_node->flipHorizontal(!state->_fish[_fish_index]->isLeft());
	_dup_node->flipHorizontal(!state->_fish[_fish_index]->isLeft());
    _exp_node->flipHorizontal(!state->_fish[_fish_index]->isLeft());
    _exp_dup_node->flipHorizontal(!state->_fish[_fish_index]->isLeft());


	state->_fish[_fish_index]->setLinearVelocity(vel);
	state->_fish[_fish_index]->rotateEntity(state->_map->getMapRect());
	//new version:
	_oc_node->setPosition(state->_fish[_fish_index]->_box->getPosition() * _grid_size);
	_oc_node->setAngle(state->_fish[_fish_index]->_box->getAngle());

	//Set positions/rotations of duplicate according to duplicate physics object
	_dup_node->setPosition(state->_fish[_fish_index]->_box_dup->getPosition() * _grid_size);
	_dup_node->setAngle(state->_fish[_fish_index]->_box_dup->getAngle());
    
    _exp_node->setPosition(state->_fish[_fish_index]->_box->getPosition() * _grid_size);
    _exp_node->setAngle(state->_fish[_fish_index]->_box->getAngle());
    
    _exp_dup_node->setPosition(state->_fish[_fish_index]->_box_dup->getPosition() * _grid_size);
    _exp_dup_node->setAngle(state->_fish[_fish_index]->_box_dup->getAngle());
    
    if(_dead){
        animateDead();
    }else{
        animateFish();
    }

}

void FishViewController::setInitialVelocity(shared_ptr<GameState> state, Vec2 vel){
    state->_fish[_fish_index]->setLinearVelocity(vel);
}

void FishViewController::dispose() {}

void FishViewController::reset() {
    CULog("fish vc reset");
}

shared_ptr<FishViewController> FishViewController::alloc(shared_ptr<GameState> init_state, shared_ptr<Texture> texture, shared_ptr<Texture> explosion, Size display, int fish_index) {
	shared_ptr<FishViewController> fish_vc = make_shared<FishViewController>();
	fish_vc->_fish_index = fish_index;
	fish_vc->_grid_size = display.width / init_state->_map->getWidth();
	fish_vc->_node = Node::allocWithPosition(Vec2(0, 0));
	fish_vc->_oc_node = AnimationNode::alloc(texture,1,10);
	fish_vc->_oc_node->setPosition(init_state->_fish[fish_index]->getPosition());
	fish_vc->_oc_node->setScale(fish_vc->_grid_size / texture->getHeight()*3, fish_vc->_grid_size / texture->getHeight()*3);
	fish_vc->_dup_node = AnimationNode::alloc(texture,1,10);
	fish_vc->_dup_node->setScale(fish_vc->_grid_size / texture->getHeight()*3, fish_vc->_grid_size / texture->getHeight()*3);
	fish_vc->_dup_node->setPosition(init_state->_fish[fish_index]->getPosition());
	fish_vc->_display = display;
    
    fish_vc->_exp_node = AnimationNode::alloc(explosion, 1, 8);
    fish_vc->_exp_node->setPosition(init_state->_fish[fish_index]->getPosition());
    fish_vc->_exp_node->setScale(fish_vc->_grid_size / explosion->getHeight()*3, fish_vc->_grid_size / explosion->getHeight()*3);
    fish_vc->_exp_node->setVisible(false);
    
    fish_vc->_exp_dup_node = AnimationNode::alloc(explosion, 1, 8);
    fish_vc->_exp_dup_node->setPosition(init_state->_fish[fish_index]->getPosition());
    fish_vc->_exp_dup_node->setScale(fish_vc->_grid_size / explosion->getHeight()*3, fish_vc->_grid_size / explosion->getHeight()*3);
    fish_vc->_exp_dup_node->setVisible(false);
    
    fish_vc->_dead = false;

	fish_vc->_node->addChild(fish_vc->_oc_node);
	fish_vc->_node->addChild(fish_vc->_dup_node);
    fish_vc->_node->addChild(fish_vc->_exp_node);
    fish_vc->_node->addChild(fish_vc->_exp_dup_node);
    
    fish_vc->_mainCycle  = true;
    
	return fish_vc;
}


void FishViewController::animateDead() {
    bool* cycle = &_mainCycle;
    if (_cooldown == 0){
        _cooldown = 1;
        
        if (_exp_node->getFrame() == 0 || _exp_node->getFrame() == 1) {
            *cycle = true;
        } else if (_exp_node->getFrame() == _exp_node->getSize()-1) {
            *cycle = false;
        }
        
        if (*cycle) {
            _exp_node->setFrame(_exp_node->getFrame()+1);
        } else {
            _exp_node->setFrame(_exp_node->getFrame()-1);
        }
        
        if (_exp_dup_node->getFrame() == 0 || _exp_dup_node->getFrame() == 1) {
            *cycle = true;
        } else if (_exp_dup_node->getFrame() == _exp_dup_node->getSize()-1) {
            *cycle = false;
        }
        
        if (*cycle) {
            _exp_dup_node->setFrame(_exp_dup_node->getFrame()+1);
        } else {
            _exp_dup_node->setFrame(_exp_dup_node->getFrame()-1);
        }
    } else{
        _cooldown --;
    }
}


void FishViewController::animateFish(){
    bool* cycle = &_mainCycle;
    if (_cooldown == 0){
        _cooldown = 3;
        if (_oc_node->getFrame() == 0 || _oc_node->getFrame() == 1) {
            *cycle = true;
        } else if (_oc_node->getFrame() == _oc_node->getSize()-1) {
            *cycle = false;
        }
        
        if (*cycle) {
            _oc_node->setFrame(_oc_node->getFrame()+1);
        } else {
            _oc_node->setFrame(_oc_node->getFrame()-1);
        }
        
        if (_dup_node->getFrame() == 0 || _dup_node->getFrame() == 1) {
            *cycle = true;
        } else if (_dup_node->getFrame() == _dup_node->getSize()-1) {
            *cycle = false;
        }
    
        if (*cycle) {
            _dup_node->setFrame(_dup_node->getFrame()+1);
        } else {
            _dup_node->setFrame(_dup_node->getFrame()-1);
        }
        
    }else {
        _cooldown --;
    }
}

void FishViewController::kill(shared_ptr<Fish> fish) {
	CULog("killing fish");
    _dead_fish = fish;
    _oc_node->setVisible(false);
    _dup_node->setVisible(false);
    _exp_node->setVisible(true);
    _exp_dup_node->setVisible(true);
    _mainCycle = true;
    _cooldown = 0;
    _dead = true;
}

void FishViewController::revive(shared_ptr<Fish> fish) {
	CULog("reviving fish");
	fish->revive();
}

void FishViewController::setDirection(shared_ptr<Fish> fish) {
    CULog("changing fish direction");
    fish->changeDirection();
}
