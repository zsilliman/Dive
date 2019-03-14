#include "Player.h"

using namespace cugl;
using namespace std;

shared_ptr<Player> Player::alloc(Vec2 start_pos) {
	shared_ptr<Player> player = make_shared<Player>();
	player->_position.set(start_pos);
//    player->_current_direction = Direction::RIGHT;
	player->_start_pos.set(start_pos);
//    player->setCanFloat(false);
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
