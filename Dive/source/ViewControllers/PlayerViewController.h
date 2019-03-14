#pragma once

#include "GameState.h"
#include "ViewController.h"
#include "../Entities/Entity.h"

using namespace cugl;
using namespace std;

class PlayerViewController : public ViewController {

protected:
	Size _display;
    shared_ptr<Obstacle> _body;
    
    float accelleration = 1.5f;
    float max_speed = 0.8f;

public:

	void draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) override;

	void update(shared_ptr<GameState> state) override;

	void dispose() override;

	void reset() override;

	bool canMove(shared_ptr<GameState> state, Direction direction);

	bool isBlocked(shared_ptr<GameState> state);

	void fall(shared_ptr<GameState> state);

	bool hitEnemy(shared_ptr<GameState> state);

	bool hitGoal(shared_ptr<GameState> state);

	static shared_ptr<PlayerViewController> alloc(shared_ptr<GameState> init_state, shared_ptr<Texture> texture, Size display);
    
    void initPhysics(shared_ptr<ObstacleWorld> world);
    
    void updateNodePosition();
    
    void setBodyName(string name) { _body->setName(name); }
    
    string getBodyName() { return _body->getName(); }
    
    void setScale(float x, float y);
    
    void setPhysicsPosition(float x, float y);
    
    Vec2 getPhysicsPosition() { return _body->getPosition(); }
    

};
