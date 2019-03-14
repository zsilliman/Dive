#pragma once

#include "ViewController.h"
#include "../World/Platform.h"
#include "../World/TiledTexture.h"

using namespace cugl;
using namespace std;

class PlatformViewController : public ViewController {

	vector<shared_ptr<Node>> _nodes = {};
	shared_ptr<Platform> _platform;
	float _grid_size = 1;

public:

	void draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) override;

	void update(shared_ptr<GameState> state) override;

	void dispose() override;

	void reset() override;

	static shared_ptr<PlatformViewController> alloc(shared_ptr<Platform> platform, shared_ptr<TiledTexture> tilesheet, float grid_size);

};