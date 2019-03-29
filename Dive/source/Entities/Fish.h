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

#define FISH_FRICTION 0.3
#define FISH_RESTITUTION 0.3
#define FISH_ANG_DAMP 0.3
#define FISH_LIN_DAMP 0.3
#define FISH_DENSITY 1

using namespace cugl;
using namespace std;

class Fish : public Entity {

public:

	void initFish(Vec2 start_pos, Rect map_rect);

	static shared_ptr<Fish> alloc(Vec2 start_pos, Rect map_rect, int index);

};
