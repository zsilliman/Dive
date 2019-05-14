#pragma once

#include "ViewController.h"
#include "PlatformViewController.h"
#include "InputController.h"

using namespace cugl;
using namespace std;

class PlatformMapViewController : public ViewController {

	vector<shared_ptr<PlatformViewController>> _platforms;
    shared_ptr<InputController> _input;
	shared_ptr<PolygonNode> _goal, _goal_dup;
	Size _display;
	float _grid_size = 1;

	void parallaxTranslateVisible(shared_ptr<PlatformMap> map, Rect visible_rect, float reference_dx);
    
public:

	void draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) override;

	void update(shared_ptr<GameState> state) override;

	void dispose() override;

	void reset() override;

	static shared_ptr<PlatformMapViewController> alloc(shared_ptr<GameState> init_state, shared_ptr<InputController> _input, shared_ptr<TiledTexture> tileset, shared_ptr<TiledTexture> tileset_moveable, shared_ptr<Texture> goal_texture, Size display);

};
