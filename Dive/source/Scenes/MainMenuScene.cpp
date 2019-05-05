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
    auto layer0 = assets->get<Node>("mainmenu");
    auto layer = assets->get<Node>("levelselect");
    auto layer2 = assets->get<Node>("credits");
    layer0->setContentSize(dimen);
    layer->setContentSize(dimen);
    layer2->setContentSize(dimen);
//    layer->doLayout(); // This rearranges the children to fit the screen
    addChild(layer0);
    addChild(layer);
    addChild(layer2);
    
//    _button = std::dynamic_pointer_cast<Button>(assets->get<Node>("mainmenu_action"));
//    _label  = std::dynamic_pointer_cast<Label>(assets->get<Node>("mainmenu_action_up_label"));
//    _button->setListener([=](const std::string& name, bool down) {
//        if (down) {
//            this->_counter++;
//            this->_label->setText("Click Count is "+cugl::to_string(this->_counter));
//        }
//    });
    
    layer0->setVisible(true);
    layer->setVisible(false);
    layer2->setVisible(false);
    
    downarr = std::dynamic_pointer_cast<Button>(assets->get<Node>("credits_downarr"));
    downarr->setListener([=](const std::string& name, bool down) {
        if (down) {
            CULog("go from credits to main");
            layer0->setVisible(true);
            layer2->setVisible(false);
            downarr->removeListener();
            surface->activate(6);
            depths->activate(7);
        }
    });
    
    
    surface = std::dynamic_pointer_cast<Button>(assets->get<Node>("mainmenu_surface"));
    surface->setListener([=](const std::string& name, bool down) {
        if (down) {
            CULog("go to credits");
            layer0->setVisible(false);
            layer2->setVisible(true);
            downarr->activate(8);
            surface->removeListener();
            depths->removeListener();
        }
    });
    
    depths = std::dynamic_pointer_cast<Button>(assets->get<Node>("mainmenu_depths"));
    depths->setListener([=](const std::string& name, bool down) {
        if (down) {
            CULog("go to level select");
            layer->setVisible(true);
            layer0->setVisible(false);
            level1->activate(1);
            level2->activate(2);
            level3->activate(3);
            level4->activate(4);
            level5->activate(5);
//            surface->deactivate();
//            depths->deactivate();

        }
    });
    
    level1 = std::dynamic_pointer_cast<Button>(assets->get<Node>("levelselect_level1"));
    level1->setListener([=](const std::string& name, bool down) {
        if (down) {
            CULog("level 1 selected");
        }
    });
    level2 = std::dynamic_pointer_cast<Button>(assets->get<Node>("levelselect_level2"));
    level2->setListener([=](const std::string& name, bool down) {
        if (down) {
            CULog("level 2 selected");
        }
    });
    level3 = std::dynamic_pointer_cast<Button>(assets->get<Node>("levelselect_level3"));
    level3->setListener([=](const std::string& name, bool down) {
        if (down) {
            CULog("level 3 selected");
        }
    });
    level4 = std::dynamic_pointer_cast<Button>(assets->get<Node>("levelselect_level4"));
    level4->setListener([=](const std::string& name, bool down) {
        if (down) {
            CULog("level 4 selected");
        }
    });
    level5 = std::dynamic_pointer_cast<Button>(assets->get<Node>("levelselect_level5"));
    level5->setListener([=](const std::string& name, bool down) {
        if (down) {
            CULog("level 5 selected");
        }
    });
    
    if (_active) {
//        _button->activate(0);
//        level1->activate(1);
//        level2->activate(2);
//        level3->activate(3);
//        level4->activate(4);
//        level5->activate(5);
        surface->activate(6);
        depths->activate(7);
    }
    
    CULog("Button 1 position: (%f,%f)", level1->getPositionX(), level1->getPositionY());
    CULog("Button 2 position: (%f,%f)", level2->getPositionX(), level2->getPositionY());
    CULog("dimension width: %d", Application::get()->getDisplayWidth());
    CULog("dimension height: %d", Application::get()->getDisplayHeight());
    CULog("safe area: (%f,%f), (%f,%f)", Application::get()->getSafeArea().getMinX(), Application::get()->getSafeArea().getMinY(),
         Application::get()->getSafeArea().getMaxX(), Application::get()->getSafeArea().getMaxY());
    
    float x_min = 0;
    float y_min = 0;
    int x_max = Application::get()->getDisplayWidth();
    int y_max = Application::get()->getDisplayHeight();
    float x_min_safe = Application::get()->getSafeArea().getMinX();
    float y_min_safe = Application::get()->getSafeArea().getMinY();
    float x_max_safe = Application::get()->getSafeArea().getMaxX();
    float y_max_safe = Application::get()->getSafeArea().getMaxY();
    float x_safe_offset = (x_min_safe-x_min);
    float y_safe_offset = (y_min_safe-y_min);
    float safe_width = (x_max-x_min)-2*x_safe_offset;
    float safe_height = (y_max-y_min)-2*y_safe_offset;
    
//    CULog("safe width and heigth: %f, %f", safe_width, safe_height);
//    CULog("Button 1 NEW position: (%f,%f)", x_min+x_safe_offset+safe_width/2, y_min+y_safe_offset+safe_height/12*9);
    
    int Y_LIFT = 850;
    int X_LIFT = 300;
    surface->setPosition(x_min+x_safe_offset+safe_width/2 +X_LIFT, y_min+y_safe_offset+safe_height/12*1 +Y_LIFT);
    depths->setPosition(x_min+x_safe_offset+safe_width/2 +X_LIFT, y_min+y_safe_offset+safe_height/12*1 +Y_LIFT - 300);
    
    downarr->setPosition(x_min+x_safe_offset+safe_width/2 +X_LIFT, y_min+y_safe_offset+safe_height/12*1 +Y_LIFT - 300);
    
    level1->setPosition(x_min+x_safe_offset+safe_width/2 +X_LIFT, y_min+y_safe_offset+safe_height/12*9 +Y_LIFT);
    level2->setPosition(x_min+x_safe_offset+safe_width/2 +X_LIFT, y_min+y_safe_offset+safe_height/12*7 +Y_LIFT);
    level3->setPosition(x_min+x_safe_offset+safe_width/2 +X_LIFT, y_min+y_safe_offset+safe_height/12*5 +Y_LIFT);
    level4->setPosition(x_min+x_safe_offset+safe_width/2 +X_LIFT, y_min+y_safe_offset+safe_height/12*3 +Y_LIFT);
    level5->setPosition(x_min+x_safe_offset+safe_width/2 +X_LIFT, y_min+y_safe_offset+safe_height/12*1 +Y_LIFT);
    
    layer->setVisible(false);

    // XNA nostalgia
    Application::get()->setClearColor(Color4f::CORNFLOWER);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void MainMenuScene::dispose() {
//    _button = nullptr;
//    _label  = nullptr;
    _assets = nullptr;
    surface = nullptr;
    depths = nullptr;
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
