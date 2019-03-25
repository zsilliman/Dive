#include "Goal.h"

using namespace cugl;
using namespace std;

shared_ptr<Goal> Goal::alloc(Vec2 start_pos, Rect map_rect) {
    shared_ptr<Goal> goal = make_shared<Goal>();
    goal->initEntity(start_pos, Vec2(0.9, 0.9), map_rect);
    goal->_box->setFriction(0);
    goal->_box_dup->setFriction(0);
    return goal;
}
