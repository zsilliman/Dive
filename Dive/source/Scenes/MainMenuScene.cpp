//
//  MainMenuScene.cpp
//  Dive
//

#include "MainMenuScene.hpp"

using namespace cugl;

/** The ID for the button listener */
#define MAIN_TO_CREDITS 9
#define MAIN_TO_LEVELS 10
#define CREDITS_TO_MAIN 11
#define LEVELS_TO_MAIN 12
#define LEVEL_ONE 13
#define LEVEL_TWO 14
#define LEVEL_THREE 15
#define LEVEL_FOUR 16
#define LEVEL_FIVE 17
#define LEVEL_SIX 18
#define LEVEL_SEVEN 19
#define LEVEL_EIGHT 20
#define LEVEL_NINE 21
#define LEVEL_TEN 22
#define LEVEL_ELEVEN 23
#define LEVEL_TWELVE 24

#define MAIN_MENU 1
#define LEVEL_SELECT 2
#define CREDITS 3
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
    auto mainMenuLayer = assets->get<Node>("mainmenu");
    auto levelSelectLayer = assets->get<Node>("levelselect");
    auto creditsLayer = assets->get<Node>("credits");
    mainMenuLayer->setContentSize(dimen);
    levelSelectLayer->setContentSize(dimen);
    creditsLayer->setContentSize(dimen);
    addChild(mainMenuLayer);
    addChild(levelSelectLayer);
    addChild(creditsLayer);
//    layer->doLayout(); // This rearranges the children to fit the screen
    
    mainMenuLayer->setVisible(true); // 1
    levelSelectLayer->setVisible(false); // 2
    creditsLayer->setVisible(false); // 3
    _current = MAIN_MENU;
    
    surface = std::dynamic_pointer_cast<Button>(assets->get<Node>("mainmenu_surface"));
    depths = std::dynamic_pointer_cast<Button>(assets->get<Node>("mainmenu_depths"));
    downarr = std::dynamic_pointer_cast<Button>(assets->get<Node>("credits_downarr"));
    uparr = std::dynamic_pointer_cast<Button>(assets->get<Node>("levelselect_uparr"));
    level1 = std::dynamic_pointer_cast<Button>(assets->get<Node>("levelselect_level1"));
    level2 = std::dynamic_pointer_cast<Button>(assets->get<Node>("levelselect_level2"));
    level3 = std::dynamic_pointer_cast<Button>(assets->get<Node>("levelselect_level3"));
    level4 = std::dynamic_pointer_cast<Button>(assets->get<Node>("levelselect_level4"));
    level5 = std::dynamic_pointer_cast<Button>(assets->get<Node>("levelselect_level5"));
    level6 = std::dynamic_pointer_cast<Button>(assets->get<Node>("levelselect_level6"));
    level7 = std::dynamic_pointer_cast<Button>(assets->get<Node>("levelselect_level7"));
    level8 = std::dynamic_pointer_cast<Button>(assets->get<Node>("levelselect_level8"));
    level9 = std::dynamic_pointer_cast<Button>(assets->get<Node>("levelselect_level9"));
    level10 = std::dynamic_pointer_cast<Button>(assets->get<Node>("levelselect_level10"));
    level11 = std::dynamic_pointer_cast<Button>(assets->get<Node>("levelselect_level11"));
    level12 = std::dynamic_pointer_cast<Button>(assets->get<Node>("levelselect_level12"));
    
    surface->setListener([=](const std::string& name, bool down) {
        if (down && _current == MAIN_MENU) {
            CULog("go from main to credits");
            mainMenuLayer->setVisible(false);
            creditsLayer->setVisible(true);
            _current = CREDITS;
        }
    });
    
    depths->setListener([=](const std::string& name, bool down) {
        if (down && _current == MAIN_MENU) {
            CULog("go from main to level select");
            mainMenuLayer->setVisible(false);
            levelSelectLayer->setVisible(true);
            _current = LEVEL_SELECT;
        }
    });

    downarr->setListener([=](const std::string& name, bool down) {
        if (down && _current == CREDITS) {
            CULog("go from credits to main");
            creditsLayer->setVisible(false);
            mainMenuLayer->setVisible(true);
            _current = MAIN_MENU;
        }
    });
    
    uparr->setListener([=](const std::string& name, bool down) {
        if (down && _current == LEVEL_SELECT) {
            CULog("go from level select to main");
            levelSelectLayer->setVisible(false);
            mainMenuLayer->setVisible(true);
            _current = MAIN_MENU;
        }
    });
    
    level1->setListener([=](const std::string& name, bool down) {
        if (down && _current == LEVEL_SELECT) {
            CULog("level 1 selected");
            _level_selected = 1;
        }
    });
    level2->setListener([=](const std::string& name, bool down) {
        if (down && _current == LEVEL_SELECT) {
            CULog("level 2 selected");
            _level_selected = 2;
        }
    });
    level3->setListener([=](const std::string& name, bool down) {
        if (down && _current == LEVEL_SELECT) {
            CULog("level 3 selected");
            _level_selected = 3;
        }
    });
    level4->setListener([=](const std::string& name, bool down) {
        if (down && _current == LEVEL_SELECT) {
            CULog("level 4 selected");
            _level_selected = 4;
        }
    });
    level5->setListener([=](const std::string& name, bool down) {
        if (down && _current == LEVEL_SELECT) {
            CULog("level 5 selected");
            _level_selected = 5;
        }
    });
    level6->setListener([=](const std::string& name, bool down) {
        if (down && _current == LEVEL_SELECT) {
            CULog("level 6 selected");
            _level_selected = 6;
        }
    });
    level7->setListener([=](const std::string& name, bool down) {
        if (down && _current == LEVEL_SELECT) {
            CULog("level 7 selected");
            _level_selected = 7;
        }
    });
    level8->setListener([=](const std::string& name, bool down) {
        if (down && _current == LEVEL_SELECT) {
            CULog("level 8 selected");
            _level_selected = 8;
        }
    });
    level9->setListener([=](const std::string& name, bool down) {
        if (down && _current == LEVEL_SELECT) {
            CULog("level 9 selected");
            _level_selected = 9;
        }
    });
    level10->setListener([=](const std::string& name, bool down) {
        if (down && _current == LEVEL_SELECT) {
            CULog("level 10 selected");
            _level_selected = 10;
        }
    });
    level11->setListener([=](const std::string& name, bool down) {
        if (down && _current == LEVEL_SELECT) {
            CULog("level 11 selected");
            _level_selected = 10; //CHANGE THIS
        }
    });
    level12->setListener([=](const std::string& name, bool down) {
        if (down && _current == LEVEL_SELECT) {
            CULog("level 12 selected");
            _level_selected = 10; //CHANGE THIS
        }
    });
    
    if (_active) {
        surface->activate(MAIN_TO_CREDITS);
        depths->activate(MAIN_TO_LEVELS);
        downarr->activate(CREDITS_TO_MAIN);
        uparr->activate(LEVELS_TO_MAIN);
        level1->activate(LEVEL_ONE);
        level2->activate(LEVEL_TWO);
        level3->activate(LEVEL_THREE);
        level4->activate(LEVEL_FOUR);
        level5->activate(LEVEL_FIVE);
        level6->activate(LEVEL_SIX);
        level5->activate(LEVEL_SEVEN);
        level8->activate(LEVEL_EIGHT);
        level9->activate(LEVEL_NINE);
        level10->activate(LEVEL_TEN);
        level11->activate(LEVEL_ELEVEN);
        level12->activate(LEVEL_TWELVE);
    }
    
//    CULog("Button 1 position: (%f,%f)", level1->getPositionX(), level1->getPositionY());
//    CULog("Button 2 position: (%f,%f)", level2->getPositionX(), level2->getPositionY());
//    CULog("dimension width: %d", Application::get()->getDisplayWidth());
//    CULog("dimension height: %d", Application::get()->getDisplayHeight());
//    CULog("safe area: (%f,%f), (%f,%f)", Application::get()->getSafeArea().getMinX(), Application::get()->getSafeArea().getMinY(),
//         Application::get()->getSafeArea().getMaxX(), Application::get()->getSafeArea().getMaxY());
    
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
    surface->setPositionY(surface->getPositionY()+y_max*2/3);
    depths->setPositionY(depths->getPositionY()-y_max*2/3);
    
    downarr->setPositionY(y_max / 12);
    CULog("POSITION: %f", downarr->getPositionY());
    
    uparr->setPositionY(y_max + y_max + y_max/2);
    level1->setPosition(x_max, y_max*2/12*12);
    level2->setPosition(x_max, y_max*2/12*11);
    level3->setPosition(x_max, y_max*2/12*10);
    level4->setPosition(x_max, y_max*2/12*9);
    level5->setPosition(x_max, y_max*2/12*8);
    level6->setPosition(x_max, y_max*2/12*7);
    level7->setPosition(x_max, y_max*2/12*6);
    level8->setPosition(x_max, y_max*2/12*5);
    level9->setPosition(x_max, y_max*2/12*4);
    level10->setPosition(x_max, y_max*2/12*3);
    level11->setPosition(x_max, y_max*2/12*2);
    level12->setPosition(x_max, y_max*2/12*1);
    
    // XNA nostalgia
    Application::get()->setClearColor(Color4f::BLACK);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void MainMenuScene::dispose() {
    _assets = nullptr;
    surface = nullptr;
    depths = nullptr;
    downarr = nullptr;
    level1 = nullptr;
    level2 = nullptr;
    level3 = nullptr;
    level4 = nullptr;
    level5 = nullptr;
    level6 = nullptr;
    level7 = nullptr;
    level8 = nullptr;
    level9 = nullptr;
    level10 = nullptr;
    level11 = nullptr;
    level12 = nullptr;
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
    if (_level_selected>=1){
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

int MainMenuScene::getLevelSelected(){
    return _level_selected;
}

void MainMenuScene::setLevelSelected(int level){
    _level_selected = level;
}
