//
//  TitleAnimationScene.hpp
//  Dive
//
//  Created by Pippen Wu on 2019/3/27.
//  2019 Game Design Initiative at Cornell.
//

#ifndef TitleAnimationScene_hpp
#define TitleAnimationScene_hpp

#include <cugl/cugl.h>

/**
 * This class is plays the title animation once.
 */
class TitleAnimationScene : public cugl::Scene {
protected:
    /** The asset manager for loading. */
    std::shared_ptr<cugl::AssetManager> _assets;
    
    /** Reference to the animation node */
    std::shared_ptr<cugl::AnimationNode> _titlenode;
    
    int NUMBER_FRAMES = 117;
    int MAX_COOLDOWN = 1; //3;
    int LINGER = 5; //40;
    /** "Cooldown" in between frames */
    int _cooldown = 3;
    /** How to linger on the last frame */
    int _linger = LINGER;
    /** If the animation is done */
    bool _finished = false;
    
    
public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new loading mode with the default values.
     *
     * This constructor does not allocate any objects or start the game.
     * This allows us to use the object without a heap pointer.
     */
    TitleAnimationScene() : Scene() {}
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~TitleAnimationScene() { dispose(); }
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    void dispose();
    
    /**
     * Initializes the controller contents, making it ready for loading
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
    
    
#pragma mark -
#pragma mark Progress Monitoring
    /**
     * The method called to update the game mode.
     *
     * This method updates the frame to be drawn in the animation.
     *
     * @param timestep  The amount of time (in seconds) since the last frame
     */
    void update(float timestep);
    
};
#endif /* TitleAnimationScene_hpp */
