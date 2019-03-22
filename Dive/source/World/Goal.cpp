#include "Goal.h"
#include "../Util.h"

using namespace cugl;
using namespace std;

shared_ptr<Goal> Goal::alloc(Vec2 start_pos) {
    shared_ptr<Goal> goal = make_shared<Goal>();
    goal->_position.set(start_pos);
	goal->_start_pos.set(start_pos);
    return goal;
}

Vec2 Goal::getPosition() {
    return Vec2(_position);
}

void Goal::reset() {
	_position.set(_start_pos);
}

void Goal::move(Dir direction, int map_width) {
    switch (direction)
    {
        case LEFT_G:
            _position.x = Util::mod(_position.x - 1, map_width);
            break;
        case RIGHT_G:
            _position.x = Util::mod(_position.x + 1, map_width);
            break;
    }
}

