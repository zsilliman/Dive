#include "InGameOverlay.h"

using namespace cugl;
using namespace std;

shared_ptr<InGameOverlay::Window> InGameOverlay::Window::alloc(shared_ptr<Texture> back_img, Rect safe_area, shared_ptr<Font> font, string title, string label1, string label2) {
	shared_ptr<InGameOverlay::Window> window = make_shared<InGameOverlay::Window>();
	window->init(back_img, safe_area, font, title, label1, label2);
	return window;
}

void InGameOverlay::Window::init(shared_ptr<Texture> back_img, Rect safe_area, shared_ptr<Font> font, string title, string label1, string label2) {
	_root = PolygonNode::allocWithTexture(back_img);
	_root->setScale(0.6 * safe_area.size.width / back_img->getWidth());
	_root->setAnchor(Vec2::ANCHOR_CENTER);
	_root->setPosition(safe_area.getMidX(), safe_area.getMidY());

	Color4 white = Color4(Vec3(1, 1, 1));
	_title = Label::alloc(title, font);
	_title->setHorizontalAlignment(Label::HAlign::CENTER);
	_title->setVerticalAlignment(Label::VAlign::MIDDLE);
	_title->setForeground(white);
	_title->setScale(_title_scale);
	Vec2 pos = Vec2(_root->getWidth() / (2 * _root->getScaleX()), _root->getHeight() / _root->getScaleY() - _title->getHeight()*0.8);
	_title->setAnchor(Vec2::ANCHOR_CENTER);
	_title->setPosition(pos);

	//Label that corresponds to the first button (defaults to RESUME)
	shared_ptr<Label> _label1 = Label::alloc(label1, font);
	_label1->setHorizontalAlignment(Label::HAlign::CENTER);
	_label1->setVerticalAlignment(Label::VAlign::MIDDLE);
	_label1->setForeground(white);
	//Add label1 to button
	_button1 = Button::alloc(_label1);
	_button1->setScale(_button_start_scale);
	_button1->setAnchor(Vec2::ANCHOR_CENTER);
	pos = Vec2(_title->getPositionX(), _title->getPositionY() - _title->getHeight() / 2 - _label1->getHeight() * 1);
	_button1->setPosition(pos);
	_button1->setName("b2");

	shared_ptr<Label> _label2 = Label::alloc(label2, font);
	_label2->setHorizontalAlignment(Label::HAlign::CENTER);
	_label2->setVerticalAlignment(Label::VAlign::MIDDLE);
	_label2->setForeground(white);

	_button2 = Button::alloc(_label2);
	_button2->setAnchor(Vec2::ANCHOR_CENTER);
	_button2->setScale(_button_start_scale);
	pos = Vec2(_title->getPositionX(), _button1->getPositionY() - _button1->getHeight() / 2 - _label2->getHeight()*0.8);
	_button2->setPosition(pos);
	_button2->setName("b2");

	_root->addChild(_title);
	_root->addChild(_button1);
	_root->addChild(_button2);
}

void InGameOverlay::Window::setActive(bool active) {
	_root->setVisible(active);
	if (!active && _button1->hasListener()) {
		_button1->removeListener();
	}
	if (!active && _button2->hasListener()) {
		_button2->removeListener();
	}
	if (active) {
		_button1->setListener(_callback1);
		_button2->setListener(_callback2);
	}
}

void InGameOverlay::Window::setCallback1(Button::Listener callback1) {
	_callback1 = callback1;
	_button1->setListener(callback1);
}

void InGameOverlay::Window::setCallback2(Button::Listener callback2) {
	_callback2 = callback2;
	_button2->setListener(callback2);
}

void InGameOverlay::Window::update() {
	if (_button1->isDown()) {
		_button1->setScale(_button_end_scale);
	}
	else {
		_button1->setScale(_button_start_scale);
	}
	if (_button2->isDown()) {
		_button2->setScale(_button_end_scale);
	}
	else {
		_button2->setScale(_button_start_scale);
	}
}

shared_ptr<InGameOverlay> InGameOverlay::alloc(const std::shared_ptr<cugl::AssetManager>& assets, Rect safe_area) {
	shared_ptr<InGameOverlay> overlay = make_shared<InGameOverlay>();
	overlay->init(assets, safe_area);
	return overlay;
}

void InGameOverlay::init(const std::shared_ptr<cugl::AssetManager>& assets, Rect safe_area) {
	_root_node = Node::alloc();

	//Get view assets
	shared_ptr<Texture> window = assets->get<Texture>("UI_window");
	shared_ptr<Texture> pause_up = assets->get<Texture>("UI_pause_up");
	shared_ptr<Texture> pause_down = assets->get<Texture>("UI_pause_down");
	shared_ptr<Font> font = assets->get<Font>("OldKlarheit");

	Color4 white = Color4(Vec3(1, 1, 1));

	_pause_node = InGameOverlay::Window::alloc(window, safe_area, font, "PAUSED", "MAIN MENU", "RESUME");
	_win_node = InGameOverlay::Window::alloc(window, safe_area, font, "VICTORY", "MAIN MENU", "CONTINUE");
	_lose_node = InGameOverlay::Window::alloc(window, safe_area, font, "YOU LOST", "MAIN MENU", "TRY AGAIN");
	
	shared_ptr<PolygonNode> pause_up_node = PolygonNode::allocWithTexture(pause_up);
	shared_ptr<PolygonNode> pause_down_node = PolygonNode::allocWithTexture(pause_down);
	_pause_button = Button::alloc(pause_up_node, pause_down_node);
	_pause_button->setScale(safe_area.size.width * 0.10 / pause_down->getWidth());
	_pause_button->setAnchor(Vec2::ANCHOR_TOP_LEFT);
	float padding = safe_area.size.width * 0.03;
	_pause_button->setPosition(safe_area.getMinX() + padding, safe_area.getMaxY() - padding);

	//Add UI window to root
	_root_node->addChild(_pause_node->getRoot());
	_root_node->addChild(_win_node->getRoot());
	_root_node->addChild(_lose_node->getRoot());

	_ingame_node = Node::alloc();
	_ingame_node->addChild(_pause_button);
	_root_node->addChild(_ingame_node);

	_pause_node->activate(1,2);
	_win_node->activate(3,4);
	_lose_node->activate(5,6);
	_pause_button->activate(7);
}

void InGameOverlay::setMainMenuCallback(Button::Listener menu_callback) {
	_pause_node->setCallback1(menu_callback);
	_win_node->setCallback1(menu_callback);
	_lose_node->setCallback1(menu_callback);
}

void InGameOverlay::setRetryCallback(Button::Listener retry_callback) {
	_lose_node->setCallback2(retry_callback);
}

void InGameOverlay::setResumeCallback(Button::Listener resume_callback) {
	_pause_node->setCallback2(resume_callback);
}

void InGameOverlay::setContinueCallback(Button::Listener continue_callback) {
	_win_node->setCallback2(continue_callback);
}

void InGameOverlay::setPauseCallback(Button::Listener pause_callback) {
	_pause_callback = pause_callback;
	_pause_button->setListener(pause_callback);
}

void InGameOverlay::dispose() {
	_ingame_node = nullptr;
	_pause_node = nullptr;
	_lose_node = nullptr;
	_win_node = nullptr;
}

void InGameOverlay::setPauseActive(bool active) {
	_ingame_node->setVisible(active);
	if (active) {
		_pause_button->setListener(_pause_callback);
	}
	else if (!active && _pause_button->hasListener()) {
		_pause_button->removeListener();
	}
}

void InGameOverlay::setState(State state) {
	_pause_node->setActive(false);
	_win_node->setActive(false);
	_lose_node->setActive(false);
	setPauseActive(false);
	_current_state = state;

	if (state == PLAY) {
		setPauseActive(true);
	}
	else if (state == LOSE) {
		_lose_node->setActive(true);
	}
	else if (state == WIN) {
		_win_node->setActive(true);
	}
	else if (state == PAUSE) {
		_pause_node->setActive(true);
	}
}

void InGameOverlay::update(float timestep) {
	_pause_node->update();
	_win_node->update();
	_lose_node->update();
}

void InGameOverlay::reset() {
	setState(PLAY);
}



