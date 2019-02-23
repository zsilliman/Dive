#include "Platform.h"

using namespace cugl;
using namespace std;

shared_ptr<PolygonNode> _node;

shared_ptr<Platform> Platform::parseFromJSON(shared_ptr<JsonValue> json, shared_ptr<AssetManager> _assets) {
	return make_shared<Platform>();
}

shared_ptr<Platform> Platform::allocWithTexture(shared_ptr<Texture> texture) {
	shared_ptr<Platform> platform = make_shared<Platform>();
	platform->_node = PolygonNode::allocWithTexture(texture);
	return platform;
}

void Platform::setScale(float x, float y) {
	_node->setScale(x, y);
}

void Platform::setInitialPosition(float x, float y) {
	_node->setPositionX(x);
	_node->setPositionY(y);
	_initial_pos = Vec2(x, y);
}

void Platform::initPhysics(shared_ptr<ObstacleWorld> world) {
	_body = BoxObstacle::alloc(Vec2(_node->getPositionX(), _node->getPositionY()), Size(_node->getWidth(), _node->getHeight()));
	_body->setBodyType(b2BodyType::b2_kinematicBody);
	world->addObstacle(_body);
}

void Platform::updatePosition() {
	_node->setPosition(_body->getPosition());
	_node->setAngle(_body->getAngle());
}

void Platform::parallaxTranslate(float reference_x, float reference_y, float reference_dx) {
	//Compute distance between y coordinates
	float dist = reference_y - _node->getPositionY();
	//Essentially draw a line with slope -1/500 with y-intercept at 1
	//I think this should be changed, but we should discuss how platform speed changes with distance(along y-axis) from the player
	float relative_speed = 1 - (dist / 500);
	//Set bounds on the speed
	if (relative_speed < 0)
		relative_speed = 0;
	if (relative_speed > 2)
		relative_speed = 2;
	//Set it so that it moves in the opposite direction of the player
	relative_speed *= -1;
	
	//Compute position based on velocity in x direction
	Vec2 position = _body->getPosition() + Vec2(reference_dx*relative_speed, 0);
	
	//Compute position based on the absolute x position of the player
	//(TOGGLE THE BELOW CODE TO CHANGE BETWEEN THE TWO TYPES OF PARALLAX TRANSLATIONS)
	//position.x = _initial_pos.x + relative_speed * reference_x;
	
	//Checks whether the jump is possible
	if (_map_size > 1) {
		//This part is still kind of faulty, but whatever
		//This makes the positions jump back to simulate an infinite world
		//It has a weird bunch of modulos. a%b sometimes returns negative
		//So I had to do (b + (a%b)) % b
		position.x = (_map_size + ((int)position.x % _map_size)) % _map_size;
	}
	//Set the position to the newly computed one
	_body->setPosition(position);
}

void Platform::setMapSize(int map_size) { 
	_map_size = map_size;
	if (map_size > 1)
		_initial_pos.x = (_map_size + ((int)_initial_pos.x % _map_size)) % _map_size;
}