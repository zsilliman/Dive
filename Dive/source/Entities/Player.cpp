#include "Player.h"

using namespace cugl;
using namespace std;

shared_ptr<Player> Player::alloc(Vec2 start_pos, Rect map_rect) {
	shared_ptr<Player> player = make_shared<Player>();
	Size size = Vec2(.6, 1.43);
	player->initEntity(start_pos, size, map_rect);
	player->_box->setFriction(0);
	player->_box_dup->setFriction(0);
	player->setName("player");
    player->_box->setGravityScale(0);
    player->_box_dup->setGravityScale(0);
	player->_box->setRestitution(0);
	player->_box_dup->setRestitution(0);
	//Side sensor detection
	player->_sensor_left = BoxObstacle::alloc(player->_position - size/2, Size(size.width * .05, size.height/2));
	player->_sensor_left->setSensor(true);
	player->_sensor_left->setName("player left side");

	player->_sensor_right = BoxObstacle::alloc(player->_position + size / 2, Size(size.width * .05, size.height / 2));
	player->_sensor_right->setSensor(true);
	player->_sensor_right->setName("player right side");

	//Side sensor detection duplicates
	player->_sensor_left_dup = BoxObstacle::alloc(player->_position - size / 2, Size(size.width * .05, size.height / 2));
	player->_sensor_left_dup->setSensor(true);
	player->_sensor_left_dup->setName("player left side");

	player->_sensor_right_dup = BoxObstacle::alloc(player->_position + size / 2, Size(size.width * .05, size.height / 2));
	player->_sensor_right_dup->setSensor(true);
	player->_sensor_right_dup->setName("player right side");

	return player;
}

Direction Player::getCurrentDirection() {
	return _current_direction;
}

void Player::initPhysics(shared_ptr<ObstacleWorld> world) {
	world->addObstacle(_box);
	world->addObstacle(_box_dup);
	world->addObstacle(_sensor_left);
	world->addObstacle(_sensor_right);
	world->addObstacle(_sensor_left_dup);
	world->addObstacle(_sensor_right_dup);
}

void Player::setLinearVelocity(Vec2 velocity) {
	_box->setLinearVelocity(velocity);
	_box_dup->setLinearVelocity(velocity);
	_sensor_left->setLinearVelocity(velocity);
	_sensor_right->setLinearVelocity(velocity);
	_sensor_left_dup->setLinearVelocity(velocity);
	_sensor_right_dup->setLinearVelocity(velocity);
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

void Player::updateSensors() {
	Size size = Vec2(0.9, 0.9);

	_sensor_left->setLinearVelocity(_box->getLinearVelocity());
	_sensor_right->setLinearVelocity(_box->getLinearVelocity());
	_sensor_left->setLinearVelocity(_box_dup->getLinearVelocity());
	_sensor_right->setLinearVelocity(_box_dup->getLinearVelocity());

	_sensor_left->setPosition(_box->getPosition() - Vec2(size.width/2, 0));
	_sensor_right->setPosition(_box->getPosition() + Vec2(size.width / 2, 0));
	_sensor_left_dup->setPosition(_box_dup->getPosition() - Vec2(size.width / 2, 0));
	_sensor_right_dup->setPosition(_box_dup->getPosition() + Vec2(size.width / 2, 0));
}

void Player::reset() {
    _box->setPosition(_start_pos);
    _box_dup->setPosition(_start_pos);
	updateSensors();
    _alive = true;
}
