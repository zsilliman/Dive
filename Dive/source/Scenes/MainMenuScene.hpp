//
//  MainMenuScene.hpp
//  Dive
//

#ifndef MainMenuScene_hpp
#define MainMenuScene_hpp

#include <cugl/cugl.h>

class MainMenuScene : public cugl::Scene {
protected:
    /** The asset manager for loading. */
    std::shared_ptr<cugl::AssetManager> _assets;
    /** A scene graph, used to display our 2D scenes */
    std::shared_ptr<cugl::Scene> _scene;
    
    // NO CONTROLLER (ALL IN SEPARATE THREAD)
    
    // VIEW
//    /** The animated progress bar */
//    std::shared_ptr<cugl::ProgressBar>  _bar;
//    /** The "play" button */
//    std::shared_ptr<cugl::Button> _button;
//
    // MODEL
//    std::shared_ptr<cugl::Button> _button;
//    std::shared_ptr<cugl::Label> _label;
    int _counter;
    int _level_selected=0;
    int _current = 1;
    
    std::shared_ptr<cugl::Button> downarr;
     std::shared_ptr<cugl::Button> uparr;
    
    std::shared_ptr<cugl::Button> surface;
    std::shared_ptr<cugl::Button> depths;
    
    std::shared_ptr<cugl::Button> level1;
    std::shared_ptr<cugl::Button> level2;
    std::shared_ptr<cugl::Button> level3;
    std::shared_ptr<cugl::Button> level4;
    std::shared_ptr<cugl::Button> level5;
    std::shared_ptr<cugl::Button> level6;
    std::shared_ptr<cugl::Button> level7;
    std::shared_ptr<cugl::Button> level8;
    std::shared_ptr<cugl::Button> level9;
    std::shared_ptr<cugl::Button> level10;
    std::shared_ptr<cugl::Button> level11;
    std::shared_ptr<cugl::Button> level12;
    
    
public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new main menu with the default values.
     *
     * This constructor does not allocate any objects or start the game.
     * This allows us to use the object without a heap pointer.
     */
    MainMenuScene() : _counter(0) {}
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~MainMenuScene() { dispose(); }
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    void dispose();
    
    /**
     * The constructor does not allocate any objects or memory.  This allows
     * us to have a non-pointer reference to this controller, reducing our
     * memory allocation.  Instead, allocation happens in this method.
     *
     * @param assets    The (loaded) assets for this game mode
     *
     * @return true if the controller is initialized properly, false otherwise.
     */
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);
    
    static std::shared_ptr<MainMenuScene> alloc(const std::shared_ptr<cugl::AssetManager>& assets) {
        std::shared_ptr<MainMenuScene> result = std::make_shared<MainMenuScene>();
        return (result->init(assets) ? result : nullptr);
    }
    
    
#pragma mark -
#pragma mark Progress Monitoring
    /**
     * The method called to update the game mode.
     *
     * This method updates the progress bar amount.
     *
     * @param timestep  The amount of time (in seconds) since the last frame
     */
    void update(float timestep);
    
    /**
     * Returns true if loading is complete, but the player has not pressed play
     *
     * @return true if loading is complete, but the player has not pressed play
     */
    bool isPending( ) const;
    
    int getLevelSelected();
    
    void setLevelSelected(int level);
};

#endif /* MainMenuScene_hpp */
