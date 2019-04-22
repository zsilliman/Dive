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
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>

#include "GameScene.h"

using namespace cugl;
using namespace std;

#pragma mark -
#pragma mark Level Layout

/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 5

#pragma mark -
#pragma mark Constructors

#define WIN_MESSAGE     "VICTORY!"
#define LOSE_MESSAGE    "You lost"
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
    _input = make_shared<InputController>();
    _input->init();

	buildScene("easy_level");

    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void GameScene::dispose() {
    if (_active) {
        _active = false;
        _input->dispose();
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
    _input->update(timestep);
	Size  size = Application::get()->getDisplaySize();
	scale = SCENE_WIDTH / size.width;
	size *= scale;

	_player_side_count = 0;
	_world->update(timestep);
	_map_vc->update(_gamestate);

	//Begin background parallax section
	float parallax_speed = 0.5f;
	float grid_size = size.width / _gamestate->_map->getWidth();
	float pos = _map_vc->getPosition().y + _gamestate->_map->getHeight()*grid_size + (size.height/(2*parallax_speed));
	_background->setPositionY(pos * parallax_speed);
	//End background parallax section
    _animation_counter--;
    //on a platform
    if (_playerFloor) {
        _player_vc->setFloor(true);
    }else{
        _player_vc->setFloor(false);
    }
    _player_vc->update(_gamestate);

	for (int i = 0; i < _urchin_vcs.size(); i++) {
		_urchin_vcs[i]->update(_gamestate);
	}
	for (int i = 0; i < _fish_vcs.size(); i++) {
		_fish_vcs[i]->update(_gamestate);
	}
	for (int i = 0; i < _angler_vcs.size(); i++) {
		_angler_vcs[i]->update(_gamestate);
	}
   
    //else if (bd2->getName().find("fish") != string::npos) {
    if(_fish_remove != -1){
        CULog("setting dead fish %d", _fish_remove);
        _fish_vcs[_fish_remove]->kill(_gamestate->_fish[_fish_remove]);
        _fish_remove = -1;
        CULog("fish dead num  %d", _fish_remove);
    }
	//remove angler fish
	if (_angler_remove != -1) {
        CULog("setting dead angler %d", _angler_remove);
		_angler_vcs[_angler_remove]->kill(_gamestate->_anglers[_angler_remove]);
		_angler_remove = -1;
	}
	if (!_complete) {
		frame_counter++;
		if (frame_counter >= UPDATE_STEP) {
			frame_counter = 0;
		}
	}
    if (_countdown > 0) {
        _countdown--;
    } else if (_countdown == 0) {
        reset();
    }
    _fish_countdown--;
}

void GameScene::setState(State state) {
	if (state == WIN && current_state == LOSE || state == LOSE && current_state == WIN)
		return;
	bool changed = state != current_state;
	if (state == WIN && current_state != LOSE && changed) {
		_winnode->setVisible(true);
		_countdown = EXIT_COUNT;
	} else if (state == LOSE && current_state != WIN && changed) {
		_losenode->setVisible(true);
		_countdown = EXIT_COUNT;
		shared_ptr<Texture> dying_texture = _assets->get<Texture>("dying");
		_player_vc->lose(dying_texture);
	} else if (state == PLAY) {
		_losenode->setVisible(false);
		_winnode->setVisible(false);
		_countdown = -1;
	}
	current_state = state;
}

string GameScene::cycleLevel(){
    string level;
    if(_current_level == "easy_level"){
        CULog("setting medium");
        level = "medium_level";
    }
    else if(_current_level == "medium_level"){
        CULog("setting hard");
        level = "hard_level";
    }
    else if(_current_level == "hard_level"){
        CULog("setting v hard");
        level = "very_hard_level";
    }
    else if(_current_level == "very_hard_level"){
        CULog("setting easy");
        level = "easy_level";
    }
    else{
        CULog("bad level");
        level = "easy_level";
    }
    return level;
}

void GameScene::buildScene(string level) {
	Size  size = Application::get()->getDisplaySize();
	scale = SCENE_WIDTH / size.width;
	size *= scale;
	// Find the safe area, adapting to the iPhone X
	Rect safe = Application::get()->getSafeArea();
	safe.origin *= scale;
	safe.size *= scale;

	Rect physics_bounds = Rect(-100, -100, 200, 200);
    
    _block_counter = 0;
    _animation_counter = 10;
    _diver_angle = 180.5f;
    _prev_diver_angle = 179;
    _fish_remove = -1;
	_angler_remove = -1;
    _fish_countdown = 10;
    _current_level = level;
    
    CULog("set block counter");
	_world = ObstacleWorld::alloc(physics_bounds, Vec2(0, -9.8));
    
	shared_ptr<Texture> texture = _assets->get<Texture>("tileset");
    shared_ptr<Texture> goal_texture = _assets->get<Texture>("statue");
	shared_ptr<TiledTexture> tilesheet = TiledTexture::alloc(texture, 382, 382);
	tilesheet->setTileIndexOffset(9);
	shared_ptr<Texture> diver_texture = _assets->get<Texture>("diver");
	shared_ptr<Texture> urchin_texture = _assets->get<Texture>("urchin");
	shared_ptr<Texture> fish_texture = _assets->get<Texture>("fish");
	shared_ptr<Texture> angler_texture = _assets->get<Texture>("angler");
	shared_ptr<Texture> background_image = _assets->get<Texture>("background");
    shared_ptr<Texture> diving_texture = _assets->get<Texture>("walking");
    shared_ptr<Texture> dying_diver_texture = _assets->get<Texture>("dying_diver");

	_gamestate = _assets->get<GameState>(level);
	_gamestate->initPhysics(_world);
    _world->activateCollisionCallbacks(true);
    _world->onBeginContact = [this](b2Contact* contact) {
        beginContact(contact);
    };
    _world->onEndContact = [this](b2Contact* contact) {
        endContact(contact);
    };
//    _world->beforeSolve = [this](b2Contact* contact, const b2Manifold* oldManifold) {
//        beforeSolve(contact,oldManifold);
//    };

	_background = PolygonNode::allocWithTexture(background_image);
	_background->setName("world");
	_background->setAnchor(Vec2::ANCHOR_TOP_LEFT);
    float b_scale = SCENE_WIDTH/_background->getWidth();
    _background->setScale(b_scale);
	addChild(_background, 0);

	_map_vc = PlatformMapViewController::alloc(_gamestate, _input, tilesheet, goal_texture, size);
	addChild(_map_vc->getNode(), 1);
    
	_player_vc = PlayerViewController::alloc(_gamestate, diving_texture, size);
    _map_vc->getNode()->addChild(_player_vc->getNode(),1);
    _playerFloor = false;

	//Create Urchin viewcontrollers
	_urchin_vcs = {};
    for (int i = 0; i < _gamestate->_urchins.size();i++) {
        shared_ptr<UrchinViewController> urchin_vc = UrchinViewController::alloc(_gamestate, urchin_texture, size, i);
        _map_vc->getNode()->addChild(urchin_vc->getNode(), 1);
        _urchin_vcs.push_back(urchin_vc);
    }

	//Create Fish viewcontrollers
	_fish_vcs = {};
	for (int i = 0; i < _gamestate->_fish.size(); i++) {
		shared_ptr<FishViewController> _fish_vc = FishViewController::alloc(_gamestate, fish_texture, size, i);
		_map_vc->getNode()->addChild(_fish_vc->getNode(), 1);
		_fish_vcs.push_back(_fish_vc);
	}

	//Create Fish viewcontrollers
	_angler_vcs = {};
	for (int i = 0; i < _gamestate->_anglers.size(); i++) {
		shared_ptr<AnglerViewController> _angler_vc = AnglerViewController::alloc(_gamestate, angler_texture, size, i);
		_map_vc->getNode()->addChild(_angler_vc->getNode(), 1);
		_angler_vcs.push_back(_angler_vc);
	}
    
    
    shared_ptr<Texture> up   = _assets->get<Texture>("close-normal");
    shared_ptr<Texture> down = _assets->get<Texture>("close-selected");
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
    
    //addChild(button,3);
    
    // We can only activate a button AFTER it is added to a scene
    //button->activate(1);

    _winnode = Label::alloc(WIN_MESSAGE, _assets->get<Font>(MESSAGE_FONT));
    _winnode->setAnchor(Vec2::ANCHOR_CENTER);
    _winnode->setPosition(2.5,4);
    _winnode->setForeground(WIN_COLOR);
    _winnode->setScale(4 / _winnode->getWidth());
    addChild(_winnode,3);

    
    _losenode = Label::alloc(LOSE_MESSAGE, _assets->get<Font>(MESSAGE_FONT));
    _losenode->setAnchor(Vec2::ANCHOR_CENTER);
    _losenode->setPosition(2.5,4);
    _losenode->setForeground(WIN_COLOR);
    _losenode->setScale(4 / _losenode->getWidth());
    addChild(_losenode,3);
    
	setState(PLAY);

    Application::get()->setClearColor(Color4f::CORNFLOWER);
}

void GameScene::reset() {
    if(current_state == WIN){
        CULog("cycling level");
        _current_level = cycleLevel();
    }
    else{
        CULog("not in win state %d", current_state);
    }
    _gamestate->reset();
    _player_vc->setAIDirection(_gamestate, "down");
    //setState(PLAY);
    CULog("Reset");
//    for (int i = 0; i < _fish_vcs.size(); i++) {
//        _fish_vcs[i]->setInitialVelocity(_gamestate, Vec2(-1,0));
//    }
    buildScene(_current_level);
}

/**
 * Processes the start of a collision
 *
 * This method is called when we first get a collision between two objects.  We use
 * this method to test if it is the "right" kind of collision.  In particular, we
 * use it to test if we make it to the win door.
 *
 * @param  contact  The two bodies that collided
 */
void GameScene::beginContact(b2Contact* contact) {
    CULog("BEGINNING CONTACT");
    b2Fixture* fix1 = contact->GetFixtureA();
    b2Fixture* fix2 = contact->GetFixtureB();
    
    b2Body* body1 = fix1->GetBody();
    b2Body* body2 = fix2->GetBody();
    
//    void* fd1 = fix1->GetUserData();
//    void* fd2 = fix2->GetUserData();
    
    Obstacle* bd1 = (Obstacle*)body1->GetUserData();
    Obstacle* bd2 = (Obstacle*)body2->GetUserData();
    
	if (bd1->getName() == "player left side" && bd2->getName() == "platform") {
		playerSidePlatformCollisions(bd1, bd2, true);
	}
	if (bd1->getName() == "player right side" && bd2->getName() == "platform") {
		playerSidePlatformCollisions(bd1, bd2, false);
	}
    if(bd1->getName() == "player"){
        if(bd2->getName() == "urchin"){
			setState(LOSE);
        }
        else if (bd2->getName().find("fish") != string::npos) {
            setState(LOSE);
        }
		else if (bd2->getName().find("angler") != string::npos) {
			setState(LOSE);
		}
        else if(bd2->getName() == "platform"){
            _block_counter++;
            CULog("inc block counter 2  %d", _block_counter);
            _player_vc->setAIDirection(_gamestate, _player_vc->getAIDirection());
            _playerFloor = true;
        }
        else if(bd2->getName() == "goal"){
			setState(WIN);
        }
    }
    else if(bd1->getName() == "urchin") {
        if(bd2->getName() == "player") {
			setState(LOSE);
        }
        else if (bd2->getName().find("fish") != string::npos) {
            fishUrchinCollisions(bd2, bd1);
        }
		else if (bd2->getName().find("angler") != string::npos) {
            anglerUrchinCollisions(bd2, bd1);
        }
    }
    else if (bd1->getName().find("fish") != string::npos) {
        if(bd2->getName() == "player"){
			setState(LOSE);
        }
        else if(bd2->getName() == "urchin") {
            fishUrchinCollisions(bd1, bd2);
        }
		else if (bd2->getName().find("angler") != string::npos) {
            fishAnglerCollisions(bd1, bd2);
		}
        else if (bd2->getName() == "platform"){
            fishPlatformCollisions(bd1, bd2);
        }
    }
	else if (bd1->getName().find("angler") != string::npos) {
		if (bd2->getName() == "player") {
			setState(LOSE);
		}
		else if (bd2->getName() == "urchin") {
			char num = bd1->getName().back();
			_angler_remove = num - '0';
		}
		else if (bd2->getName().find("fish") != string::npos) {
            fishAnglerCollisions(bd2, bd1);
		}
	}
    else if(bd1->getName() == "platform"){
        if(bd2->getName() == "player"){
            //diverPlatformCollisions(bd2, bd1);
            
            _block_counter++;
            CULog("inc block counter 1  %d", _block_counter);
            _player_vc->setAIDirection(_gamestate, _player_vc->getAIDirection());
            _playerFloor = true;
        }
        else if (bd2->getName().find("fish") != string::npos) {
            fishPlatformCollisions(bd2, bd1);
		}
		else if (bd2->getName() == "player left side") {
			playerSidePlatformCollisions(bd2, bd1, true);
		}
		else if (bd2->getName() == "player right side") {
			playerSidePlatformCollisions(bd2, bd1, false);
		}
    }
    else if(bd1->getName() == "goal"){
        if(bd2->getName() == "player"){
			setState(WIN);
        }
    }
}

void GameScene::playerSidePlatformCollisions(Obstacle* player_side, Obstacle* platform, bool left) {
	if (_player_side_count != 0 || !_player_vc->getFloor()) return;
		_player_side_count++;
	if (left) {
		_player_vc->setAIDirection(_gamestate, "right");
		CULog("GO RIGHT");
	} else {
		_player_vc->setAIDirection(_gamestate, "left");
		CULog("GO LEFT");
	}
}

void GameScene::fishPlatformCollisions(Obstacle* fish, Obstacle* platform){
    char num = fish->getName().back();
    int fishint = num-'0';
    if(_fish_countdown<=0){
        _fish_countdown=10;
        _fish_vcs[fishint]->setDirection(_gamestate->_fish[fishint]);
    }
}

void GameScene::fishAnglerCollisions(Obstacle* fish, Obstacle* angler){
    char num = fish->getName().back();
    _fish_remove = num-'0';
    char num2 = angler->getName().back();
    _angler_remove = num2-'0';
}

void GameScene::fishUrchinCollisions(Obstacle* fish, Obstacle* urchin){
    char num = fish->getName().back();
    _fish_remove = num-'0';
}

void GameScene::anglerUrchinCollisions(Obstacle* angler, Obstacle* urchin){
    char num = angler->getName().back();
    _angler_remove = num-'0';
}

void diverPlatformCollisions(Obstacle* diver, Obstacle* platform){
    
}


//void GameScene::beforeSolve(b2Contact* contact, const b2Manifold* oldManifold){
//
//}

/**
 * Processes the end of a collision
 *
 *
 *
 * @param  contact  The two bodies that collided
 */
void GameScene::endContact(b2Contact* contact) {
    b2Fixture* fix1 = contact->GetFixtureA();
    b2Fixture* fix2 = contact->GetFixtureB();
    
    b2Body* body1 = fix1->GetBody();
    b2Body* body2 = fix2->GetBody();
    
//    void* fd1 = fix1->GetUserData();
//    void* fd2 = fix2->GetUserData();
    
    Obstacle* bd1 = (Obstacle*)body1->GetUserData();
    Obstacle* bd2 = (Obstacle*)body2->GetUserData();
    
    if(bd1->getName() == "platform"){
        if(bd2->getName() == "player"){
            _block_counter--;
            CULog("dec block counter 1  %d", _block_counter);
            if(_block_counter<0){
                _block_counter = 0;
                CULog("setting block counter to 0.1");
            }
            //change ai direction
            if(_block_counter==0){
                CULog("block counter 0.1");
                _player_vc->setAIDirection(_gamestate, "down");
                _playerFloor = false;
            }
        }
    }
    else if(bd1->getName() == "player"){
        if(bd2->getName() == "platform"){
            _block_counter--;
            CULog("dec block counter 2  %d", _block_counter);
            if(_block_counter<0){
                _block_counter = 0;
                CULog("setting block counter to 0.1");
            }
            //change ai direction
            if(_block_counter==0){
                CULog("block counter 0.2");

                _player_vc->setAIDirection(_gamestate, "down");
                _playerFloor = false;
            }
        }
    }
}
    
