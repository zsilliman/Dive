#include "Entity.h"
#include "Util.h"

using namespace cugl;
using namespace std;

Vec2 Entity::getPosition() {
	return Vec2(_position);
}

bool Entity::canFloat() { return _can_float; }

void Entity::setCanFloat(bool floats) { _can_float = floats; }

void Entity::move(Direction direction, int map_width) {
	switch (direction)
	{
	case UP:
		_position.y -= 1;
		break;
	case DOWN:
		_position.y += 1;
		break;
	case LEFT:
		_position.x = Util::mod(_position.x - 1, map_width);
		break;
	case RIGHT:
		_position.x = Util::mod(_position.x + 1, map_width);
		break;
	}
}