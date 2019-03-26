//
//  InputController.cpp
//  Dive
//
//  This input controller is primarily designed for keyboard control.  On mobile
//  you will notice that we use gestures to emulate keyboard commands. They even
//  use the same variables (though we need other variables for internal keyboard
//  emulation).  This simplifies our design quite a bit.
//
//  This file is based on the PFInput class in the PlatformDemo.
//
//  Created by Pippen Wu on 2019/3/15.
//

#include "InputController.hpp"

using namespace cugl;

#pragma mark Input Constants

/** The key to use for reseting the game */
#define RESET_KEY KeyCode::R
/** The key for exitting the game */
#define EXIT_KEY  KeyCode::ESCAPE
/** How far we must swipe left or right for a gesture */
#define EVENT_SWIPE_LENGTH  200
/** How fast we must swipe left or right for a gesture */
#define EVENT_SWIPE_TIME   1000
/** The key for the event handlers */
#define LISTENER_KEY        1

// The screen is divided into two zones: Left and Right.
// These are shown in the diagram below.
//
//   |---------|
//   |    |    |
//   |    |    |
//   | L  |  R |
//   |    |    |
//   |    |    |
//   |---------|

/** The portion of the screen used for the left zone */
#define LEFT_ZONE       0.5f
/** The portion of the screen used for the right zone */
#define RIGHT_ZONE      0.5f

#pragma mark -
#pragma mark Input Controller
/**
 * Creates a new input controller.
 *
 * This constructor does NOT do any initialzation.  It simply allocates the
 * object. This makes it safe to use this class without a pointer.
 */
InputController::InputController() :
_active(false),
_keyReset(false),
_keyExit(false),
_keyLeft(false),
_keyRight(false),
_leftPressed(false),
_rightPressed(false),
_resetPressed(false),
_exitPressed(false),
_horizontal(0.0f){
}

/**
 * Deactivates this input controller, releasing all listeners.
 *
 * This method will not dispose of the input controller. It can be reused
 * once it is reinitialized.
 */
void InputController::dispose() {
    if (_active) {
#ifndef CU_TOUCH_SCREEN
        Input::deactivate<Keyboard>();
#else
        Touchscreen* touch = Input::get<Touchscreen>();
        touch->removeBeginListener(LISTENER_KEY);
        touch->removeEndListener(LISTENER_KEY);
#endif
        _active = false;
    }
}

/**
 * Initializes the input control for the given drawing scale.
 *
 * This method works like a proper constructor, initializing the input
 * controller and allocating memory.  However, it still does not activate
 * the listeners.  You must call start() do that.
 *
 * @return true if the controller was initialized successfully
 */
bool InputController::init() {
    bool success = true;
    
    _bounds = Application::get()->getDisplayBounds();
    
    createZones();
    clearTouchInstance(_ltouch);
    clearTouchInstance(_rtouch);

#ifndef CU_TOUCH_SCREEN
    success = Input::activate<Keyboard>();
#else
    
    Touchscreen* touch = Input::get<Touchscreen>();
    touch->addBeginListener(LISTENER_KEY,[=](const cugl::TouchEvent& event, bool focus) {
        this->touchBeganCB(event,focus);
    });
    touch->addEndListener(LISTENER_KEY,[=](const cugl::TouchEvent& event, bool focus) {
        this->touchEndedCB(event,focus);
    });
    touch->addMotionListener(LISTENER_KEY,[=](const cugl::TouchEvent& event, const cugl::Vec2& previous, bool focus) {
        this->touchesMovedCB(event, previous, focus);
    });
    
#endif
    _active = success;
    return success;
}

/**
 * Processes the currently cached inputs.
 *
 * This method is used to to poll the current input state.  This will poll the
 * keyboad and accelerometer.
 *
 * This method also gathers the delta difference in the touches. Depending on
 * the OS, we may see multiple updates of the same touch in a single animation
 * frame, so we need to accumulate all of the data together.
 */
void InputController::update(float dt) {
#ifndef CU_TOUCH_SCREEN
    // DESKTOP CONTROLS
    Keyboard* keys = Input::get<Keyboard>();
    
    // Map "keyboard" events to the current frame boundary
    _keyReset  = keys->keyPressed(RESET_KEY);
    _keyExit   = keys->keyPressed(EXIT_KEY);
    _keyLeft = keys->keyDown(KeyCode::ARROW_LEFT);
    _keyRight = keys->keyDown(KeyCode::ARROW_RIGHT);
#endif
    
    _leftPressed = _keyLeft;
    _rightPressed = _keyRight;
    _resetPressed = _keyReset;
    _exitPressed  = _keyExit;
    
    // Directional controls
    _horizontal = 0.0f;
    if (_keyRight) {
        _horizontal += 1.0f;
    }
    if (_keyLeft) {
        _horizontal -= 1.0f;
    }
    
    // If it does not support keyboard, we must reset "virtual" keyboard
#ifdef CU_TOUCH_SCREEN
    _keyExit = false;
    _keyReset = false;
    _keyLeft = false;
    _keyRight = false;
#endif
}

/**
 * Clears any buffered inputs so that we may start fresh.
 */
void InputController::clear() {
    _resetPressed = false;
    _exitPressed  = false;
    _leftPressed = false;
    _rightPressed = false;
}

/**
 * Defines the zone boundaries, so we can quickly categorize touches.
 */
void InputController::createZones() {
    _lzone = _bounds;
    _lzone.size.width *= LEFT_ZONE;
    _rzone = _bounds;
    _rzone.size.width *= RIGHT_ZONE;
    _rzone.origin.x = _bounds.origin.x+_bounds.size.width-_rzone.size.width;
}

/**
 * Populates the initial values of the input TouchInstance
 */
void InputController::clearTouchInstance(TouchInstance& touchInstance) {
    touchInstance.touchids.clear();
    touchInstance.position = Vec2::ZERO;
}

/**
 * Returns the correct zone for the given position.
 *
 * See the comments above for a description of how zones work.
 *
 * @param  pos  a position in screen coordinates
 *
 * @return the correct zone for the given position.
 */
InputController::Zone InputController::getZone(const cugl::Vec2& pos) {
    if (_lzone.contains(pos)) {
        return Zone::LEFT;
    } else if (_rzone.contains(pos)) {
        return Zone::RIGHT;
    }
    return Zone::UNDEFINED;
}

/**
 * Returns a nonzero value if this is a quick left or right swipe
 *
 * The function returns -1 if it is left swipe and 1 if it is a right swipe.
 *
 * @param  start    the start position of the candidate swipe
 * @param  stop     the end position of the candidate swipe
 * @param  current  the current timestamp of the gesture
 *
 * @return a nonzero value if this is a quick left or right swipe
 */
int InputController::checkSwipe(const cugl::Vec2& start, const cugl::Vec2& stop, cugl::Timestamp current) {
    // Look for swipes up that are "long enough"
    float xdiff = (stop.x-start.x);
    if (current.ellapsedMillis(_swipetime) < EVENT_SWIPE_TIME) {
        float thresh = EVENT_SWIPE_LENGTH;
        if (xdiff > thresh) {
            return 1;
        } else if (xdiff < thresh * -1) {
            return -1;
        }
    }
    return 0;
}

#pragma mark -
#pragma mark Touch and Mouse Callbacks
/**
 * Callback for the beginning of a touch event
 *
 * @param event The associated event
 * @param focus    Whether the listener currently has focus
 */
void InputController::touchBeganCB(const cugl::TouchEvent& event, bool focus) {
    //CULog("Touch began %lld", event.touch);
    Vec2 pos = event.position;
    //transform so bottom of screen is y = 0
    pos.y = _bounds.getMaxY() - pos.y;
    Zone zone = getZone(pos);
    switch (zone) {
        case Zone::LEFT:
            //CULog("Zone left");
            // Only process if no touch in zone
            if (_ltouch.touchids.empty()) {
                _ltouch.position = event.position;
                _ltouch.touchids.insert(event.touch);
                _keyLeft = true;
                CULog("left is pressed");
            }
            break;
        case Zone::RIGHT:
            //CULog("Zone right");
            // Only process if no touch in zone
            if (_rtouch.touchids.empty()) {
                _rtouch.position = event.position;
                _rtouch.touchids.insert(event.touch);
                _keyRight = true;
                CULog("right is pressed");
            }
            break;
        default:
            CUAssertLog(false, "Touch is out of bounds");
            break;
    }
    _swipetime = event.timestamp;
}

/**
 * Callback for the end of a touch event
 *
 * @param event The associated event
 * @param focus    Whether the listener currently has focus
 */
void InputController::touchEndedCB(const cugl::TouchEvent& event, bool focus) {
    // Reset all keys that might have been set
    //CULog("Touch is up %lld", event.touch);
    Vec2 pos = event.position;
    //transform so bottom of screen is y = 0
    pos.y = _bounds.getMaxY() - pos.y;
    if (_ltouch.touchids.find(event.touch) != _ltouch.touchids.end()) {
        _ltouch.touchids.clear();
        _keyLeft = false;
    } else if (_rtouch.touchids.find(event.touch) != _rtouch.touchids.end()) {
        _rtouch.touchids.clear();
        _keyRight = false;
    }
    _dbtaptime = event.timestamp;
}

/**
 * Callback for a touch moved event.
 *
 * @param event The associated event
 * @param previous The previous position of the touch
 * @param focus    Whether the listener currently has focus
 */
void InputController::touchesMovedCB(const cugl::TouchEvent& event, const Vec2& previous, bool focus) {
//    Vec2 pos = event.position;
//    //transform so bottom of screen is y = 0
//    pos.y = _bounds.getMaxY() - pos.y;
//    Zone zone = getZone(pos);
}
