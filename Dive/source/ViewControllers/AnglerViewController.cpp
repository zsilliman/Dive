#include "AnglerViewController.h"
#include "../Util.h"

void AnglerViewController::draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) {}

void AnglerViewController::update(shared_ptr<GameState> state) {
    
    if (_dead == true && _exp_node->getFrame() == _exp_node->getSize()-1){
        _exp_node->setVisible(false);
        _exp_dup_node->setVisible(false);
        _dead_ang->kill();
        _dead = false;
    }
    
	_node->setVisible(state->_anglers[_angler_index]->isAlive());

	if (!state->_anglers[_angler_index]->isAlive())
		return;

	state->_anglers[_angler_index]->setLinearVelocity(Vec2(0, 0));
	state->_anglers[_angler_index]->rotateEntity(state->_map->getMapRect());

	Vec2 player_pos = state->_player->_box->getPosition();
	Vec2 player_pos_dup = state->_player->_box_dup->getPosition();
	Vec2 angler_pos = state->_anglers[_angler_index]->_box->getPosition();
	Vec2 angler_pos_dup = state->_anglers[_angler_index]->_box_dup->getPosition();

	//Select the closest pair to compute direction of angler fish
	Vec2 closest_player = Vec2();
	Vec2 closest_angler = Vec2();
	Vec2 closest_angler1 = Vec2();
	Vec2 closest_angler2 = Vec2();

	//Original angler is closer to original player than duplicate angler
	if (player_pos.distance(angler_pos) < player_pos.distance(angler_pos_dup))
		closest_angler1 = angler_pos;
	else
		closest_angler1 = angler_pos_dup;
	//Compute distance to compare with closest to the player duplicate
	float dist1 = closest_angler1.distance(player_pos);
	
	//Original angler is closer to duplicate player than duplicate angler
	if (player_pos_dup.distance(angler_pos) < player_pos_dup.distance(angler_pos_dup))
		closest_angler2 = angler_pos;
	else
		closest_angler2 = angler_pos_dup;
	//Compute distance to compare with closest to the player original
	float dist2 = closest_angler2.distance(player_pos_dup);

	//Get the closest physics object position to the angler fish
	if (dist1 < dist2) {
		closest_player = player_pos;
		closest_angler = closest_angler1;
	} else {
		closest_player = player_pos_dup;
		closest_angler = closest_angler2;
	}

	//If the angler fish falls below the player's y coordinate and is within 5 tiles then it is active
	if (closest_player.y < closest_angler.y && closest_angler.y - closest_player.y < 5) {
		//Compute vector pointing from angler to the player
		Vec2 diff = closest_player - closest_angler;
		//Prevent divide by zero error
		if (diff.length() > 0.001)
			state->_anglers[_angler_index]->setLinearVelocity(ANGLER_MAX_SPEED * diff / diff.length());
	}

	//new version:
	_oc_node->setPosition(state->_anglers[_angler_index]->_box->getPosition() * _grid_size);
	_oc_node->setAngle(state->_anglers[_angler_index]->_box->getAngle());
    
    _exp_node->setPosition(state->_anglers[_angler_index]->_box->getPosition() * _grid_size);
    _exp_node->setAngle(state->_anglers[_angler_index]->_box->getAngle());

	Vec2 lin_vel = state->_anglers[_angler_index]->_box->getLinearVelocity();

	//Set positions/rotations of duplicate according to duplicate physics object
	_dup_node->setPosition(state->_anglers[_angler_index]->_box_dup->getPosition() * _grid_size);
    _exp_dup_node->setPosition(state->_anglers[_angler_index]->_box_dup->getPosition() * _grid_size);

	//Handle rotation of angler
	float angle = -PI;
	if (lin_vel.length() > 0.01)
		angle = lin_vel.getAngle();
	angle += PI;
	if (angle < PI/2 && angle > -PI/2) {
		_oc_node->flipVertical(false);
		_dup_node->flipVertical(false);
        _exp_node->flipVertical(false);
        _exp_dup_node->flipVertical(false);
	}
	else {
		_oc_node->flipVertical(true);
		_dup_node->flipVertical(true);
        _exp_node->flipVertical(true);
        _exp_dup_node->flipVertical(true);
	}
	_oc_node->setAngle(angle);
	_dup_node->setAngle(angle);
    _exp_node->setAngle(angle);
    _exp_dup_node->setAngle(angle);
    
    if(_dead){
        animateDead();
    }else{
        animateAngler();
    }
}

void AnglerViewController::dispose() {}

void AnglerViewController::reset() {

}

shared_ptr<AnglerViewController> AnglerViewController::alloc(shared_ptr<GameState> init_state, shared_ptr<Texture> texture, shared_ptr<Texture> explosion,  Size display, int angler_index) {
	shared_ptr<AnglerViewController> angler_vc = make_shared<AnglerViewController>();
	angler_vc->_angler_index = angler_index;
	angler_vc->_grid_size = display.width / init_state->_map->getWidth();
	angler_vc->_node = Node::allocWithPosition(Vec2(0, 0));
	angler_vc->_oc_node = AnimationNode::alloc(texture, 1, 14);
	angler_vc->_oc_node->setPosition(init_state->_anglers[angler_index]->getPosition());
	angler_vc->_oc_node->setScale(angler_vc->_grid_size / texture->getHeight(), angler_vc->_grid_size / texture->getHeight());
	angler_vc->_dup_node = AnimationNode::alloc(texture, 1, 14);
	angler_vc->_dup_node->setScale(angler_vc->_grid_size / texture->getHeight(), angler_vc->_grid_size / texture->getHeight());
	angler_vc->_dup_node->setPosition(init_state->_anglers[angler_index]->getPosition());
	angler_vc->_display = display;
    
    angler_vc->_exp_node = AnimationNode::alloc(explosion, 1, 8);
    angler_vc->_exp_node->setScale(angler_vc->_grid_size / explosion->getHeight()*3, angler_vc->_grid_size / explosion->getHeight()*3);
    angler_vc->_exp_node->setPosition(init_state->_anglers[angler_index]->getPosition());
    angler_vc->_exp_node->setVisible(false);
    
    angler_vc->_exp_dup_node = AnimationNode::alloc(explosion, 1, 8);
    angler_vc->_exp_dup_node->setScale(angler_vc->_grid_size / explosion->getHeight()*3, angler_vc->_grid_size / explosion->getHeight()*3);
    angler_vc->_exp_dup_node->setPosition(init_state->_anglers[angler_index]->getPosition());
    angler_vc->_exp_dup_node->setVisible(false);
    
    angler_vc->_dead = false;

	angler_vc->_node->addChild(angler_vc->_oc_node);
	angler_vc->_node->addChild(angler_vc->_dup_node);
    angler_vc->_node->addChild(angler_vc->_exp_node);
    angler_vc->_node->addChild(angler_vc->_exp_dup_node);

	angler_vc->_mainCycle = true;

	return angler_vc;
}

void AnglerViewController::animateDead() {
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

void AnglerViewController::animateAngler() {
	bool* cycle = &_mainCycle;
    if (_cooldown == 0){
        _cooldown = 3;
        
        if (_oc_node->getFrame() == 0 || _oc_node->getFrame() == 1) {
            *cycle = true;
        }
        else if (_oc_node->getFrame() == _oc_node->getSize() - 1) {
            *cycle = false;
        }

        if (*cycle) {
            _oc_node->setFrame(_oc_node->getFrame()+1);
        } else {
            _oc_node->setFrame(_oc_node->getFrame()-1);
        }

        if (_dup_node->getFrame() == 0 || _dup_node->getFrame() == 1) {
            *cycle = true;
        }
        else if (_dup_node->getFrame() == _dup_node->getSize() - 1) {
            *cycle = false;
        }

        if (*cycle) {
            _dup_node->setFrame(_dup_node->getFrame()+1);
        } else {
            _dup_node->setFrame(_dup_node->getFrame()-1);
        }
        
    } else {
        _cooldown --;
    }
}

void AnglerViewController::kill(shared_ptr<Angler> angler) {
	CULog("killing angler fish");
    _dead_ang = angler;
    _oc_node->setVisible(false);
    _dup_node->setVisible(false);
    _exp_node->setVisible(true);
    _exp_dup_node->setVisible(true);
    _mainCycle = true;
    _cooldown = 0;
    _dead = true;
}

void AnglerViewController::revive(shared_ptr<Angler> angler) {
	CULog("reviving angler fish");
	angler->revive();
}
