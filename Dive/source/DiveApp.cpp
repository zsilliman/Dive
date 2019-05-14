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
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>
#include "Entities/Player.h"
#include "Loaders/GamestateLoader.h"

// This keeps us from having to write cugl:: all the time
using namespace cugl;

// The number of frames before moving the logo to a new position
#define TIME_STEP 60

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
    // Create an asset manager to load all assets
    _assets = AssetManager::alloc();
	_batch = SpriteBatch::alloc();
    
    // You have to attach the individual loaders for each asset type
    _assets->attach<Texture>(TextureLoader::alloc()->getHook());
    _assets->attach<Font>(FontLoader::alloc()->getHook());
	_assets->attach<Node>(SceneLoader::alloc()->getHook());
	_assets->attach<GameState>(GamestateLoader::alloc()->getHook());
	_assets->attach<JsonValue>(JsonLoader::alloc()->getHook());
    _assets->attach<Sound>(SoundLoader::alloc()->getHook());
    
    AudioChannels::start(24);

    // Activate mouse or touch screen input as appropriate
    // We have to do this BEFORE the scene, because the scene has a button
#if defined (CU_TOUCH_SCREEN)
    Input::activate<Touchscreen>();
#else
    Input::activate<Mouse>();
	Input::activate<Keyboard>();
#endif
    
	// Create a scene graph the same size as the window
	setClearColor(Color4(229, 229, 229, 255));

	// Create a "loading" screen
	_title.init(_assets);
	_loaded = false;

	// This reads the given JSON file and uses it to load all other assets
    _animated = false;
    _menued = false;
	_assets->loadDirectoryAsync("json/assets.json", nullptr);

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
    AudioChannels::stop();
    // Delete all smart pointers
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
<<<<<<< HEAD

//    if (!_loaded && _loading.isActive()) {
//        _loading.update(0.01f);
//    }
//    else if (!_loaded) {
//        _loading.dispose(); // Disables the input listeners in this mode
//        _gameplay.init(_assets);
//        _loaded = true;
//    }
//    else {
//        _gameplay.update(timestep);
//    }

    if (!_loaded && _loading.isActive()) {
        _loading.update(0.01f);
    }
    else if (!_loaded) {
        _loading.dispose(); // Disables the input listeners in this mode
        _title.init(_assets);
        _loaded = true;
    }
    else if (!_animated && _title.isActive()){
        _title.update(timestep);
    }
    else if (!_animated){
        _title.dispose();
        _menu.init(_assets);
        _animated = true;
    }
    else if (!_menued && _menu.isActive()){
        _menu.update(timestep);
    }
    else if (!_menued){
        _menu.dispose();
        _gameplay.init(_assets);
        _menued = true;
    }
    else{
        _gameplay.update(timestep);
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
	if (!_loaded) {
		_title.render(_batch);
	}
    else if (!_animated){
        _title.render(_batch);
    }
    else if (!_menued){
        _menu.render(_batch);
    }
	else {
		_gameplay.render(_batch);
	}
}
