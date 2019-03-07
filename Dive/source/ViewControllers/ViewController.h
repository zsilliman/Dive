#pragma once

#include "GameState.h"

using namespace std;
using namespace cugl;

class ViewController {

protected:

	shared_ptr<Node> _node;

public:

	virtual void update(shared_ptr<GameState> state) = 0;

	virtual void draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) = 0;

	virtual void dispose() = 0;

	virtual void reset() = 0;

	// Useful Getters and Setters

	void setPosition(float x, float y) { _node->setPosition(x, y); }

	Vec2 getPosition() { return _node->getPosition(); }

	void addViewToScene(shared_ptr<Scene> scene) { scene->addChild(_node); }

	void addViewToScene(Scene *scene) { scene->addChild(_node); }

	void addViewToNode(shared_ptr<Node> node) { node->addChild(_node); }

	void setVisible(bool visible) { _node->setVisible(visible); }
	
	bool isVisible() { return _node->isVisible(); }

	shared_ptr<Node> getNode() { return _node; }
};