#pragma once

#include "ViewController.h"
#include "PlatformViewController.h"

using namespace cugl;
using namespace std;

class PlatformMapViewController : public ViewController {

	vector<shared_ptr<PlatformViewController>> _platforms;
	Size _display;
	float _grid_size = 1;

public:

	void draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) override;

	void update(shared_ptr<GameState> state) override;

	void dispose() override;

	void reset() override;

	static shared_ptr<PlatformMapViewController> alloc(shared_ptr<GameState> init_state, shared_ptr<TiledTexture> texture, Size display);

};