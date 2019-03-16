#include "Entity.h"
#include "Util.h"

using namespace cugl;
using namespace std;

Vec2 Entity::getPosition() {
	//CULog("pos from get x y : %d, %d", _position.x, _position.y);
	return _box->getPosition();
}

void Entity::setPosition(Vec2 position) {
	_box->setPosition(position);
	//put other off screen
	_box_dup->setPosition(Vec2(-_box_dup->getDimension().width * 2, position.y));
}

bool Entity::canFloat() { return _can_float; }

void Entity::setCanFloat(bool floats) { _can_float = floats; }

void Entity::reset() {
	_position.set(_start_pos);
}

Rect Entity::getBoxRect(shared_ptr<BoxObstacle> box) {
	Size size = box->getDimension();
	Vec2 pos = box->getPosition() - (size / 2);
	return Rect(pos.x, pos.y, size.width, size.height);
}

bool Entity::overlapsLeftEdge(Rect entity_rect, Rect map_rect) {
	//If there is no intersection return false
	if (!entity_rect.doesIntersect(map_rect))
		return false;
	//If the lowerleft corner is less than 0, then we know it hit the left side
	if (entity_rect.getMinX() <= map_rect.getMinX())
		return true;
	//Otherwise it does not overlap the left edge
	return false;
}

bool Entity::overlapsRightEdge(Rect entity_rect, Rect map_rect) {
	if (!entity_rect.doesIntersect(map_rect))
		return false;
	//If the lowerright corner is greater than map right edge, then we know it hit the right side
	if (entity_rect.getMaxX() >= map_rect.getMaxX())
		return true;
	//Otherwise it does not overlap the left edge
	return false;
}

void Entity::rotateEntity(Rect map_rect) {
	float width = map_rect.size.width;
	Rect oc_rect = getBoxRect(_box);     //Original
	Rect cp_rect = getBoxRect(_box_dup); //duplicate
	CULog("Rect Positions");
	CULog(Vec2(map_rect.getMinX(), map_rect.getMinY()).toString().c_str());
	CULog(Vec2(oc_rect.getMinX(), oc_rect.getMinY()).toString().c_str());
	CULog(Vec2(cp_rect.getMinX(), cp_rect.getMinY()).toString().c_str());
	// oc_rect intersects left ==> cp_rect needs to be on right portion of the map
	if (overlapsLeftEdge(oc_rect, map_rect)) {
		_box_dup->setPosition(_box->getPosition() + Vec2(width, 0));
	}
	//cp_rect intersects left ==> oc_rect needs to be on the right portion of the map
	else if (overlapsLeftEdge(cp_rect, map_rect)) {
		_box->setPosition(_box_dup->getPosition() + Vec2(width, 0));
	}
	//original intersects right ==> copy needs to be on left portion of the map
	else if (overlapsRightEdge(oc_rect, map_rect)) {
		_box_dup->setPosition(_box->getPosition() - Vec2(width, 0));
	}
	//copy intersects right ==> original needs to be on left portion of the map
	else if (overlapsRightEdge(cp_rect, map_rect)) {
		_box->setPosition(_box_dup->getPosition() - Vec2(width, 0));
	}
}

void Entity::initEntity(Vec2 pos, Vec2 size, Rect map_rect) {
	_box = BoxObstacle::alloc(pos, size);
	_box_dup = BoxObstacle::alloc(pos + Vec2(map_rect.size.width, 0), size);
	_can_float = false;
	_position = Vec2(pos);
	_start_pos = Vec2(pos);
}