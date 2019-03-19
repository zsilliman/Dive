#pragma once

#include <cugl/cugl.h>
#include "Platform.h"
#include "Goal.h"

using namespace cugl;
using namespace std;

class PlatformMap
{

protected:
	
	vector<shared_ptr<Platform>> platforms = {};
	vector<shared_ptr<Platform>> platform_dups = {};

	float _height, _width;
	//Region to left of the map, rect defining the whole map, region to right of the map
	Rect map_rect;
	string asset_name;

    bool overlapsLeftEdge(Rect platform_rect);
	bool overlapsRightEdge(Rect platform_rect);

public:

	static shared_ptr<PlatformMap> parseFromJSON(shared_ptr<JsonValue> json, shared_ptr<AssetManager> _assets);

	static shared_ptr<PlatformMap> alloc();

	const vector<shared_ptr<Platform>>& getPlatforms() { return platforms; };
	const vector<shared_ptr<Platform>>& getPlatformDups() { return platform_dups; };
	const Rect getMapRect() { return map_rect; }
	const float getWidth() { return _width; }
	const float getHeight() { return _height; }

	void initPhysics(shared_ptr<ObstacleWorld> world) {
		for (int i = 0; i < platforms.size(); i++) {
			world->addObstacle(platforms[i]);
			world->addObstacle(platform_dups[i]);
		}
	}

	void rotatePlatforms();

	void addPlatform(shared_ptr<Platform> platform);

	void reset();

	void parallaxTranslatePlatforms(float reference_dx);

};
