//
//  GameScene.cpp
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
#define SCENE_WIDTH 12

#pragma mark -
#pragma mark Constructors

#define WIN_MESSAGE     "VICTORY!"
/** The color of the win message */
#define WIN_COLOR       Color4::YELLOW
#define EXIT_COUNT      100
#define MESSAGE_FONT    "charlemagne"

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
//    CULog("height is: %d", size.getIHeight());
//    CULog("width is: %d", size.getIWidth());
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
        //removeAllChildren();
		//_map_vc->dispose();
		//_player_vc->dispose();
        //_goal_vc->dispose();
        //_winnode->dispose();
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
	_world->update(timestep);
	_map_vc->update(_gamestate);
    _player_vc->update(_gamestate);
	_urchin_vc->update(_gamestate);

	if (!_complete) {
		//_goal_vc->update(_gamestate);
		frame_counter++;
		if (frame_counter >= UPDATE_STEP) {
//            CULog("STEP");
			//_player_vc->update(_gamestate);
			//for (int i = 0; i < _urchin_vcs.size(); i++) {
			//	_urchin_vcs[i]->update(_gamestate);
			//}
			frame_counter = 0;
		}

		//if (_gamestate->GoalCollision()) {
		//	setComplete(true);
		//}
		//if (_player_vc->hitEnemy(_gamestate)) {
		//	setComplete(true);
		//}
	}
    //w below commented out, dups move oc don't
//    if (_countdown > 0) {
//        _countdown--;
//    } else if (_countdown == 0) {
//        reset();
//    }
}

void GameScene::setComplete(bool value) {
    bool change = _complete != value;
    _complete = value;
    if (value && change) {
//        CULog("WINNN");
        _winnode->setVisible(true);
        _countdown = EXIT_COUNT;
    } else if (!value) {
        _winnode->setVisible(false);
        _countdown = -1;
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

	Rect physics_bounds = Rect(-100, -100, 200, 200);
	_world = ObstacleWorld::alloc(physics_bounds, Vec2(0, -9.8));
    
	shared_ptr<Texture> texture = _assets->get<Texture>("blank");
    shared_ptr<Texture> goal_texture = _assets->get<Texture>("goal");
	shared_ptr<TiledTexture> tilesheet = TiledTexture::alloc(texture, 16, 16);
	shared_ptr<Texture> diver_texture = _assets->get<Texture>("diver");
	shared_ptr<Texture> urchin_texture = _assets->get<Texture>("urchin");
	shared_ptr<Texture> image = _assets->get<Texture>("background");

	_gamestate->_map = PlatformMap::parseFromJSON("levels/sample_level.json", _assets);
	_gamestate->_map->initPhysics(_world);

	_gamestate->_player = Player::alloc(Vec2(4, 31), Vec2(1,1), _gamestate->_map->getMapRect());
	_gamestate->_player->initPhysics(_world);

	shared_ptr<Urchin> urchin1 = Urchin::alloc(Vec2(4, 28), _gamestate->_map->getMapRect());
	urchin1->initPhysics(_world);
	shared_ptr<Urchin> urchin2 = Urchin::alloc(Vec2(8, 19), _gamestate->_map->getMapRect());
	urchin2->initPhysics(_world);
	_gamestate->_urchins.push_back(urchin1);
	_gamestate->_urchins.push_back(urchin2);

	_background = PolygonNode::allocWithTexture(image);
	_background->setName("world");
	_background->setAnchor(Vec2::ANCHOR_TOP_RIGHT);
	addChild(_background, 0);

	_map_vc = PlatformMapViewController::alloc(_gamestate, tilesheet, size);
	addChild(_map_vc->getNode(), 1);
    
	_player_vc = PlayerViewController::alloc(_gamestate, diver_texture, size);
    _map_vc->getNode()->addChild(_player_vc->getNode(),1);

	_urchin_vc = UrchinViewController::alloc(_gamestate, urchin_texture, size, 0);
	_map_vc->getNode()->addChild(_urchin_vc->getNode(), 1);

//    _player_vc->setPhysicsPosition((size.width / 2)-.2, (size.height / 2)+.5);


	/*_map_vc = GridMapViewController::alloc(_gamestate, tilesheet, size);
	addChild(_map_vc->getNode(),0);

	_player_vc = PlayerViewController::alloc(_gamestate, diver_texture, size);
	_map_vc->getNode()->addChild(_player_vc->getNode());
    
	//Making a bunch of urchins
	std::shared_ptr<Texture> urchin_texture = _assets->get<Texture>("urchin");
	_urchin_vcs = {};
	shared_ptr<UrchinViewController> urchin_vc1 = UrchinViewController::alloc(_gamestate, urchin_texture, size, 0);
	_map_vc->getNode()->addChild(urchin_vc1->getNode());
	_urchin_vcs.push_back(urchin_vc1);
	shared_ptr<UrchinViewController> urchin_vc2 = UrchinViewController::alloc(_gamestate, urchin_texture, size, 1);
	_map_vc->getNode()->addChild(urchin_vc2->getNode());
	_urchin_vcs.push_back(urchin_vc2);
	shared_ptr<UrchinViewController> urchin_vc3 = UrchinViewController::alloc(_gamestate, urchin_texture, size, 2);
	_map_vc->getNode()->addChild(urchin_vc3->getNode());
	_urchin_vcs.push_back(urchin_vc3);
	shared_ptr<UrchinViewController> urchin_vc4 = UrchinViewController::alloc(_gamestate, urchin_texture, size, 3);
	_map_vc->getNode()->addChild(urchin_vc4->getNode());
	_urchin_vcs.push_back(urchin_vc4);

    _goal_vc = GoalViewController::alloc(_gamestate, goal_texture, size);
	_map_vc->getNode()->addChild(_goal_vc->getNode());
    
    _winnode = Label::alloc(WIN_MESSAGE, _assets->get<Font>(MESSAGE_FONT));
    _winnode->setAnchor(Vec2::ANCHOR_CENTER);
    _winnode->setPosition(size.width/2.0f,size.height/2.0f);
    _winnode->setForeground(WIN_COLOR);
    setComplete(false);
    addChild(_winnode,3);
    
    _goal_vc = GoalViewController::alloc(_gamestate, goal_texture, size);
    addChild(_goal_vc->getNode(),0);*/
    
    shared_ptr<Texture> up   = _assets->get<Texture>("close-normal");
    shared_ptr<Texture> down = _assets->get<Texture>("close-selected");
    
    Size bsize = up->getSize();
    shared_ptr<Button> button = Button::alloc(PolygonNode::allocWithTexture(up),
                                              PolygonNode::allocWithTexture(down));
    
    // Create a callback function for the button
    button->setName("close");
    button->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            CULog("Goodbye!");
            Application::get()->quit();
        }
    });
    
    button->setAnchor(Vec2::ANCHOR_CENTER);
    button->setPosition(4.75,.2);
    button->setScale(0.5f / up->getWidth());
    
    addChild(button,3);
    
    // We can only activate a button AFTER it is added to a scene
    button->activate(1);

 
/*  _winnode = Label::alloc("VICTORY!", _assets->get<Font>("charlemagne"));
    _winnode->setForeground(Color4::YELLOW);
    _winnode->setAnchor(Vec2::ANCHOR_CENTER);
    _winnode->setPosition(0,0);
    setComplete(false);
    addChild(_winnode, 3);*/
    
    Application::get()->setClearColor(Color4f::CORNFLOWER);
}

void GameScene::reset() {
	_gamestate->reset();
	//_map_vc->updateRows(_gamestate);
	//_map_vc->reset();
    //_player_vc->reset();
    //_goal_vc->reset();
    //removeAllChildren();
    //setComplete(false);
    //buildScene();
}
