#include "Entity.h"
#include "../Util.h"

using namespace cugl;
using namespace std;

Vec2 Entity::getPosition() {
	//CULog("pos from get x y : %d, %d", _position.x, _position.y);
	return _box->getPosition();
}

void Entity::setPosition(Vec2 position) {
	_box->setPosition(position);
	//put other off screen
	_box_dup->setPosition(Vec2(-_dimensions.width * 10, position.y));
}

void Entity::initPhysics(shared_ptr<ObstacleWorld> world) {
	world->addObstacle(_box);
	world->addObstacle(_box_dup);
}

void Entity::setLinearVelocity(Vec2 velocity) {
	_box->setLinearVelocity(velocity);
	_box_dup->setLinearVelocity(velocity);
}

bool Entity::canFloat() { return _can_float; }

void Entity::setCanFloat(bool floats) { _can_float = floats; }

void Entity::setName(string name) {
	_box->setName(name);
	_box_dup->setName(name);
}

string Entity::getName() {
	return _box->getName();
}

bool Entity::isAlive() { return _alive; }

void Entity::kill() {
	_box->setActive(false);
	_box_dup->setActive(false);
	_alive = false;
}

void Entity::revive() {
	_box->setActive(true);
	_box_dup->setActive(true);
	_alive = true;
}

void Entity::reset() {
	_box->setPosition(_start_pos);
	_box->setAngle(0);
    _box->setLinearVelocity(Vec2(0,0));
	_box_dup->setPosition(_start_pos);
	_box_dup->setAngle(0);
    _box_dup->setLinearVelocity(Vec2(0,0));
    
	revive();
}

void Entity::changeDirection() {
    CULog("in change direction");
    int x = _box->getLinearVelocity().x;
    int y = _box->getLinearVelocity().y;
    _box->setLinearVelocity(Vec2(-x, y));
    _box_dup->setLinearVelocity(Vec2(-x, y));
}

Rect Entity::getBoxRect(shared_ptr<Obstacle> box) {
	Size size = Size(_dimensions);
	Vec2 pos = box->getPosition() - (size / 2);
	return Rect(pos.x, pos.y, size.width, size.height);
}

bool Entity::overlapsLeftEdge(Rect entity_rect, Rect map_rect) {
	//If there is no intersection return false
	if (!entity_rect.doesIntersect(map_rect))
		return false;
	//If the lowerleft corner is less than 0, then we know it hit the left side
	if (entity_rect.getMinX() < map_rect.getMinX())
		return true;
	//Otherwise it does not overlap the left edge
	return false;
}

bool Entity::overlapsRightEdge(Rect entity_rect, Rect map_rect) {
	if (!entity_rect.doesIntersect(map_rect))
		return false;
	//If the lowerright corner is greater than map right edge, then we know it hit the right side
	if (entity_rect.getMaxX() > map_rect.getMaxX())
		return true;
	//Otherwise it does not overlap the left edge
	return false;
}

void Entity::rotateEntity(Rect map_rect) {
	float width = map_rect.size.width;
	Rect oc_rect = getBoxRect(_box);     //Original
	Rect cp_rect = getBoxRect(_box_dup); //duplicate
	_box->setActive(true);
	_box_dup->setActive(true);

	//oc_rect intersects left ==> cp_rect needs to be on the right portion of the map
	if (overlapsLeftEdge(oc_rect, map_rect) && _active_box == 0) {
		_box_dup->setPosition(_box->getPosition() + Vec2(width, 0));
		_box_dup->setAngle(_box->getAngle());
		_box_dup->setLinearVelocity(_box->getLinearVelocity());
	}
	//cp_rect intersects left ==> oc_rect needs to be on the right portion of the map
	else if (overlapsLeftEdge(cp_rect, map_rect) && _active_box == 1) {
		_box->setPosition(_box_dup->getPosition() + Vec2(width, 0));
		_box->setAngle(_box_dup->getAngle());
		_box->setLinearVelocity(_box_dup->getLinearVelocity());
	}
	//original intersects right ==> copy needs to be on left portion of the map
	else if (overlapsRightEdge(oc_rect, map_rect) && _active_box == 0) {
		_box_dup->setPosition(_box->getPosition() - Vec2(width, 0));
		_box_dup->setAngle(_box->getAngle());
		_box_dup->setLinearVelocity(_box->getLinearVelocity());
	}
	//copy intersects right ==> original needs to be on left portion of the map
	else if (overlapsRightEdge(cp_rect, map_rect) && _active_box == 1) {
		_box->setPosition(_box_dup->getPosition() - Vec2(width, 0));
		_box->setAngle(_box_dup->getAngle());
		_box->setLinearVelocity(_box_dup->getLinearVelocity());
	}
	//original is fully on screen ==> copy is off screen and thus inactive
	else if (map_rect.contains(oc_rect)) {
		_box_dup->setPosition(_box->getPosition() + Vec2(width, 0));
		_box_dup->setAngle(_box->getAngle());
		_box_dup->setLinearVelocity(_box->getLinearVelocity());
		//_box_dup->setActive(false);
		_active_box = 0;
	}
	//copy is fully on screen ==> original is off screen and thus inactive
	else if (map_rect.contains(cp_rect)) {
		_box->setPosition(_box_dup->getPosition() + Vec2(width, 0));
		_box->setAngle(_box_dup->getAngle());
		_box->setLinearVelocity(_box_dup->getLinearVelocity());
		//_box->setActive(false);
		_active_box = 1;
	}
}

void Entity::initEntity(Vec2 pos, Vec2 size, Rect map_rect) {
	_position = Vec2(pos + size / 2);
	_start_pos = Vec2(pos + size / 2);
	_dimensions = Size(size.x, size.y);
	_box = CapsuleObstacle::alloc(_position, size);
	_box_dup = CapsuleObstacle::alloc(_position + Vec2(map_rect.size.width, 0), size);
	_can_float = false;
}
