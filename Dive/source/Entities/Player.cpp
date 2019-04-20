#include "Player.h"

using namespace cugl;
using namespace std;

shared_ptr<Player> Player::alloc(Vec2 start_pos, Rect map_rect) {
    CULog("alloc in player");
	shared_ptr<Player> player = make_shared<Player>();
	player->initEntity(start_pos, Vec2(0.9, 0.9), map_rect);
	player->_box->setFriction(0);
	player->_box_dup->setFriction(0);
	player->setName("player");
    player->_box->setGravityScale(0);
    player->_box_dup->setGravityScale(0);
	player->_box->setRestitution(0);
	player->_box_dup->setRestitution(0);
	return player;
}

Direction Player::getCurrentDirection() {
	return _current_direction;
}

Direction Player::swapCurrentDirection() {
	if (_current_direction == Direction::RIGHT) {
		_current_direction = Direction::LEFT;
	}
	else {
		_current_direction = Direction::RIGHT;
	}
	return _current_direction;
}


void Player::reset() {
    _box->setPosition(_start_pos);
    _box_dup->setPosition(_start_pos);
    _alive = true;
}
