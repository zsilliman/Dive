#pragma once

#include "../GameState.h"
#include "ViewController.h"
#include "../Entities/Entity.h"

using namespace cugl;
using namespace std;

class AnglerViewController : public ViewController {

protected:
	Size _display;
	shared_ptr<AnimationNode> _oc_node;
	shared_ptr<AnimationNode> _dup_node;
    shared_ptr<AnimationNode> _exp_node;
    shared_ptr<AnimationNode> _exp_dup_node;
    shared_ptr<AnimationNode> _see_node;
    shared_ptr<AnimationNode> _see_dup_node;
    shared_ptr<AnimationNode> _skel_node;
    shared_ptr<AnimationNode> _skel_dup_node;
    
    shared_ptr<Texture> rev;
    shared_ptr<Texture> normal;
    
    shared_ptr<Angler> _dead_ang;

	float accelleration = 1.5f;
	float max_speed = 0.8f;
	float _grid_size = 1;
	int _angler_index = 0;
    
    int _cooldown = 3;
    int s_cooldown = 3;
	bool _mainCycle;
    bool _sCycle;
    
    bool _dead;
    bool _see;
    bool _skel;

public:

	void animateAngler();
    
    void animateDead();
    
    void animateSee();
    
    void animateSkel();

	void draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) override;

	void update(shared_ptr<GameState> state) override;

	void dispose() override;

	void reset() override;

	static shared_ptr<AnglerViewController> alloc(shared_ptr<GameState> init_state, shared_ptr<Texture> texture, shared_ptr<Texture> reverse, shared_ptr<Texture> explosion, shared_ptr<Texture> see, shared_ptr<Texture> skel, Size display, int angler_index);

	void kill(shared_ptr<Angler> angler);

	void revive(shared_ptr<Angler> angler);
};
