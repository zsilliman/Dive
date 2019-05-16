//
//  GameScene.h
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
//  Version: 1/10/18
//
#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__
#include "../GameState.h"
#include "../ViewControllers/PlatformMapViewController.h"
#include "../ViewControllers/PlayerViewController.h"
#include "../ViewControllers/UrchinViewController.h"
#include "../ViewControllers/FishViewController.h"
#include "../ViewControllers/AnglerViewController.h"
#include "../ViewControllers/GoalViewController.h"
#include "../ViewControllers/InputController.h"
#include "InGameOverlay.h"
#include "../Util.h"
#include <cugl/cugl.h>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <vector>

/**
 * This class is the primary gameplay constroller for the demo.
 *
 * A world has its own objects, assets, and input controller.  Thus this is
 * really a mini-GameEngine in its own right.  As in 3152, we separate it out
 * so that we can have a separate mode for the loading screen.
 */
class GameScene : public cugl::Scene {
protected:
    /** The asset manager for this game mode. */
    std::shared_ptr<cugl::AssetManager> _assets;
    //Stuff that gets loaded once for all levels
	shared_ptr<Texture> texture;
	shared_ptr<Texture> goal_texture;
	shared_ptr<TiledTexture> tilesheet;
	shared_ptr<TiledTexture> tilesheet_moveable;
    
	shared_ptr<Texture> _urchin;
	shared_ptr<Texture> _shark;
    shared_ptr<Texture> _shark_left;
	shared_ptr<Texture> _angler;
    
    shared_ptr<Texture> explode;
    
	shared_ptr<Texture> background_image;
	shared_ptr<Texture> diving_texture;
    shared_ptr<Texture> diving_left_texture;
    //sounds
    std::shared_ptr<Sound> bubble_sound;
    std::shared_ptr<Sound> victory_sound;
    std::shared_ptr<Sound> lose_sound;
    std::shared_ptr<Sound> background_music;
    std::shared_ptr<Sound> ocean_sound;
    std::shared_ptr<Sound> shark_sound;

    int waveCounter;

    
	std::shared_ptr<cugl::Node> _background = nullptr;
	std::shared_ptr<InputController> _input = nullptr;
	std::shared_ptr<InGameOverlay> _overlay = nullptr;

	//Loaded again for each new level
	//Gamestate contains all necessary models
	std::shared_ptr<GameState> _gamestate = nullptr;
	float scale;

    // COUPLED VIEW + CONTROLLERS
	/** controller that manages how the user controls world translations */
	std::shared_ptr<PlatformMapViewController> _map_vc = nullptr;
    std::shared_ptr<PlayerViewController> _player_vc = nullptr;
	std::vector<std::shared_ptr<UrchinViewController>> _urchin_vcs = {};
	std::vector<std::shared_ptr<FishViewController>> _fish_vcs = {};
	std::vector<std::shared_ptr<AnglerViewController>> _angler_vcs = {};

    //input controller
	std::shared_ptr<ObstacleWorld> _world = nullptr;

	//Non-shared_ptr fields
	State prev_state = PLAY;
	State current_state = PLAY;
	bool _complete = false;

    int _block_counter;
    int _animation_counter;
    int _diver_angle;
    int _prev_diver_angle;
    int _fish_remove;
	int _angler_remove;
    int _fish_countdown;
	int _player_side_count;
    string _current_level;
    string _next_level;
    int final_level = 10;
    int counter;

	int frame_counter = 0;
	bool _playerFloor;

	//Setting up the scene
	void buildYellow();
    void buildRed();
    void buildBlue();
    void buildPurple();
    void createLevelAssets();
    void buildOnce();
	void buildScene(string level);
    
public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new game mode with the default values.
     *
     * This constructor does not allocate any objects or start the game.
     * This allows us to use the object without a heap pointer.
     */
    GameScene() : Scene() {}
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~GameScene() { dispose(); }
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    void dispose();
    
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
    bool init(const std::shared_ptr<cugl::AssetManager>& assets, int level);

	void setState(State state);
    
#pragma mark -
#pragma mark Gameplay Handling
    /**
     * The method called to update the game mode.
     *
     * This method contains any gameplay code that is not an OpenGL call.
     *
     * @param timestep  The amount of time (in seconds) since the last frame
     */
    void update(float timestep);

    /**
     * Resets the status of the game so that we can play again.
     */
    void reset();
    
#pragma mark Collision Handling
    /**
     * Processes the start of a collision
     *
     * This method is called when we first get a collision between two objects.
     * We use this method to test if it is the "right" kind of collision.  In
     * particular, we use it to test if we make it to the win door.
     *
     * @param  contact  The two bodies that collided
     */
    void beginContact(b2Contact* contact);
    
	void playerSidePlatformCollisions(Obstacle* player_side, Obstacle* platform, bool left);

    void fishPlatformCollisions(Obstacle* fish, Obstacle* platform);
    
    void diverPlatformCollisions(Obstacle* diver, Obstacle* platform);
    
    void fishAnglerCollisions(Obstacle* fish, Obstacle* angler);
    
    void fishUrchinCollisions(Obstacle* fish, Obstacle* urchin);
    
    void anglerUrchinCollisions(Obstacle* angler, Obstacle* urchin);

    string cycleLevel();
    
    void startLevel(int level);

    /**
     * Handles any modifications necessary before collision resolution
     *
     * This method is called just before Box2D resolves a collision.  We use
     * this method to implement sound on contact, using the algorithms outlined
     * in Ian Parberry's "Introduction to Game Physics with Box2D".
     *
     * @param  contact  The two bodies that collided
     * @param  contact  The collision manifold before contact
     */
    //void beforeSolve(b2Contact* contact, const b2Manifold* oldManifold);
    
    /**
     * Processes the end of a collision
     *
     * This method is called when we first get a collision between two objects.
     * We use this method to test if it is the "right" kind of collision.  In
     * particular, we use it to test if we make it to the win door.
     *
     * @param  contact  The two bodies that collided
     */
    void endContact(b2Contact* contact);
};

#endif /* __GAME_SCENE_H__ */
