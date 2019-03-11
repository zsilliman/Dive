#pragma once

#include <cugl/cugl.h>

using namespace cugl;
using namespace std;

enum Dir {
    LEFT_G, RIGHT_G
};

class Goal {
    
protected:
    Vec2 _position = Vec2();
	Vec2 _start_pos = Vec2();

public:
        
    static shared_ptr<Goal> alloc(Vec2 start_pos);
    
    Vec2 getPosition();
    
    void move(Dir direction, int map_width);

	void reset();
};
