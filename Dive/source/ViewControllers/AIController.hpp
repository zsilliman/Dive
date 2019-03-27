//
//  AIController.hpp
//  Dive
//
//  Created by Anna Rice on 3/27/19.
//  Copyright © 2019 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef AIController_hpp
#define AIController_hpp

#include <stdio.h>

#endif /* AIController_hpp */

#include "../GameState.h"
#include "ViewController.h"
#include "../Entities/Entity.h"

using namespace cugl;
using namespace std;

class AIController : public ViewController {
    
protected:
    Size _display;
    //shared_ptr<Node> _oc_node;
    //shared_ptr<Node> _dup_node;
    
    float accelleration = 1.5f;
    float max_speed = 0.8f;
    float _grid_size = 1;
    shared_ptr<Entity> entity = nullptr;
    
    
public:
    
    //void draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) override;
    
    void update(shared_ptr<GameState> state) override;
    
    void dispose() override;
    
    void reset() override;
    
    //static shared_ptr<PlayerViewController> alloc(shared_ptr<GameState> init_state, shared_ptr<Texture> texture, Size display);
    
    static shared_ptr<Entity> getEntity();
};

