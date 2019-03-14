#pragma once

#include <cugl/cugl.h>
#include "Platform.h"
#include "Goal.h"

using namespace cugl;
using namespace std;

class PlatformMap
{

protected:
	
	vector<shared_ptr<Platform>> platforms;

	float _height, _width;

public:

	static shared_ptr<PlatformMap> parseFromJSON(string filename, shared_ptr<AssetManager> _assets);

	static shared_ptr<PlatformMap> alloc();

	const vector<shared_ptr<Platform>>& getPlatforms() { return platforms; };
	const float getWidth() { return _width; }
	const float getHeight() { return _height; }

	void initPhysics(shared_ptr<ObstacleWorld> world) {
		for (int i = 0; i < platforms.size(); i++) {
			world->addObstacle(platforms[i]);
		}
	}

	void addPlatform(shared_ptr<Platform> platform);

	void reset();

	void parallaxTranslatePlatforms(float reference_dx);

};
