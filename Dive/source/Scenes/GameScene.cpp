//
//  SDGameScene.h
//  Ship Demo
//
//  This is the most important class in this demo.  This class manages the
//  gameplay for this demo.  It is a relativeluy simple class as we are not
//  worried about collisions.
//
//  WARNING: There are a lot of shortcuts in this design that will do not adapt
//  well to data driven design.  This demo has a lot of simplifications to make
//  it a bit easier to see how everything fits together.  However, the model
//  classes and how they are initialized will need to be changed if you add
//  dynamic level loading.
//
//  Author: Walker White
//  Version: 1/10/17
//
#include <cugl/cugl.h>
#include <iostream>
#include <sstream>

#include "GameScene.h"

using namespace cugl;
using namespace std;

#pragma mark -
#pragma mark Level Layout

/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 5

#pragma mark -
#pragma mark Constructors


/**
 * Initializes the controller contents, and starts the game
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * @param assets    The (loaded) assets for this game mode
 *
 * @return true if the controller is initialized properly, false otherwise.
 */
bool GameScene::init(const shared_ptr<AssetManager>& assets) {
	Size size = Application::get()->getDisplaySize();
	size *= SCENE_WIDTH / size.width;

	_gamestate = GameState::allocEmpty();

	if (assets == nullptr) {
		return false;
	}
	else if (!Scene::init(size)) {
		return false;
	}

	_assets = assets;

	buildScene();

    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void GameScene::dispose() {
    if (_active) {
        removeAllChildren();
		_map_vc->dispose();
		_player_vc->dispose();
        _active = false;
    }
}


#pragma mark -
#pragma mark Gameplay Handling

/**
 * The method called to update the game mode.
 *
 * This method contains any gameplay code that is not an OpenGL call.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void GameScene::update(float timestep) {
	_map_vc->update(_gamestate);
	frame_counter++;
	if (frame_counter >= UPDATE_STEP) {
		CULog("STEP");
		_player_vc->update(_gamestate);
		frame_counter = 0;
	}
}

void GameScene::buildScene() {
	Size  size = Application::get()->getDisplaySize();
	scale = SCENE_WIDTH / size.width;
	size *= scale;

	// Find the safe area, adapting to the iPhone X
	Rect safe = Application::get()->getSafeArea();
	safe.origin *= scale;
	safe.size *= scale;

	_gamestate = GameState::allocEmpty();

	_gamestate->_map = GridMap::parseFromJSON("levels/sample_level.json", _assets);
	_gamestate->_player = Player::alloc(Vec2(0, 0));
	shared_ptr<Texture> texture = _assets->get<Texture>("blank");
	shared_ptr<TiledTexture> tilesheet = TiledTexture::alloc(texture, 16, 16);
	shared_ptr<Texture> diver_texture = _assets->get<Texture>("diver");
	//mine
	std::shared_ptr<Texture> image = _assets->get<Texture>("background");
	_worldnode = PolygonNode::allocWithTexture(image);
	_worldnode->setName("world");
	_worldnode->setAnchor(Vec2::ANCHOR_TOP_RIGHT);
	addChild(_worldnode, 0);
	//end mine
	_map_vc = GridMapViewController::alloc(_gamestate, tilesheet, size);
	addChild(_map_vc->getNode());

	_player_vc = PlayerViewController::alloc(_gamestate, diver_texture, size);
	addChild(_player_vc->getNode());

	//CULog("Player Pos:");
	//CULog(_player_vc->getPosition().toString().c_str());
}

void GameScene::reset() {
	//TODO
	_map_vc->reset();
	_player_vc->reset();
}
