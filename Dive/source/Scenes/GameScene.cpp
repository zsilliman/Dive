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
#define SCENE_WIDTH 500

#pragma mark -
#pragma mark Constructors

#define WIN_MESSAGE     "VICTORY!"
#define LOSE_MESSAGE    "You lost"
/** The color of the win message */
#define WIN_COLOR       Color4::YELLOW
#define EXIT_COUNT      130
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
bool GameScene::init(const shared_ptr<AssetManager>& assets, int level) {
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
    
    string level_name = "level_"+std::to_string(level);
	buildOnce();
	buildScene(level_name);

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
	_overlay->update(timestep);
	Size  size = Application::get()->getDisplaySize();
	scale = SCENE_WIDTH / size.width;
	size *= scale;

	if (current_state == PLAY) {
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
		}
		else {
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
		if (_fish_remove != -1) {
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
        for (int i = 0; i < _fish_vcs.size(); i++){
            _fish_countdown[i]--;
        }
        waveCounter++;
        if(waveCounter >= 500){
            AudioChannels::get()->playEffect("ocean", ocean_sound);
            waveCounter = -200;
        }
        
        for (int i = 0; i < _fish_vcs.size(); i++){
            float dist = _gamestate->_player->_box->getPosition().distance(_gamestate->_fish[i]->_box->getPosition());
            if(dist < 4 && !AudioChannels::get()->isActiveEffect("shark")){
                AudioChannels::get()->stopAllEffects();
                AudioChannels::get()->playEffect("shark", shark_sound);
            }

        }
	}
	else if (current_state == LOSE) {
		//YOU LOSE ANIMATION (OVERLAY ALREADY IMPLEMENTED)
	}
	else if (current_state == WIN) {
		//YOU WIN ANIMATION (OVERLAY ALREADY IMPLEMENTED)
	}
	else { //PAUSED
		//probably nothing
        AudioChannels::get()->stopAllEffects();
        //AudioChannels::get()->stopMusic();
	}
}

void GameScene::setState(State state) {
	if ((state == WIN && current_state == LOSE) || (state == LOSE && current_state == WIN))
		return;
	bool changed = state != current_state;
	if (state == WIN && current_state != LOSE && changed) {
        AudioChannels::get()->stopAllEffects();
        AudioChannels::get()->stopMusic();
		AudioChannels::get()->playEffect("victory", victory_sound);
		_overlay->setState(WIN);
		current_state = WIN;
	}
	else if (state == LOSE && current_state != WIN && changed) {
        AudioChannels::get()->stopAllEffects();
        CULog("stopping music");
        AudioChannels::get()->stopMusic();
        AudioChannels::get()->playEffect("lose", lose_sound);
		_overlay->setState(LOSE);
		current_state = LOSE;
	} else if (state == PLAY) {
        if(AudioChannels::get()->isActiveEffect("victory")){
            AudioChannels::get()->stopEffect("victory");
        }
        if(AudioChannels::get()->isActiveEffect("lose")){
            AudioChannels::get()->stopEffect("lose");
        }

		_overlay->setState(PLAY);
		current_state = PLAY;
	}
	else if (state == PAUSE) {
		_overlay->setState(PAUSE);
		current_state = PAUSE;
	}
}

string GameScene::cycleLevel(){
    string level = _current_level.erase(0,6);
    int level_int = stoi(level)+1;
    if(level_int<=final_level){
        _next_level = "level_"+std::to_string(level_int);
    }
    else{
        _next_level = "level_1";
    }
    return _next_level;
}

void GameScene::startLevel(int level){
    buildScene("level_"+std::to_string(level));
}

void GameScene::buildYellow(){
    _urchin = _assets->get<Texture>("yellow_urchin");
    _shark = _assets->get<Texture>("yellow_shark");
    _shark_left = _assets->get<Texture>("yellow_shark_left");
    _angler = _assets->get<Texture>("yellow_angler");
    background_image = _assets->get<Texture>("background");
    texture = _assets->get<Texture>("tileset");
}

void GameScene::buildBlue(){
    _urchin = _assets->get<Texture>("blue_urchin");
    _shark = _assets->get<Texture>("blue_shark");
    _shark_left = _assets->get<Texture>("blue_shark_left");
    _angler = _assets->get<Texture>("blue_angler");
    background_image = _assets->get<Texture>("background");
    texture = _assets->get<Texture>("tileset");
}

void GameScene::buildRed(){
    _urchin = _assets->get<Texture>("red_urchin");
    _shark = _assets->get<Texture>("red_shark");
    _shark_left = _assets->get<Texture>("red_shark_left");
    _angler = _assets->get<Texture>("red_angler");
    background_image = _assets->get<Texture>("background");
    texture = _assets->get<Texture>("tileset");
}

void GameScene::buildPurple(){
    _urchin = _assets->get<Texture>("purple_urchin");
    _shark = _assets->get<Texture>("purple_shark");
    _shark_left = _assets->get<Texture>("purple_shark_left");
    _angler = _assets->get<Texture>("purple_angler");
    background_image = _assets->get<Texture>("background");
    texture = _assets->get<Texture>("tileset");
}

void GameScene::createLevelAssets(){
    string level = _current_level.substr(6);
    int level_int = stoi(level);
    switch (level_int) {
        case 1: case 2: case 3: case 4:
            buildBlue();
            break;
        case 5: case 6: case 7: case 8:
            buildBlue();
            break;
        case 9: case 10: case 11: case 12:
            buildBlue();
            break;
        default:
            break;
    }
    
    tilesheet = TiledTexture::alloc(texture, 382, 382);
    tilesheet->setTileIndexOffset(9);
    _background = nullptr;
    _background = PolygonNode::allocWithTexture(background_image);
    _background->setName("world");
    _background->setAnchor(Vec2::ANCHOR_TOP_LEFT);
    float b_scale = SCENE_WIDTH / _background->getWidth();
    _background->setScale(b_scale);
}


void GameScene::buildOnce() {
	Size  size = Application::get()->getDisplaySize();
	scale = SCENE_WIDTH / size.width;
	size *= scale;
	// Find the safe area, adapting to the iPhone X
	Rect safe = Application::get()->getSafeArea();
	safe.origin *= scale;
	safe.size *= scale;

    counter = 0;
    waveCounter = 0;
	goal_texture = _assets->get<Texture>("statue");

	shared_ptr<Texture> texture_moveable = _assets->get<Texture>("tileset_moveable");
	tilesheet_moveable = TiledTexture::alloc(texture_moveable, 382, 382);
	tilesheet_moveable->setTileIndexOffset(9);
    
	diving_texture = _assets->get<Texture>("walking");
    diving_left_texture = _assets->get<Texture>("walking_left");
    
    explode = _assets->get<Texture>("explode");

    background_music = _assets->get<Sound>("background_sound");
    bubble_sound = _assets->get<Sound>("bubble");
    victory_sound = _assets->get<Sound>("victory");
    lose_sound = _assets->get<Sound>("lose");
    ocean_sound = _assets->get<Sound>("ocean");
    shark_sound = _assets->get<Sound>("shark");

	_overlay = InGameOverlay::alloc(_assets, safe);
	Button::Listener main_menu_callback = [=](const std::string& name, bool down) {
        if (!down) {
            CULog("MAIN MENU PRESSED");
            this->setActive(false);
        }
    };
	_overlay->setMainMenuCallback(main_menu_callback);
	Button::Listener resume_callback = [=](const std::string& name, bool down) {
        if (!down) {
            this->setState(PLAY); CULog("RESUME PRESSED");
            AudioChannels::get()->stopAllEffects();
            AudioChannels::get()->playEffect("bubble", bubble_sound);
        }
    };
	_overlay->setResumeCallback(resume_callback);
	Button::Listener retry_callback = [=](const std::string& name, bool down) {
        if (!down) {
            AudioChannels::get()->stopAllEffects();
            AudioChannels::get()->playEffect("bubble", bubble_sound);
            this->reset();
            this->setState(PLAY);
            CULog("RETRY PRESSED");
        }
    };
	_overlay->setRetryCallback(retry_callback);
	Button::Listener continue_callback = [=](const std::string& name, bool down) {
        if (!down) {
            AudioChannels::get()->stopAllEffects();
            AudioChannels::get()->playEffect("bubble", bubble_sound);
            this->reset();
            CULog("CONTINUE PRESSED");
        }
    };
	_overlay->setContinueCallback(continue_callback);
	Button::Listener pause_callback = [=](const std::string& name, bool down) {
        if (!down) {
            AudioChannels::get()->stopAllEffects();
            AudioChannels::get()->playEffect("bubble", bubble_sound);
            this->setState(PAUSE);
            CULog("PAUSE GAME PRESSED");
        }
    };
	_overlay->setPauseCallback(pause_callback);

	_overlay->setState(PLAY);
    
    AudioChannels::get()->playMusic(background_music, true, background_music->getVolume());
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
    //_fish_countdown = 10;
    _current_level = level;

	this->removeAllChildren();

	_gamestate = _assets->get<GameState>(level);
    
    createLevelAssets();

	if (_world != nullptr)
		_world.reset();

	_world = ObstacleWorld::alloc(physics_bounds, Vec2(0, -9.8));
	_gamestate->initPhysics(_world);
	    _world->activateCollisionCallbacks(true);
    _world->onBeginContact = [this](b2Contact* contact) {
        beginContact(contact);
    };
    _world->onEndContact = [this](b2Contact* contact) {
        endContact(contact);
    };

	if (_map_vc != nullptr)
		_map_vc.reset();

	_map_vc = PlatformMapViewController::alloc(_gamestate, _input, tilesheet, tilesheet_moveable, goal_texture, size);
	addChild(_map_vc->getNode(), 1);
    
    if (_player_vc != nullptr){
        _player_vc.reset();
        CULog("resetting player");
    }
    _player_vc = PlayerViewController::alloc(_gamestate, diving_texture, diving_left_texture, size);

    _map_vc->getNode()->addChild(_player_vc->getNode(),1);
    _playerFloor = false;

	_urchin_vcs.clear();
	//Create Urchin viewcontrollers
    for (int i = 0; i < _gamestate->_urchins.size();i++) {
        shared_ptr<UrchinViewController> urchin_vc = UrchinViewController::alloc(_gamestate, _urchin, size, i);
        _map_vc->getNode()->addChild(urchin_vc->getNode(), 1);
        _urchin_vcs.push_back(urchin_vc);
    }

	//Create Fish viewcontrollers
	_fish_vcs.clear();
    for (int i = 0; i < _gamestate->_fish.size(); i++) {
        shared_ptr<FishViewController> _fish_vc = FishViewController::alloc(_gamestate, _shark, _shark_left, explode, size, i);
        _map_vc->getNode()->addChild(_fish_vc->getNode(), 1);
        _fish_vcs.push_back(_fish_vc);
    }
    
    for (int i = 0; i < _gamestate->_fish.size(); i++) {
        _fish_countdown[i] =10;
    }

	//Create Fish viewcontrollers
	_angler_vcs.clear();
	for (int i = 0; i < _gamestate->_anglers.size(); i++) {
		shared_ptr<AnglerViewController> _angler_vc = AnglerViewController::alloc(_gamestate, _angler, _angler, explode, size, i);
		_map_vc->getNode()->addChild(_angler_vc->getNode(), 1);
		_angler_vcs.push_back(_angler_vc);
	}

	addChild(_background, 0);
	addChild(_overlay->_root_node, 4);
	sortZOrder();
	setState(PLAY);
    Application::get()->setClearColor(Color4f::BLACK);
    AudioChannels::get()->playMusic(background_music, true, background_music->getVolume());

	_gamestate->reset();
	_player_vc->setAIDirection(_gamestate, "down");
}

void GameScene::reset() {
    if(current_state == WIN){
        _current_level = cycleLevel();
    }
    _gamestate->reset();
    _player_vc->setAIDirection(_gamestate, "down");

    CULog("Reset");
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
            _player_vc->setAIDirection(_gamestate, _player_vc->getAIDirection());
            _playerFloor = true;
        }
        else if(bd2->getName() == "goal") {
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
		else if (bd2->getName().find("fish") != string::npos) {
			fishFishCollisions(bd1, bd2);
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
	} else {
		_player_vc->setAIDirection(_gamestate, "left");
	}
}

void GameScene::fishPlatformCollisions(Obstacle* fish, Obstacle* platform){
    char num = fish->getName().back();
    int fishint = num-'0';
    if(_fish_countdown[fishint]<=0){
        _fish_countdown[fishint]=10;
        _fish_vcs[fishint]->setDirection(_gamestate->_fish[fishint]);
        _gamestate->_fish[fishint]->setLeft(!_gamestate->_fish[fishint]->isLeft());

    }
}

void GameScene::fishFishCollisions(Obstacle* fish, Obstacle* fish2) {
	CULog("SHARK COLLIDE");
	char num1 = fish->getName().back();
	int fishint1 = num1 - '0';

	if (_fish_countdown[fishint1] <= 0) {
		_fish_countdown[fishint1] = 10;
		_fish_vcs[fishint1]->setDirection(_gamestate->_fish[fishint1]);
		_gamestate->_fish[fishint1]->setLeft(!_gamestate->_fish[fishint1]->isLeft());
	}

	char num2 = fish2->getName().back();
	int fishint2 = num2 - '0';
	if (_fish_countdown[fishint2] <= 0) {
		_fish_countdown[fishint2] = 10;
		_fish_vcs[fishint2]->setDirection(_gamestate->_fish[fishint2]);
		_gamestate->_fish[fishint2]->setLeft(!_gamestate->_fish[fishint2]->isLeft());
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
            if(_block_counter<0){
                _block_counter = 0;
            }
            //change ai direction
            if(_block_counter==0){
                _player_vc->setAIDirection(_gamestate, "down");
                _playerFloor = false;
            }
        }
    }
    else if(bd1->getName() == "player"){
        if(bd2->getName() == "platform"){
            _block_counter--;
            if(_block_counter<0){
                _block_counter = 0;
            }
            //change ai direction
            if(_block_counter==0){
                _player_vc->setAIDirection(_gamestate, "down");
                _playerFloor = false;
            }
        }
    }
}
    
