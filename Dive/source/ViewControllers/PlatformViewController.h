#pragma once

#include "ViewController.h"
#include "../World/Platform.h"
#include "../World/TiledTexture.h"

using namespace cugl;
using namespace std;

class PlatformViewController : public ViewController {


	shared_ptr<Node> _oc_node, _cp_node;
	vector<shared_ptr<Node>> _nodes = {};
	int _platform_index = 0;
	float _grid_size = 1;

public:

	void draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) override;

	void update(shared_ptr<GameState> state) override;

	void dispose() override;

	void reset() override;

	static shared_ptr<PlatformViewController> alloc(shared_ptr<GameState> init_state, shared_ptr<TiledTexture> tilesheet, float grid_size, int platform_index);

};