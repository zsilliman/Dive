//
//  InputController.h
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

#ifndef InputController_h
#define InputController_h

#include <cugl/cugl.h>
#include <unordered_set>

/**
 * This class represents player input in the game.
 *
 * This input handler uses the CUGL input API.  It uses the polling API for
 * keyboard, but the callback API for touch.  This demonstrates a mix of ways
 * to handle input, and the reason for hiding it behind an abstraction like
 * this class.
 *
 * Unlike CUGL input devices, this class is not a singleton.  It must be
 * allocated before use.  However, you will notice that we do not do any
 * input initialization in the constructor.  This allows us to allocate this
 * controller as a field without using pointers. We simply add the class to the
 * header file of its owner, and delay initialization (via the method init())
 * until later. This is one of the main reasons we like to avoid initialization
 * in the constructor.
 */

class InputController{
    
private:
    /** Whether or not this input is active */
    bool _active;
    // KEYBOARD EMULATION
    /** Whether the reset key is down */
    bool  _keyReset;
    /** Whether the exit key is down */
    bool  _keyExit;
    /** Whether the left arrow key is down */
    bool  _keyLeft;
    /** Whether the right arrow key is down */
    bool  _keyRight;

protected:
    // INPUT RESULTS
    /** Whether the left action was chosen. */
    bool _leftPressed;
    /** Whether the righteset action was chosen. */
    bool _rightPressed;
    /** Whether the reset action was chosen. */
    bool _resetPressed;
    /** Whether the exit action was chosen. */
    bool _exitPressed;
    /** How much did we move horizontally? */
    float _horizontal;
    
#pragma mark Internal Touch Management
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
    //
    // The meaning of any touch depends on the zone it begins in.

    /** Information representing a single "touch" (possibly multi-finger) */
    struct TouchInstance {
        /** The current touch position */
        cugl::Vec2 position;
        /** The touch id(s) for future reference */
        std::unordered_set<Uint64> touchids;
    };

    /** Enumeration identifying a zone for the current touch */
    enum class Zone {
        /** The touch was not inside the screen bounds */
        UNDEFINED,
        /** The touch was in the left zone (as shown above) */
        LEFT,
        /** The touch was in the right zone (as shown above) */
        RIGHT
    };
    
    /** The bounds of the entire game screen */
    cugl::Rect _bounds;
    /** The bounds of the left touch zone */
    cugl::Rect _lzone;
    /** The bounds of the right touch zone */
    cugl::Rect _rzone;
    
    // Each zone can have only one touch
    /** The current touch location for the left zone */
    TouchInstance _ltouch;
    /** The current touch location for the right zone */
    TouchInstance _rtouch;
    
    /** The timestamp for the beginning of the current swipe gesture */
    cugl::Timestamp _swipetime;
    /** The timestamp for a double tap (main zone only) */
    cugl::Timestamp _dbtaptime;
    
    /**
     * Defines the zone boundaries, so we can quickly categorize touches.
     */
    void createZones();
    
    /**
     * Populates the initial values of the TouchInstances
     */
    void clearTouchInstance(TouchInstance& touchInstance);
    
    /**
     * Returns the correct zone for the given position.
     *
     * See the comments above for a description of how zones work.
     *
     * @param  pos  a position in screen coordinates
     *
     * @return the correct zone for the given position.
     */
    Zone getZone(const cugl::Vec2& pos);
    
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
    int  checkSwipe(const cugl::Vec2& start, const cugl::Vec2& stop, cugl::Timestamp current);
    
public:
#pragma mark -
#pragma mark Constructors
    
    /**
     * Creates a new input controller.
     *
     * This constructor does NOT do any initialzation.  It simply allocates the
     * object. This makes it safe to use this class without a pointer.
     */
    InputController(); // Don't initialize.  Allow stack based
    
    /**
     * Disposes of this input controller, releasing all listeners.
     */
    ~InputController() { dispose(); }
    
    /**
     * Deactivates this input controller, releasing all listeners.
     *
     * This method will not dispose of the input controller. It can be reused
     * once it is reinitialized.
     */
    void dispose();
    
    /**
     * This method works like a proper constructor, initializing the input
     * controller and allocating memory.  However, it still does not activate
     * the listeners.  You must call start() do that.
     */
    bool init();
    
#pragma mark -
#pragma mark Input Detection
    /**
     * Returns true if the input handler is currently active
     *
     * @return true if the input handler is currently active
     */
    bool isActive( ) const { return _active; }
    
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
    void  update(float dt);
    
    /**
     * Clears any buffered inputs so that we may start fresh.
     */
    void clear();
    
#pragma mark -
#pragma mark Input Results
    /**
     * Returns the amount of sideways movement.
     *
     * -1 = left, 1 = right, 0 = still
     *
     * @return the amount of sideways movement.
     */
    float getHorizontal() const { return _horizontal; }
    
    // Returns true if the left key (left screen) is pressed
    bool didPressLeft() {return _leftPressed;}
    
    // Returns true if the right key (right screen) is pressed
    bool didPressRight() {return _rightPressed;}
    
    bool goingLeft() {return _horizontal>0;}
    bool goingRight() {return _horizontal<0;}

    /**
     * Returns true if the reset button was pressed.
     *
     * @return true if the reset button was pressed.
     */
    bool didReset() const { return _resetPressed; }
    
    /**
     * Returns true if the exit button was pressed.
     *
     * @return true if the exit button was pressed.
     */
    bool didExit() const { return _exitPressed; }
    
#pragma mark -
#pragma mark Touch and Mouse Callbacks
    /**
     * Callback for the beginning of a touch event
     *
     * @param event The associated event
     * @param focus    Whether the listener currently has focus
     *
     */
    void touchBeganCB(const cugl::TouchEvent& event, bool focus);
    
    /**
     * Callback for the end of a touch event
     *
     * @param event The associated event
     * @param focus    Whether the listener currently has focus
     */
    void touchEndedCB(const cugl::TouchEvent& event, bool focus);
    
    /**
     * Callback for a mouse release event.
     *
     * @param event The associated event
     * @param previous The previous position of the touch
     * @param focus    Whether the listener currently has focus
     */
    void touchesMovedCB(const cugl::TouchEvent& event, const cugl::Vec2& previous, bool focus);
    
};
#endif /* InputController_hpp */
