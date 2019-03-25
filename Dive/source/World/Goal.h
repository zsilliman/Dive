#pragma once

#include <cugl/cugl.h>
#include "Platform.h"

using namespace cugl;
using namespace std;


class Goal : public Platform  {

public:

	shared_ptr<Goal> duplicateGoal();

    static shared_ptr<Goal> allocGoal(Vec2 goal_pos);
        
};
