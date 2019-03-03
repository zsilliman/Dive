#include "Entity.h"

using namespace cugl;
using namespace std;

/*shared_ptr<Entity> Entity::allocWithTexture(shared_ptr<Texture> texture) {
	shared_ptr<Entity> entity = make_shared<Entity>();
	entity->_node = PolygonNode::allocWithTexture(texture);
	return entity;
}*/

void Entity::setScale(float x, float y) {
	_node->setScale(x, y);
}

//Note that init physics must be called first
void Entity::setPhysicsPosition(float x, float y) {
	_body->setPosition(x, y);
	_node->setPosition(x, y);
}

void Entity::push(Vec2 force) {
	_body->getBody()->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
	Vec2 lin_vel = _body->getLinearVelocity();
	if (lin_vel.length() > max_speed) {
		lin_vel.normalize().scale(max_speed);
		_body->setLinearVelocity(lin_vel);
	}
}

//This is how the Entity follows the mouse
void Entity::pushDirection(Vec2 mouse_pos, shared_ptr<PlatformMap> platform_map) {
	//Convert from screen to local coordinates
	Vec2 local_pos = platform_map->_node->screenToNodeCoords(mouse_pos) - _body->getPosition();
	//This is so the center of the node is attracted to the click location
	//local_pos -= Vec2(_node->getWidth() / (2.0f*_node->getScaleX()), _node->getHeight() / (2.0f*_node->getScaleY()));
	local_pos -= Vec2(_node->getWidth() / (2.0f*_node->getScaleX()), _node->getHeight() / (2.0f*_node->getScaleY()));
	//Prevent the push if the click location is too close to the node
	if (local_pos.length() < accelleration*(1.0f / 10.0f))
		return;
	Vec2 force = local_pos.getNormalization().scale(accelleration);

	push(force);
}

void Entity::pushToDestination(Vec2 mouse_pos, shared_ptr<PlatformMap> platform_map) {
	Vec2 _destination = platform_map->_node->screenToNodeCoords(mouse_pos);
	Vec2 local_pos = _destination - _body->getPosition();
	if (local_pos.length() < accelleration*(1.0f / 10.0f))
		return;
	Vec2 force = local_pos.getNormalization().scale(accelleration);
	push(force);
}

void Entity::pushToDestination(Vec2 destination) {
	Vec2 local_pos = destination - _body->getPosition();
	if (local_pos.length() < accelleration*(1.0f / 10.0f))
		return;
	Vec2 force = local_pos.getNormalization().scale(accelleration);
	push(force);
}

void Entity::initPhysics(shared_ptr<ObstacleWorld> world) {
	_body = WheelObstacle::alloc(Vec2(_node->getPositionX(), _node->getPositionY()), _node->getWidth() / 2);
	_body->setBodyType(b2BodyType::b2_dynamicBody);
	_body->setLinearDamping(0.5f);
	_body->setGravityScale(3);
	world->addObstacle(_body);
}

void Entity::updateNodePosition() {
	_node->setPosition(_body->getPosition());
	_node->setAngle(_body->getAngle());
}
