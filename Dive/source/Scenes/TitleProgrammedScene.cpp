//
//  TitleAnimationScene.cpp
//  Dive
//
//  Created by Pippen Wu on 2019/3/27.
//  2019 Game Design Initiative at Cornell.
//

#include "TitleProgrammedScene.h"
#include "../Util.h"

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
bool TitleProgrammedScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
	// Initialize the scene to a locked width
	_dimen = Application::get()->getDisplaySize();
	_dimen *= SCENE_WIDTH / _dimen.width; // Lock the game to a reasonable resolution
	if (assets == nullptr) {
		return false;
	}
	else if (!Scene::init(_dimen)) {
		return false;
	}

	_assets = assets;
	shared_ptr<Texture> texture = _assets->get<Texture>("background");
	shared_ptr<Texture> texture_clear = _assets->get<Texture>("background_clear");
	shared_ptr<Font> font = _assets->get<Font>("charlemagne");
	_root = Node::allocWithPosition(Vec2(0,0));

	float scale = _dimen.width/texture->getWidth();
	_background1 = PolygonNode::allocWithTexture(texture);
	_background1->setAnchor(Vec2::ANCHOR_TOP_LEFT);
	_background1->setScale(scale);
	_background1->setZOrder(0);
	_root->addChild(_background1);

	_background2 = PolygonNode::allocWithTexture(texture_clear);
	_background2->setAnchor(Vec2::ANCHOR_TOP_LEFT);
	_background2->setScale(scale);
	_background2->setZOrder(1);
	_root->addChild(_background2);
	

	_background3 = PolygonNode::allocWithTexture(texture_clear);
	_background3->setAnchor(Vec2::ANCHOR_TOP_LEFT);
	_background3->setScale(scale);
	_background3->setZOrder(2);
	_root->addChild(_background3);

	_dive = Node::allocWithPosition(Vec2());
	shared_ptr<FloatLayout> f_layout = FloatLayout::alloc();
	f_layout->setHorizontal(true);
	_dive->setLayout(f_layout);

	float padding = 10;
	_D_node = Label::alloc("D", font);
	_D_node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	_D_node->setPositionX(0);
	_dive->addChild(_D_node);

	_I_node = Label::alloc("I", font);
	_I_node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	_I_node->setPositionX(_D_node->getPositionX() + _D_node->getWidth() + padding);
	_dive->addChild(_I_node);
	
	_V_node = Label::alloc("V", font);
	_V_node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	_V_node->setPositionX(_I_node->getPositionX() + _I_node->getWidth() + padding);
	_dive->addChild(_V_node);
	
	_E_node = Label::alloc("E", font);
	_E_node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	_E_node->setPositionX(_V_node->getPositionX() + _V_node->getWidth() + padding);
	_dive->addChild(_E_node);
	_dive->setZOrder(5);

	_dive_width = _D_node->getWidth() + _I_node->getWidth() + _V_node->getWidth() + _E_node->getWidth() + padding * 3;
	_dive_height = _D_node->getHeight();

	_root->addChild(_dive);
	addChild(_root);

	_t = 0;
	_active = true;
	return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void TitleProgrammedScene::dispose() {
	if (_active) {
		_root = nullptr;
		_background1 = nullptr;
		_background2 = nullptr;
		_background3 = nullptr;

		_D_node = nullptr;
		_I_node = nullptr;
		_V_node = nullptr;
		_E_node = nullptr;
		_active = false;
	}
}

void TitleProgrammedScene::dive_func(float t) {
	float f_t1 = (2*atan(5*(t-0.5)))/(0.758*PI);	
	float f_t = sin(4 * PI*f_t1);
	//float f_t = sqrtf(t)*sinf(20 * PI * (pow(-2*t - 1, 2) + 1));
	float angle = 0.25f * PI * f_t * t;
	_D_node->setAngle(angle);
	_I_node->setAngle(angle);
	_V_node->setAngle(angle);
	_E_node->setAngle(angle);

	Vec2 start = Vec2(_dimen.width*1.2, _dimen.height-100);
	Vec2 end = Vec2(_dimen.width/2 - _dive_width / 2, _dimen.height/2 - _dive_height / 2);
	Vec2 pos = start + (end - start) * sqrtf(t);
	pos.x += f_t * 50;
	_dive->setPosition(pos);
}

void TitleProgrammedScene::parallax_func1(float t) {
	Vec2 start = Vec2(0, _dimen.height);
	Vec2 end = Vec2(0, _background1->getHeight());
	Vec2 pos = start + (end - start) * EasingFunction::quintOut(t);
	_background1->setPosition(pos);
}

void TitleProgrammedScene::parallax_func2(float t) {
	Vec2 start = Vec2(0, -_background1->getHeight());
	Vec2 end = Vec2(0, _background1->getHeight()*0.9);
	Vec2 pos = start + (end - start)*EasingFunction::quintOut(t);
	_background2->setPosition(pos);
}

void TitleProgrammedScene::parallax_func3(float t) {
	Vec2 start = Vec2(0, -2 * _background1->getHeight());
	Vec2 end = Vec2(0, _background1->getHeight()*0.8);
	Vec2 pos = start + (end - start)*EasingFunction::quintOut(t);
	_background3->setPosition(pos);
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
void TitleProgrammedScene::update(float progress) {
	float t_step = progress/4;
	if (!_finished) {
		if (!_stall) {
			_t += t_step;
			if (_t > 1)
				_t = 1;
			parallax_func1(_t);
			parallax_func2(_t);
			parallax_func3(_t);
			dive_func(_t);
			if (_t >= 1) {
				_stall = true;
				_t = 0;
			}
		}
		else {
			_t += t_step;
			if (_t > 1) {
				_t = 1;
				_finished = true;
			}
		}
	}

	this->_active = !_finished;
}
