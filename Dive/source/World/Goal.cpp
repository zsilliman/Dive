#include "Goal.h"
#include "Util.h"

using namespace cugl;
using namespace std;

shared_ptr<Goal> Goal::alloc(Vec2 start_pos) {
    shared_ptr<Goal> goal = make_shared<Goal>();
    goal->_position.set(start_pos);
    return goal;
}

