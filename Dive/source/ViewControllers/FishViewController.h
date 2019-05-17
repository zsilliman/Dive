#pragma once

#include "../GameState.h"
#include "ViewController.h"
#include "../Entities/Entity.h"

using namespace cugl;
using namespace std;

class FishViewController : public ViewController {

protected:
	Size _display;
	shared_ptr<AnimationNode> _oc_node;
	shared_ptr<AnimationNode> _dup_node;
    shared_ptr<AnimationNode> _exp_node;
    shared_ptr<AnimationNode> _exp_dup_node;
    shared_ptr<AnimationNode> _skel_node;
    shared_ptr<AnimationNode> _skel_dup_node;
    
    shared_ptr<Fish> _dead_fish;
    
    shared_ptr<Texture> rev;
    shared_ptr<Texture> normal;
    
    shared_ptr<Texture> rev_skel;
    shared_ptr<Texture> normal_skel;
    
	float accelleration = 1.5f;
	float max_speed = 0.8f;
	float _grid_size = 1;
	int _fish_index = 0;
    int _cooldown = 0;

	bool _mainCycle;
    
    bool _dead;
    
    bool _skel;

public:

	void animateFish();
    
    void animateDead();
    
    void animateSkel();
    
	void draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) override;

	void update(shared_ptr<GameState> state) override;

	void dispose() override;

	void reset() override;

	static shared_ptr<FishViewController> alloc(shared_ptr<GameState> init_state, shared_ptr<Texture> texture, shared_ptr<Texture> reverse, shared_ptr<Texture> explosion, shared_ptr<Texture> skel, shared_ptr<Texture> reverse_skel, Size display, int fish_index);

	void kill(shared_ptr<Fish> fish);

	void revive(shared_ptr<Fish> fish);

	void setDirection(shared_ptr<Fish> fish);

	void setInitialVelocity(shared_ptr<GameState> state, Vec2 vel);

};
