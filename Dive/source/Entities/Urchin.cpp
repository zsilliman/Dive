#include "Urchin.h"

using namespace cugl;
using namespace std;


shared_ptr<Urchin> Urchin::allocWithTexture(shared_ptr<Texture> texture) {
	shared_ptr<Urchin> urchin = make_shared<Urchin>();
	urchin->_node = PolygonNode::allocWithTexture(texture);
	return urchin;
}

void Urchin::initPhysics(shared_ptr<ObstacleWorld> world) {
	_body = WheelObstacle::alloc(_node->getPosition(), _node->getTexture()->getWidth() * _node->getScaleX()/2);
	_body->setBodyType(b2BodyType::b2_dynamicBody);
	// Set the physics attributes
	_body->setDensity(URCHIN_DENSITY);
	_body->setFriction(URCHIN_FRICTION);
	_body->setAngularDamping(URCHIN_DAMPING);
	_body->setRestitution(BASIC_RESTITUTION);
	_body->setInertia(URCHIN_INERTIA);
	_body->setLinearDamping(0.1f);
	_body->setGravityScale(20);
	_body->setName("urchin");
	world->addObstacle(_body);
}