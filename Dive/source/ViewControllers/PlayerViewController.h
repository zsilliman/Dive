#pragma once

#include "GameState.h"
#include "ViewController.h"
#include "../Entities/Entity.h"

using namespace cugl;
using namespace std;

class PlayerViewController : public ViewController {

protected:
	Size _display;

public:

	void draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) override;

	void update(shared_ptr<GameState> state) override;

	void dispose() override;

	void reset() override;

	bool canMove(shared_ptr<GameState> state, Direction direction);

	bool isBlocked(shared_ptr<GameState> state);

	void fall(shared_ptr<GameState> state);

	static shared_ptr<PlayerViewController> alloc(shared_ptr<GameState> init_state, shared_ptr<Texture> texture, Size display);

};
