#pragma once

#include <cugl/cugl.h>
#include "../Entities/Entity.h"

using namespace cugl;
using namespace std;

class Goal : public Entity {
    
protected:
    
public:
        
    static shared_ptr<Goal> alloc(Vec2 start_pos);
};
