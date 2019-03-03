#include "Player.h"

using namespace cugl;
using namespace std;

shared_ptr<Player> Player::allocWithTexture(shared_ptr<Texture> texture) {
	shared_ptr<Player> player = make_shared<Player>();
	player->_node = PolygonNode::allocWithTexture(texture);
	return player;
}

void Player::initPhysics(shared_ptr<ObstacleWorld> world) {
	_body = WheelObstacle::alloc(Vec2(_node->getPositionX(), _node->getPositionY()), _node->getWidth() / 2);
	_body->setBodyType(b2BodyType::b2_dynamicBody);
	_body->setLinearDamping(0.5f);
	_body->setGravityScale(3);
	_body->setName("player");
	world->addObstacle(_body);

	accelleration = 2;
	max_speed = 1;
}