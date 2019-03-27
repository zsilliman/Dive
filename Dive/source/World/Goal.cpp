#include "Goal.h"
#include "../Util.h"

using namespace cugl;
using namespace std;

shared_ptr<Goal> Goal::allocGoal(Vec2 goal_pos) {
    shared_ptr<Goal> goal = make_shared<Goal>();
	goal->setSensor(true);
	goal->adj_tiles.push_back(goal_pos);
	goal->adj_values.push_back(GOAL_TILE_ID);
	goal->_initial_pos.set(goal->getMinCorner());
	goal->setName("goal");
    return goal;
}

shared_ptr<Goal> Goal::duplicateGoal() {
	shared_ptr<Goal> goal = make_shared<Goal>();
	//copy values
	goal->adj_tiles = adj_tiles;
	goal->adj_values = adj_values;
	goal->_initial_pos.set(_initial_pos);
	goal->_relative_speed = _relative_speed;
	goal->setSensor(true);
	goal->setName(getName());
	return goal;
}