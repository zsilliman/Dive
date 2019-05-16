#include "InGameOverlay.h"

using namespace cugl;
using namespace std;

shared_ptr<InGameOverlay::Window> InGameOverlay::Window::alloc(shared_ptr<Texture> back_img, Rect safe_area, shared_ptr<Font> font, string title, string label1, string label2) {
	shared_ptr<InGameOverlay::Window> window = make_shared<InGameOverlay::Window>();
	window->init(back_img, safe_area, font, title, label1, label2);
	return window;
}

void InGameOverlay::Window::init(shared_ptr<Texture> back_img, Rect safe_area, shared_ptr<Font> font, string title, string label1, string label2) {
	_root = Node::alloc();
	_button_container = Node::alloc();
	//_root->setPosition(safe_area.getMidX(), safe_area.getMidY());

	_background = PolygonNode::allocWithTexture(back_img);
	_background->setAnchor(Vec2::ANCHOR_CENTER);
	_background->setPosition(safe_area.getMidX(), safe_area.getMidY());
	_background->setScale(0.7 * safe_area.size.width / back_img->getWidth());
	_root->addChild(_background);

	text_height = _background->getHeight() / 6;
	CULog("TEXT HEIGHT!");
	CULog(std::to_string(text_height).c_str());

	Color4 white = Color4(Vec3(1, 1, 1));
	_title = Label::alloc(title, font);
	_title->setHorizontalAlignment(Label::HAlign::CENTER);
	_title->setVerticalAlignment(Label::VAlign::MIDDLE);
	_title->setForeground(white);
	_title->setScale(text_height/font->getHeight() * _title_scale);
	Vec2 pos = Vec2(safe_area.getMidX(), safe_area.getMidY() + _background->getHeight() / 2 - text_height * 1.5);
	_title->setAnchor(Vec2::ANCHOR_CENTER);
	_title->setPosition(pos);

	_button_start_scale = text_height / font->getHeight();
	_button_end_scale = (text_height / font->getHeight()) * 1.3;

	//Label that corresponds to the first button (defaults to RESUME)
	shared_ptr<Label> _label1 = Label::alloc(label1, font);
	_label1->setHorizontalAlignment(Label::HAlign::CENTER);
	_label1->setVerticalAlignment(Label::VAlign::MIDDLE);
	_label1->setForeground(white);
	//Add label1 to button
	_button1 = Button::alloc(_label1);
	_button1->setScale(_button_start_scale);
	_button1->setAnchor(Vec2::ANCHOR_CENTER);
	pos = Vec2(_title->getPositionX(), _title->getPositionY() - text_height / 2 - text_height * 1);
	_button1->setPosition(pos);
	_button1->setName("b1");

	shared_ptr<Label> _label2 = Label::alloc(label2, font);
	_label2->setHorizontalAlignment(Label::HAlign::CENTER);
	_label2->setVerticalAlignment(Label::VAlign::MIDDLE);
	_label2->setForeground(white);

	_button2 = Button::alloc(_label2);
	_button2->setAnchor(Vec2::ANCHOR_CENTER);
	_button2->setScale(_button_start_scale);
	pos = Vec2(_title->getPositionX(), _button1->getPositionY() - text_height / 2 - text_height *0.8);
	_button2->setPosition(pos);
	_button2->setName("b2");

	_button_container->addChild(_title);
	_button_container->addChild(_button1);
	_button_container->addChild(_button2);
	_root->addChild(_button_container);
}

void InGameOverlay::Window::addButton(shared_ptr<Texture> back_img, Rect safe_area, shared_ptr<Font> font, string label) {
	Color4 white = Color4(Vec3(1, 1, 1));
	shared_ptr<Label> _label3 = Label::alloc(label, font);
	_label3->setHorizontalAlignment(Label::HAlign::CENTER);
	_label3->setVerticalAlignment(Label::VAlign::MIDDLE);
	_label3->setForeground(white);

	_title->setPositionY(_title->getPositionY() + text_height*.8);
	_button1->setPositionY(_button1->getPositionY() + text_height * .8);
	_button2->setPositionY(_button2->getPositionY() + text_height * .8);

	_button3 = Button::alloc(_label3);
	_button3->setAnchor(Vec2::ANCHOR_CENTER);
	Vec2 pos = Vec2(_title->getPositionX(), _button2->getPositionY() - text_height / 2 - text_height*0.8);
	_button3->setPosition(pos);
	_button3->setName("b3");
	_button3->setScale(_button_start_scale);
	_button_container->addChild(_button3);

	float button_margin = text_height*1;
	_background->setScale(_background->getScaleX(), button_margin/back_img->getWidth() + _background->getScaleY());
}

void InGameOverlay::Window::setActive(bool active) {
	_root->setVisible(active);
	if (!active && _button1->hasListener()) {
		_button1->removeListener();
	}
	if (!active && _button2->hasListener()) {
		_button2->removeListener();
	}
	if (!active && _button3 != nullptr && _button3->hasListener()) {
		_button3->removeListener();
	}
	if (active) {
		_button1->setListener(_callback1);
		_button2->setListener(_callback2);
		if (_button3 != nullptr)
			_button3->setListener(_callback3);
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

void InGameOverlay::Window::setCallback3(Button::Listener callback3) {
	_callback3 = callback3;
	_button3->setListener(callback3);
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
	if (_button3 == nullptr) {
		return;
	}
	if (_button3->isDown()) {
		_button3->setScale(_button_end_scale);
	}
	else {
		_button3->setScale(_button_start_scale);
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
	_pause_node->addButton(window, safe_area, font, "RESTART");
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

	_pause_node->activate(2,3);
	_pause_node->activate3(100);
	_win_node->activate(4,5);
	_lose_node->activate(6,7);
	_pause_button->activate(8);
}

void InGameOverlay::setMainMenuCallback(Button::Listener menu_callback) {
	_pause_node->setCallback1(menu_callback);
	_win_node->setCallback1(menu_callback);
	_lose_node->setCallback1(menu_callback);
}

void InGameOverlay::setRetryCallback(Button::Listener retry_callback) {
	_lose_node->setCallback2(retry_callback);
	_pause_node->setCallback3(retry_callback);
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



