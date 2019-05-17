#pragma once

#include <cugl/cugl.h>
#include "../Util.h"
#include "../ViewControllers/InputController.h"

using namespace cugl;
using namespace std;

class InGameOverlay {
protected:

	class Window {
	protected:
		shared_ptr<Node> _root, _background, _button_container;
		shared_ptr<Label> _title;
		shared_ptr<Button> _button1, _button2, _button3 = nullptr;
		float _button_start_scale = 1;
		float _button_end_scale = 1;
		float _title_scale = 1.6;
		float text_height;
		Button::Listener _callback1, _callback2, _callback3;
		
	public:
		static shared_ptr<Window> alloc(shared_ptr<Texture> back_img, Rect safe_area, shared_ptr<Font> font, string title, string label1, string label2);
		void addButton(shared_ptr<Texture> back_img, Rect safe_area, shared_ptr<Font> font, string label);
		void init(shared_ptr<Texture> back_img, Rect safe_area, shared_ptr<Font> font, string title, string label1, string label2);
		shared_ptr<Node> getRoot() { return _root; }
		void activate(int listener1, int listener2) { _button1->activate(listener1); _button2->activate(listener2); }
		void activate3(int listener3) { _button3->activate(listener3); }
		void setActive(bool active);
		void update();
		void setCallback1(Button::Listener callback1);
		void setCallback2(Button::Listener callback2);
		void setCallback3(Button::Listener callback2);
	};

	shared_ptr<Node> _ingame_node, tutorial_node;
	shared_ptr<Window> _pause_node, _lose_node, _win_node;
	shared_ptr<Button> _pause_button;
	shared_ptr<InputController> _input;
	Button::Listener _pause_callback, _tutorial_callback;
	bool _tutorial_active = false, pressed_prev = false;
    
    std::shared_ptr<cugl::Sound> bubble_sound;

	State _current_state;
	
	void buildTutorialNode(const std::shared_ptr<cugl::AssetManager>& assets, Rect safe_area, Size display);

	void setPauseActive(bool active);

public:
	shared_ptr<Node> _root_node;

	InGameOverlay() {}
	~InGameOverlay() { dispose(); }

	static shared_ptr<InGameOverlay> alloc(const std::shared_ptr<cugl::AssetManager>& assets, shared_ptr<InputController> _input, Rect safe_area, Size display);

	void init(const std::shared_ptr<cugl::AssetManager>& assets, shared_ptr<InputController> _input, Rect safe_area, Size display);

	void setMainMenuCallback(Button::Listener menu_callback);
	void setRetryCallback(Button::Listener retry_callback);
	void setResumeCallback(Button::Listener resume_callback);
	void setContinueCallback(Button::Listener continue_callback);
	void setPauseCallback(Button::Listener pause_callback);
	void setTutorialCallback(Button::Listener tutorial_callback);

	State getState() { return _current_state; }

	void dispose();

	void setState(State state);

	void update(float timestep);

	void reset();

};
