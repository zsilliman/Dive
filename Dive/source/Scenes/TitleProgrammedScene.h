#pragma once

#include <cugl/cugl.h>

/**
 * This class is plays the title animation once.
 */
class TitleProgrammedScene : public cugl::Scene {
protected:
	/** The asset manager for loading. */
	std::shared_ptr<cugl::AssetManager> _assets;

	std::shared_ptr<cugl::Node> _root;
	std::shared_ptr<cugl::Node> _dive;

	std::shared_ptr<cugl::PolygonNode> _background1;
	std::shared_ptr<cugl::PolygonNode> _background2;
	std::shared_ptr<cugl::PolygonNode> _background3;

	std::shared_ptr<cugl::Label> _D_node;
	std::shared_ptr<cugl::Label> _I_node;
	std::shared_ptr<cugl::Label> _V_node;
	std::shared_ptr<cugl::Label> _E_node;

	cugl::Size _dimen;
	float _t = 0, _dive_width, _dive_height;

	bool _stall = false;
	bool _finished = false;

	void dive_func(float t);

	void parallax_func1(float t);

	void parallax_func2(float t);

	void parallax_func3(float t);

public:
#pragma mark -
#pragma mark Constructors
	/**
	 * Creates a new loading mode with the default values.
	 *
	 * This constructor does not allocate any objects or start the game.
	 * This allows us to use the object without a heap pointer.
	 */
	TitleProgrammedScene() : Scene() {}

	/**
	 * Disposes of all (non-static) resources allocated to this mode.
	 *
	 * This method is different from dispose() in that it ALSO shuts off any
	 * static resources, like the input controller.
	 */
	~TitleProgrammedScene() { dispose(); }

	/**
	 * Disposes of all (non-static) resources allocated to this mode.
	 */
	void dispose();

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
	bool init(const std::shared_ptr<cugl::AssetManager>& assets);


#pragma mark -
#pragma mark Progress Monitoring
	/**
	 * The method called to update the game mode.
	 *
	 * This method updates the frame to be drawn in the animation.
	 *
	 * @param timestep  The amount of time (in seconds) since the last frame
	 */
	void update(float timestep);

};
