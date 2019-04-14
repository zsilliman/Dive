#include "Goal.h"
#include "../Util.h"

using namespace cugl;
using namespace std;

shared_ptr<Goal> Goal::allocGoal(Vec2 goal_pos) {
    shared_ptr<Goal> goal = make_shared<Goal>();
	goal->setSensor(true);
	goal->adj_tiles = { goal_pos };
	goal->col_tiles = { goal_pos };
	goal->adj_values = { GOAL_TILE_ID };
	goal->_initial_pos.set(goal_pos);
	goal->setPosition(goal_pos);
	goal->setName("goal");
    return goal;
}

shared_ptr<Goal> Goal::duplicateGoal() {
	shared_ptr<Goal> goal = make_shared<Goal>();
	//copy values
	goal->adj_tiles = adj_tiles;
	goal->adj_values = adj_values;
	goal->col_tiles = col_tiles;
	goal->_initial_pos.set(_initial_pos);
	goal->setPosition(getPosition());
	goal->_relative_speed = _relative_speed;
	goal->setSensor(true);
	goal->setName(getName());
	return goal;
}