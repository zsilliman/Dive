#pragma once

#include "ViewController.h"
#include "../World/TiledTexture.h"
#include "GridRowViewController.h"

using namespace cugl;
using namespace std;

class GridMapViewController : public ViewController {

protected:
	vector<shared_ptr<GridRowViewController>> _rows = {};
	float _grid_size;
	Size _display;

public: 

	void update(shared_ptr<GameState> state) override;

	void updateRows(shared_ptr<GameState> state);

	void draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) override;

	void dispose() override;

	void reset() override;

	static shared_ptr<GridMapViewController> alloc(shared_ptr<GameState> init_state);

	static shared_ptr<GridMapViewController> alloc(shared_ptr<GameState> init_state, shared_ptr<TiledTexture> tilesheet, Size display);

	void init(shared_ptr<GameState> init_state, shared_ptr<TiledTexture> tilesheet, Size display);

};
