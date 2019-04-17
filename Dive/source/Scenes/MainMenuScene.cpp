//
//  MainMenuScene.cpp
//  Dive
//

#include "MainMenuScene.hpp"

using namespace cugl;

/** The ID for the button listener */
#define LISTENER_ID 1
/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 1024

#pragma mark -
#pragma mark Constructors

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
bool MainMenuScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(dimen)) {
        return false;
    }
    
    _assets = assets;
    auto layer = assets->get<Node>("mainmenu");
    layer->setContentSize(dimen);
    layer->doLayout(); // This rearranges the children to fit the screen
    addChild(layer);
    
    _button = std::dynamic_pointer_cast<Button>(assets->get<Node>("mainmenu_action"));
    _label  = std::dynamic_pointer_cast<Label>(assets->get<Node>("mainmenu_action_up_label"));
    _button->setListener([=](const std::string& name, bool down) {
        if (down) {
            this->_counter++;
            this->_label->setText("Click Count is "+cugl::to_string(this->_counter));
        }
    });
    
    level1 = std::dynamic_pointer_cast<Button>(assets->get<Node>("mainmenu_bluebg_level1"));
    level1->setListener([=](const std::string& name, bool down) {
        if (down) {
            CULog("level 1 selected");
        }
    });
    level2 = std::dynamic_pointer_cast<Button>(assets->get<Node>("mainmenu_bluebg_level2"));
    level2->setListener([=](const std::string& name, bool down) {
        if (down) {
            CULog("level 2 selected");
        }
    });
    level3 = std::dynamic_pointer_cast<Button>(assets->get<Node>("mainmenu_bluebg_level3"));
    level3->setListener([=](const std::string& name, bool down) {
        if (down) {
            CULog("level 3 selected");
            this->_counter++;
        }
    });
    level4 = std::dynamic_pointer_cast<Button>(assets->get<Node>("mainmenu_bluebg_level4"));
    level4->setListener([=](const std::string& name, bool down) {
        if (down) {
            CULog("level 4 selected");
        }
    });
    level5 = std::dynamic_pointer_cast<Button>(assets->get<Node>("mainmenu_bluebg_level5"));
    level5->setListener([=](const std::string& name, bool down) {
        if (down) {
            CULog("level 5 selected");
        }
    });
    
    if (_active) {
        _button->activate(0);
        level1->activate(1);
        level2->activate(2);
        level3->activate(3);
        level4->activate(4);
        level5->activate(5);
    }
    
    // XNA nostalgia
    Application::get()->setClearColor(Color4f::CORNFLOWER);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void MainMenuScene::dispose() {
    _button = nullptr;
    _label  = nullptr;
    _assets = nullptr;
    level1 = nullptr;
    level2 = nullptr;
    level3 = nullptr;
    level4 = nullptr;
    level5 = nullptr;
    Scene::dispose();
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
void MainMenuScene::update(float progress) {
    if (_counter>=5){
        this->_active = false;
    }
}

/**
 * Returns true if loading is complete, but the player has not pressed play
 *
 * @return true if loading is complete, but the player has not pressed play
 */
bool MainMenuScene::isPending( ) const {
    return false;
}
