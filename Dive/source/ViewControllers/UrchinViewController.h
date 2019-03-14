#pragma once

#include "GameState.h"
#include "ViewController.h"
#include "../Entities/Entity.h"

using namespace cugl;
using namespace std;

class UrchinViewController : public ViewController {

protected:
	Size _display;
	int _urchin_index;
	shared_ptr<Obstacle> _body;

public:

	void draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) override;

	void update(shared_ptr<GameState> state) override;

	void dispose() override;

	void reset() override;

	bool canMove(shared_ptr<GameState> state, Direction direction);

	void fall(shared_ptr<GameState> state);

	static shared_ptr<UrchinViewController> alloc(shared_ptr<GameState> init_state, shared_ptr<Texture> texture, Size display, int urchin_index);

	void setPhysicsPosition(float x, float y);

	void updateNodePosition();

	void initPhysics(shared_ptr<ObstacleWorld> world);
};
