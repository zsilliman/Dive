#include "Player.h"

using namespace cugl;
using namespace std;

shared_ptr<Player> Player::allocWithTexture(shared_ptr<Texture> texture) {
	shared_ptr<Player> player = make_shared<Player>();
	player->_node = PolygonNode::allocWithTexture(texture);
	return player;
}

void Player::setScale(float x, float y) {
	_node->setScale(x, y);
}

//Note that init physics must be called first
void Player::setPhysicsPosition(float x, float y) {
	_body->setPosition(x, y);
}

void Player::push(Vec2 force) {
	_body->getBody()->ApplyForceToCenter(b2Vec2(force.x, force.y), true);

	/* Changed movement
	Vec2 linear_velocity = _body->getLinearVelocity();

	//Normalize the direction from node to mouse click
	Vec2 dir = force.getNormalization();
	//Scale it to the accelleration speed
	dir.scale(accelleration);
	//Accellerate the player, but cap it at max_speed
	if ((linear_velocity + dir).length() <= max_speed) {
		linear_velocity += dir;
	}
	else {
		linear_velocity += dir;
		linear_velocity.scale(max_speed / linear_velocity.length());
	}
	//Set the velocity of the physics body
	//_body->setLinearVelocity(linear_velocity);*/
}

//This is how the player follows the mouse
void Player::pushDirection(Vec2 mouse_pos, shared_ptr<PlatformMap> platform_map) {
	//Return immediately if no destination has been set
	if (!_dest_active)
		return;
	//Convert from screen to local coordinates
	Vec2 local_pos = platform_map->_node->screenToNodeCoords(mouse_pos) - _body->getPosition();
	//This is so the center of the node is attracted to the click location
	//local_pos -= Vec2(_node->getWidth() / (2.0f*_node->getScaleX()), _node->getHeight() / (2.0f*_node->getScaleY()));
	local_pos -= Vec2(_body->getWidth() / 2, _body->getHeight() / 2);
	//Prevent the push if the click location is too close to the node
	if (local_pos.length() < accelleration*(1.0f / 10.0f))
		return;
	Vec2 force = local_pos.getNormalization().scale(accelleration);

	push(force);
}

void Player::setPushDestination(Vec2 mouse_pos, shared_ptr<PlatformMap> platform_map) {
	_dest_active = true;
	_destination = platform_map->_node->screenToNodeCoords(mouse_pos);
}

void Player::pushToDestination() {
	Vec2 local_pos = _destination - _body->getPosition();
	if (local_pos.length() < accelleration*(1.0f / 10.0f))
		return;
	Vec2 force = local_pos.getNormalization().scale(accelleration);
	push(force);
}

void Player::initPhysics(shared_ptr<ObstacleWorld> world) {
	_body = BoxObstacle::alloc(Vec2(_node->getPositionX(), _node->getPositionY()), Size(_node->getWidth(), _node->getHeight()));
	_body->setBodyType(b2BodyType::b2_dynamicBody);
	_body->setLinearDamping(10);
	_body->setName("player");
	world->addObstacle(_body);
}

void Player::updatePosition() {
	_node->setPosition(_body->getPosition());
	_node->setAngle(_body->getAngle());
}