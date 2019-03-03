#pragma once
//
//  Enemy.h
//  Dive
//
//  Created by Pippen Wu on 2019/2/28.
//  Copyright © 2019 Game Design Initiative at Cornell. All rights reserved.
//

#include <cugl/cugl.h>
#include <cugl/2d/physics/CUBoxObstacle.h>
#include <cugl/2d/physics/CUCapsuleObstacle.h>
#include "Entity.h"

using namespace cugl;
using namespace std;

class Enemy : public Entity {    
public:
    
    static shared_ptr<Enemy> allocWithTexture(shared_ptr<Texture> texture);
   
    void initPhysics(shared_ptr<ObstacleWorld> world);
    
};
