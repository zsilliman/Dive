#pragma once

#include "../GameState.h"
#include "ViewController.h"
#include "../Entities/Entity.h"

using namespace cugl;
using namespace std;

class PlayerViewController : public ViewController {

protected:
	Size _display;
	shared_ptr<AnimationNode> _oc_node;
	shared_ptr<AnimationNode> _dup_node;

    float accelleration = 1.5f;
    float max_speed = 0.8f;
	float _grid_size = 1;
    
    bool _mainCycle;

public:
    void animatePlayer();
    
	void draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) override;

	void update(shared_ptr<GameState> state) override;

	void dispose() override;

	void reset() override;

	static shared_ptr<PlayerViewController> alloc(shared_ptr<GameState> init_state, shared_ptr<Texture> texture, Size display);

};
