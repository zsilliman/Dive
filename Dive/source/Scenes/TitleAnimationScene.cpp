//
//  TitleAnimationScene.cpp
//  Dive
//
//  Created by Pippen Wu on 2019/3/27.
//  2019 Game Design Initiative at Cornell.
//

#include "TitleAnimationScene.hpp"

using namespace cugl;

/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 640

#pragma mark -
#pragma mark Constructors

/**
 * Initializes the controller contents, making it ready for loading
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * @param assets    The (loaded) assets for this animation
 *
 * @return true if the controller is initialized properly, false otherwise.
 */
bool TitleAnimationScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(dimen)) {
        return false;
    }
    
    _assets = assets;
	shared_ptr<Texture> texture = _assets->get<Texture>("title");
    _titlenode  = std::dynamic_pointer_cast<AnimationNode>(_assets->get<Node>("dive"));
	_titlenode->setPosition(dimen/2);
	_titlenode->setAnchor(Vec2::ANCHOR_CENTER);
	_titlenode->setScale(1);
	addChild(_titlenode);
    //_titlenode->setContentSize(dimen);
    
    CULog("X: %f, Y: %f", _titlenode->getPositionX(), _titlenode->getPositionY());
    
    _active = true;
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void TitleAnimationScene::dispose() {
    if (_active) {
        _titlenode = nullptr;
        _active = false;
    }
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
void TitleAnimationScene::update(float progress) {
//    if (_titlenode != NULL){
//        CULog("frame # %d", _titlenode->getFrame());
//    }else{
//        CULog("null");
//    }
    
    if (!_finished){
        if (_titlenode->getFrame()>=117){
            _cooldown = 3;
            _linger--;
            if (_linger < 0){
                _finished = true;
            }
        }
        if (_cooldown <= 0){
            _titlenode->setFrame(_titlenode->getFrame()+1);
            _cooldown = 1;
        }else{
            _cooldown--;
        }
    }
    
    this->_active = !_finished;
}
