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
#include "../ViewControllers/GoalViewController.h"
#include "../ViewControllers/InputController.h"
#include <cugl/cugl.h>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <vector>

//Step world AI once every 20 frames
#define UPDATE_STEP 20
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
    

    

	//MODELS
    // Gamestate contains all necessary models
	std::shared_ptr<GameState> _gamestate;
    
	std::shared_ptr<cugl::Node> _background;

	float scale;

    // COUPLED VIEW + CONTROLLERS
	/** controller that manages how the user controls world translations */
	std::shared_ptr<PlatformMapViewController> _map_vc;
    std::shared_ptr<PlayerViewController> _player_vc;
	std::vector<std::shared_ptr<UrchinViewController>> _urchin_vcs;
	std::vector<std::shared_ptr<FishViewController>> _fish_vcs;
    //input controller
    std::shared_ptr<InputController> _input;
    
	std::shared_ptr<ObstacleWorld> _world;
    std::shared_ptr<cugl::Label> _winnode;
    std::shared_ptr<cugl::Label> _losenode;

	enum State { WIN, LOSE, PLAY };
	State prev_state = PLAY;
	State current_state = PLAY;
	bool _complete = false;

    int _countdown;

	int frame_counter = 0;

	//Setting up the scene
	void buildScene();
    
    bool _playerFloor;
    
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
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);

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
    
    
    void endContact(b2Contact* contact);
    
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
    
};

#endif /* __GAME_SCENE_H__ */
