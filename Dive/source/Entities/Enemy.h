#pragma once
//
//  Enemy.h
//  Dive
//
//  Created by Pippen Wu on 2019/2/28.
//  Copyright © 2019 Game Design Initiative at Cornell. All rights reserved.
//

#include <cugl/cugl.h>
#include "Entity.h"

using namespace cugl;
using namespace std;

class Enemy : public Entity {

public:

	Direction _current_direction = Direction::RIGHT;

	static shared_ptr<Enemy> alloc(Vec2 start_pos);

};