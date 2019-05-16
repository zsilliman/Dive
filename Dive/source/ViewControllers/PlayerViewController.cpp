#include "PlayerViewController.h"
#include "../Util.h"


void PlayerViewController::draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) {}

void PlayerViewController::update(shared_ptr<GameState> state) {
	state->_player->rotateEntity(state->_map->getMapRect());
	_oc_node->setPosition(state->_player->_box->getPosition() * _grid_size);
	_oc_node->setAngle(state->_player->_box->getAngle());

	//Set positions/rotations of duplicate according to duplicate physics object
	_dup_node->setPosition(state->_player->_box_dup->getPosition() * _grid_size);
	_dup_node->setAngle(state->_player->_box_dup->getAngle());

	if (_direction == "right") {
        _oc_node->setTexture(rev);
        _dup_node->setTexture(rev);
	}
	else {
        _oc_node->setTexture(normal);
        _dup_node->setTexture(normal);
	}

	state->_player->updateSensors();
    animatePlayer();
}


void PlayerViewController::dispose() {}

void PlayerViewController::reset() {
	_direction = "right";
}

void PlayerViewController::lose(shared_ptr<Texture> texture){

}

shared_ptr<PlayerViewController> PlayerViewController::alloc(shared_ptr<GameState> init_state, shared_ptr<Texture> texture, shared_ptr<Texture> reverse, Size display) {
	shared_ptr<PlayerViewController> player_vc = make_shared<PlayerViewController>();
	player_vc->_grid_size = display.width / init_state->_map->getWidth();
	player_vc->_node = Node::allocWithPosition(Vec2(0, 0));
	player_vc->_oc_node = AnimationNode::alloc(texture, 1, 10);
	player_vc->_oc_node->setPosition(init_state->_player->getPosition());
	player_vc->_oc_node->setScale(player_vc->_grid_size / texture->getHeight()*3, player_vc->_grid_size / texture->getHeight()*3);
	player_vc->_dup_node = AnimationNode::alloc(texture, 1, 10);
	player_vc->_dup_node->setScale(player_vc->_grid_size / texture->getHeight()*3, player_vc->_grid_size / texture->getHeight()*3);
	player_vc->_dup_node->setPosition(init_state->_player->getPosition());
    player_vc->_display = display;
    
    player_vc->normal = texture;
    player_vc->rev = reverse;
    
	player_vc->_node->addChild(player_vc->_oc_node);
	player_vc->_node->addChild(player_vc->_dup_node);
    
    player_vc->_mainCycle = true;
	player_vc->setAIDirection(init_state, "down");
	player_vc->_direction = "right";
    
	return player_vc;
}

void PlayerViewController::setFloor(bool f){
    _floor = f;
}

void PlayerViewController::animatePlayer(){
    if (_floor == false){
        _oc_node->setFrame(5);
        _dup_node->setFrame(5);
    }else{
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
        }else{
            _cooldown --;
        }
    }
}

void PlayerViewController::setAIDirection(shared_ptr<GameState> state, string direction) {
    if(direction == "down"){
        state->_player->setLinearVelocity(Vec2(0, -PLAYER_VERTICAL_SPEED));
    }
    else if(direction == "up"){
        state->_player->setLinearVelocity(Vec2(0, PLAYER_VERTICAL_SPEED));
    }
    else if(direction == "left"){
        state->_player->setLinearVelocity(Vec2(-PLAYER_HORIZONTAL_SPEED, state->_player->_box->getLinearVelocity().y));
		_direction = direction;
    }
    else if(direction == "right"){
        state->_player->setLinearVelocity(Vec2(PLAYER_HORIZONTAL_SPEED, state->_player->_box->getLinearVelocity().y));
		_direction = direction;
    }
}

string PlayerViewController::getAIDirection() { return _direction; }
