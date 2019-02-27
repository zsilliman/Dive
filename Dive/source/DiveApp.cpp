//
//  DiveApp.cpp
//  Cornell University Game Library (CUGL)
//
//  This is the implementation file for the custom application. This is the
//  definition of your root (and in this case only) class.
//
//  CUGL zlib License:
//      This software is provided 'as-is', without any express or implied
//      warranty.  In no event will the authors be held liable for any damages
//      arising from the use of this software.
//
//      Permission is granted to anyone to use this software for any purpose,
//      including commercial applications, and to alter it and redistribute it
//      freely, subject to the following restrictions:
//
//      1. The origin of this software must not be misrepresented; you must not
//      claim that you wrote the original software. If you use this software
//      in a product, an acknowledgment in the product documentation would be
//      appreciated but is not required.
//
//      2. Altered source versions must be plainly marked as such, and must not
//      be misrepresented as being the original software.
//
//      3. This notice may not be removed or altered from any source distribution.
//
//  Author: Walker White
//  Version: 1/8/17
//
// Include the class header, which includes all of the CUGL classes
#include "DiveApp.h"
#include <cugl/base/CUBase.h>

// Add support for simple random number generation
#include <cstdlib>
#include <ctime>
#include "Player.h"
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>

// This keeps us from having to write cugl:: all the time
using namespace cugl;

// The number of frames before moving the logo to a new position
#define TIME_STEP 60
// This is adjusted by screen aspect ratio to get the height
#define GAME_WIDTH 1024

#define GOAL_TEXTURE    "goal"
#define MESSAGE_FONT    "charlemagne"
#define WIN_MESSAGE     "VICTORY!"
#define WIN_COLOR       Color4::YELLOW
#define EXIT_COUNT      240

/**
 * The method called after OpenGL is initialized, but before running the application.
 *
 * This is the method in which all user-defined program intialization should
 * take place.  You should not create a new init() method.
 *
 * When overriding this method, you should call the parent method as the
 * very last line.  This ensures that the state will transition to FOREGROUND,
 * causing the application to run.
 */
void DiveApp::onStartup() {
    Size size = getDisplaySize();
    size *= GAME_WIDTH/size.width;
    
    // Create a scene graph the same size as the window
    _scene = Scene::alloc(size.width, size.height);
    // Create a sprite batch (and background color) to render the scene
    _batch = SpriteBatch::alloc();
    setClearColor(Color4(229,229,229,255));
    
    // Create an asset manager to load all assets
    _assets = AssetManager::alloc();
    
    // You have to attach the individual loaders for each asset type
    _assets->attach<Texture>(TextureLoader::alloc()->getHook());
    _assets->attach<Font>(FontLoader::alloc()->getHook());
    
    // This reads the given JSON file and uses it to load all other assets
    _assets->loadDirectory("json/assets.json");

    // Activate mouse or touch screen input as appropriate
    // We have to do this BEFORE the scene, because the scene has a button
#if defined (CU_TOUCH_SCREEN)
    Input::activate<Touchscreen>();
#else
    Input::activate<Mouse>();
	Input::activate<Keyboard>();
#endif
    
    //define rect bounds of the physics engine
    cugl::Rect phys_bounds = cugl::Rect();
    phys_bounds.origin.x = -1000;
    phys_bounds.origin.y = -1000;
    phys_bounds.size.width = 2000;
    phys_bounds.size.height = 2000;
    
    //Create physics world
    _world = cugl::ObstacleWorld::alloc(phys_bounds, Vec2(0, 0));
    
    _world->onBeginContact = [this](b2Contact* contact) {
        beginContact(contact);
    };
    
    
    // Build the scene from these assets
    buildScene();

    Application::onStartup();
}

/**
 * The method called when the application is ready to quit.
 *
 * This is the method to dispose of all resources allocated by this
 * application.  As a rule of thumb, everything created in onStartup()
 * should be deleted here.
 *
 * When overriding this method, you should call the parent method as the
 * very last line.  This ensures that the state will transition to NONE,
 * causing the application to be deleted.
 */
void DiveApp::onShutdown() {
    // Delete all smart pointers
    _scene = nullptr;
    _batch = nullptr;
    _assets = nullptr;
    
    // Deativate input
#if defined CU_TOUCH_SCREEN
    Input::deactivate<Touchscreen>();
#else
    Input::deactivate<Mouse>();
	Input::deactivate<Keyboard>();
#endif
    Application::onShutdown();
}

/**
 * The method called to update the application data.
 *
 * This is your core loop and should be replaced with your custom implementation.
 * This method should contain any code that is not an OpenGL call.
 *
 * When overriding this method, you do not need to call the parent method
 * at all. The default implmentation does nothing.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void DiveApp::update(float timestep) {
    Size size = getDisplaySize();
    size *= GAME_WIDTH/size.width;

	cugl::Mouse* mouse = Input::get<cugl::Mouse>();
	//if (mouse->buttonDown().hasLeft()) {
	_player->push_direction(mouse->pointerPosition(), _platform_map);
	//}
    CULog("%s", std::to_string(timestep).c_str());
	_world->update(timestep);
	_platform_map->parallaxTranslatePlatforms(_player->_node->getPosition(), _player->getdX(timestep));

	_platform_map->updatePlatformPositions();
	//Centers map around the player
	_platform_map->anchorCameraTo(_player->getPhysicsPosition(), _player->_node->getPosition());
    
//    sprite->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
//    sprite -> setPosition(-_player->getPhysicsPosition().x + _player->_node->getWorldPosition().x , -_player->getPhysicsPosition().y + _player->_node->getWorldPosition().y);
//    
    if (_countdown > 0) {
        _countdown--;
    } else if (_countdown == 0) {
        reset();
    }
}

/**
 * The method called to draw the application to the screen.
 *
 * This is your core loop and should be replaced with your custom implementation.
 * This method should OpenGL and related drawing calls.
 *
 * When overriding this method, you do not need to call the parent method
 * at all. The default implmentation does nothing.
 */
void DiveApp::draw() {
    // This takes care of begin/end
    _scene->render(_batch);
}

void DiveApp::setComplete(bool value) {
    bool change = _complete != value;
    _complete = value;
    if (value && change) {
        _winnode->setVisible(true);
        _countdown = EXIT_COUNT;
    } else if (!value) {
        _winnode->setVisible(false);
        _countdown = -1;
    }
}

void DiveApp::beginContact(b2Contact* contact) {
    b2Fixture* fix1 = contact->GetFixtureA();
    b2Fixture* fix2 = contact->GetFixtureB();
    
    b2Body* body1 = fix1->GetBody();
    b2Body* body2 = fix2->GetBody();
    
    Obstacle* bd1 = (Obstacle*)body1->GetUserData();
    Obstacle* bd2 = (Obstacle*)body2->GetUserData();
    
    if((bd1 == _player.get() && bd2 == _goalDoor.get()) ||
       (bd1 == _goalDoor.get() && bd2 == _player.get())) {
        setComplete(true);
    }
}

/**
 * Internal helper to build the scene graph.
 *
 * Scene graphs are not required.  You could manage all scenes just like
 * you do in 3152.  However, they greatly simplify scene management, and
 * have become standard in most game engines.
 */
void DiveApp::buildScene() {
    Size  size  = getDisplaySize();
    scale = GAME_WIDTH/size.width;
    size *= scale;
    
    // Create a button.  A button has an up image and a down image
    std::shared_ptr<Texture> up   = _assets->get<Texture>("close-normal");
    std::shared_ptr<Texture> down = _assets->get<Texture>("close-selected");
    
    Size bsize = up->getSize();
    std::shared_ptr<Button> button = Button::alloc(PolygonNode::allocWithTexture(up),
                                                   PolygonNode::allocWithTexture(down));
    
    // Create a callback function for the button
    button->setName("close");
    button->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            CULog("Goodbye!");
            this->quit();
        }
    });

    // Find the safe area, adapting to the iPhone X
    Rect safe = getSafeArea();
    safe.origin *= scale;
    safe.size   *= scale;
    
    // Get the right and bottom offsets.
    float bOffset = safe.origin.y;
    float rOffset = (size.width)-(safe.origin.x+safe.size.width);
    
    // Position the button in the bottom right corner
    button->setAnchor(Vec2::ANCHOR_CENTER);
    button->setPosition(size.width-(bsize.width+rOffset)/2,(bsize.height+bOffset)/2);

    _scene->addChild(button);
    
    // We can only activate a button AFTER it is added to a scene
    button->activate(1);

    // Start the logo countdown and C-style random number generator
    _countdown = TIME_STEP;
    std::srand((int)std::time(0));

	// A REALLY MESS JUMBLE OF CODE THAT INITIALIZES THE WORLD

    //Goal Setup
    
    _winnode = Label::alloc(WIN_MESSAGE, _assets->get<Font>(MESSAGE_FONT));
    _winnode->setAnchor(Vec2::ANCHOR_CENTER);
    _winnode->setPosition(size.width/2,size.height/2);
    _winnode->setForeground(WIN_COLOR);
    _winnode->setVisible(false);
    _scene -> addChild(_winnode,3);
    setComplete(false);

    float GOAL_POS[] = {20, -100};
    Vec2 goalPos = ((Vec2)GOAL_POS);
    std::shared_ptr<Texture> image = _assets->get<Texture>(GOAL_TEXTURE);
    Size goalSize = image->getSize()/scale;
    std::shared_ptr<PolygonNode> sprite = PolygonNode::allocWithTexture(image);

    _goalDoor = BoxObstacle::alloc(goalPos,goalSize);

    // Set the physics attributes
    _goalDoor->setBodyType(b2_staticBody);
    _goalDoor->setDensity(0.0f);
    _goalDoor->setFriction(0.0f);
    _goalDoor->setRestitution(0.0f);
    _goalDoor->setSensor(true);
    _world->addObstacle(_goalDoor);
    sprite->setPosition(_goalDoor->getPosition()*scale);
    _scene->addChild(sprite,0);
    
	//Load texture of the platforms
	std::shared_ptr<Texture> platform_tex = _assets->get<Texture>("blank");

	//Create the platform map
	_platform_map = PlatformMap::alloc();

	//Create some platforms
	for (int i = 0; i < 5; i++) {
        if (i > 0){
            //Allocate
            shared_ptr<Platform> p = Platform::allocWithTexture(platform_tex);
            //Set starting location
            p->setInitialPosition(-200 + i * 100, i * 100);
            //Set size (texture is 16x16px)
            p->setScale(30, 2);
            //Initialize physics
            p->initPhysics(_world);
            //Add it to the map
            _platform_map->addPlatform(p);
            
            shared_ptr<Platform> p2 = Platform::allocWithTexture(platform_tex);
            //Set starting location
            p2->setInitialPosition(200 + i * 100, i * 100);
            //Set size (texture is 16x16px)
            p2->setScale(10, 2);
            //Initialize physics
            p2->initPhysics(_world);
            //Add it to the map
            _platform_map->addPlatform(p2);
            
            shared_ptr<Platform> p3 = Platform::allocWithTexture(platform_tex);
            //Set starting location
            p3->setInitialPosition(600 + i * 100, i * 100);
            //Set size (texture is 16x16px)
            p3->setScale(30, 2);
            //Initialize physics
            p3->initPhysics(_world);
            //Add it to the map
            _platform_map->addPlatform(p3);
        }else {
            shared_ptr<Platform> p = Platform::allocWithTexture(platform_tex);
            //Set starting location
            p->setInitialPosition(-200 + i * 100, i * 100);
            //Set size (texture is 16x16px)
            p->setScale(150, 2);
            //Initialize physics
            p->initPhysics(_world);
            //Add it to the map
            _platform_map->addPlatform(p);
        }
	}
	//Create the player set some basic stuff
	_player = Player::allocWithTexture(platform_tex);
	//_player->setPosition(420, 420);
	_player->setScale(4, 4);
	_player->_node->setPosition(Vec2(size.width/2, size.height/2));
	//Initialize all of its physics properties and add it to the physics world
	_player->initPhysics(_world);
	_player->setPhysicsPosition(420, 420);
	//Add player to the _platform_map, just makes relative positions easier
	_scene->addChild(_player->_node);
	//Add the platform map to the scene
	_platform_map->addToScene(_scene);
	//This sets the size of the "circle" so platforms teleport to the start
	_platform_map->setMapSize(10, 1000);
}


void DiveApp::reset() {
    _world->clear();
    _scene->removeAllChildren();
    setComplete(false);
    buildScene();
}
