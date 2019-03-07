#pragma once

#include "World/TiledTexture.h"
#include "GameState.h"
#include "ViewController.h"

using namespace cugl;
using namespace std;

class GridRowViewController : public ViewController {

protected:
	shared_ptr<TiledTexture> _tilesheet;
	vector<shared_ptr<PolygonNode>> _blocks = {};
	float _grid_size;
	int _row_index;

public:

	void draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) override;

	void update(shared_ptr<GameState> state) override;

	void dispose() override;

	void reset() override;

	void setTileSet(shared_ptr<TiledTexture> tilesheet);

	void init(shared_ptr<GameState> init_state, int row_index, float grid_size);

	static shared_ptr<GridRowViewController> alloc(shared_ptr<GameState> init_state, shared_ptr<TiledTexture> tilesheet, int row_index, float grid_size);

};