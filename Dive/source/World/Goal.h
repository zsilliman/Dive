#pragma once

#include <cugl/cugl.h>
#include <cugl/2d/physics/CUCapsuleObstacle.h>

using namespace cugl;
using namespace std;

class Goal {
    
protected:
    Vec2 _initial_pos = Vec2(0, 0);
    int _map_size = 0, _x_start = 0, _x_end = 0;

    
public:
    //This needs to be put into a separate View class but whatever
    shared_ptr<PolygonNode> _node;
    
    shared_ptr<BoxObstacle> _body;
    
    static shared_ptr<Goal> allocWithTexture(shared_ptr<Texture> texture);
    
    void setScale(float x, float y);
    
    void setInitialPosition(float x, float y);

    void initPhysics(shared_ptr<ObstacleWorld> world);
    
    void updatePosition(float dt);
    
    void setMapSize(int x_start, int x_end);
    
    void parallaxTranslate(float reference_x, float reference_y, float reference_dx);
};
