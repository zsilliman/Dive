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
//#include "PlatformMap.h"

using namespace cugl;
using namespace std;

#ifndef Enemy_h
#define Enemy_h

class Enemy{
    
protected:
    shared_ptr<WheelObstacle> _body;
    Vec2 _destination;
    float accelleration = 60;
    float max_speed = 500;
    
public:
    //This needs to be put into a separate View class but whatever
    shared_ptr<PolygonNode> _node;
    
    static shared_ptr<Enemy> allocWithTexture(shared_ptr<Texture> texture);
    
    void setScale(float x, float y);
    
    void setPhysicsPosition(float x, float y);
    
    Vec2 getPhysicsPosition() { return _body->getPosition(); }
    
    Vec2 get_push_destination() { return _destination; }
    
    void push(Vec2 force);
    
    void initPhysics(shared_ptr<ObstacleWorld> world);
    
    void updatePosition();
    
    float getdX(float timestep) const { return _body->getLinearVelocity().x * timestep; }
    
};

#endif /* Enemy_h */
