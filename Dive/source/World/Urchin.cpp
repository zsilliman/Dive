#include "Urchin.h"

using namespace cugl;
using namespace std;

#pragma mark Physics Constants

// Physics constants for initialization
/** Density of non-urchin objects */
#define BASIC_DENSITY       0.0f
/** Density of the urchin objects */
#define URCHIN_DENSITY       1.0f
/** Friction of non-urchin objects */
#define BASIC_FRICTION      0.1f
/** Friction of the urchin objects */
#define URCHIN_FRICTION      0.2f
/** Angular damping of the urchin objects */
#define URCHIN_DAMPING       1.0f
/** Collision restitution for all objects */
#define BASIC_RESTITUTION   0.1f


shared_ptr<Urchin> Urchin::allocWithTexture(shared_ptr<Texture> texture) {
	shared_ptr<Urchin> urchin = make_shared<Urchin>();
	urchin->_node = PolygonNode::allocWithTexture(texture);
	return urchin;
}

void Urchin::setScale(float x, float y) {
	_node->setScale(x, y);
}

//Note that init physics must be called first
void Urchin::setPhysicsPosition(float x, float y) {
	_body->setPosition(x, y);
}

void Urchin::push(Vec2 force) {
	if (force.length() < accelleration*(1.0f / 10.0f))
		return;
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
	_body->setLinearVelocity(linear_velocity);
}

//This is how the player follows the mouse
//void Player::pushDirection(Vec2 mouse_pos, shared_ptr<PlatformMap> platform_map) {
//	//Convert from screen to local coordinates
//	Vec2 local_pos = platform_map->_node->screenToNodeCoords(mouse_pos) - _body->getPosition();
//	//This is so the center of the node is attracted to the click location
//	//local_pos -= Vec2(_node->getWidth() / (2.0f*_node->getScaleX()), _node->getHeight() / (2.0f*_node->getScaleY()));
//	local_pos -= Vec2(_body->getWidth() / 2, _body->getHeight() / 2);
//	//Prevent the push if the click location is too close to the node
//	push(local_pos);
//}

//void Player::setPushDestination(Vec2 mouse_pos, shared_ptr<PlatformMap> platform_map) {
//	_destination = platform_map->_node->screenToNodeCoords(mouse_pos);
//}
//
//void Player::pushToDestination() {
//	Vec2 local_pos = _destination - _body->getPosition();
//	push(local_pos);
//}

void Urchin::initPhysics(shared_ptr<ObstacleWorld> world) {
	_body = BoxObstacle::alloc(Vec2(_node->getPositionX(), _node->getPositionY()), Size(_node->getWidth(), _node->getHeight()));
	_body->setBodyType(b2BodyType::b2_dynamicBody);
	// Set the physics attributes
	_body->setDensity(URCHIN_DENSITY);
	_body->setFriction(URCHIN_FRICTION);
	_body->setAngularDamping(URCHIN_DAMPING);
	_body->setRestitution(BASIC_RESTITUTION);
	_body->setLinearDamping(10);
	//_body->setGravityScale(-4.9f);
	world->addObstacle(_body);
}

void Urchin::updatePosition() {
	_node->setPosition(_body->getPosition());
	_node->setAngle(_body->getAngle());
}