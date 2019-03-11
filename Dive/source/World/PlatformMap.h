#pragma once

#include <cugl/cugl.h>
#include "Platform.h"
#include "Goal.h"

using namespace cugl;
using namespace std;

class PlatformMap
{

protected:
	
	vector<shared_ptr<Platform>> layers;

	int _map_size = 0, _x_start = 0, _x_end = 0;

public:

	shared_ptr<Node> _node;

	static shared_ptr<PlatformMap> parseFromJSON(string filename, shared_ptr<AssetManager> _assets);

	static shared_ptr<PlatformMap> alloc();

	void setScale(float x, float y);

	void setPosition(float x, float y);

	void addPlatform(shared_ptr<Platform> platform);
    
	void addToScene(shared_ptr<Scene> scene) { scene->addChild(_node); }

	void updatePlatformPositions(float dt);

	void parallaxTranslatePlatforms(float reference_x, float reference_y, float reference_dx);

	void parallaxTranslatePlatforms(Vec2 reference, float reference_dx);

	void setMapSize(int x_start, int x_end);

    void anchorCameraTo(float x, float y, float viewport_width, float viewport_height);

	void anchorCameraTo(Vec2 position, Vec2 viewport);

	void anchorCameraToY(float y, float screen_y);
};
