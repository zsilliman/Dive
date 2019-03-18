#pragma once

#include <cugl/cugl.h>

using namespace cugl;
using namespace std;

#define PLATFORM_DENSITY 1
#define PLATFORM_FRICTION 1
#define PLATFORM_RESTITUTION 0.01

class Platform : public SimpleObstacle
{

protected:
	Vec2 _initial_pos = Vec2(0, 0);
	float _relative_speed = 120;
	//Useful for ignoring collisions between
	int _platform_id = 0;

	void rec_init(vector<int>* grid, Vec2 current, Vec2 map_dimen);

	//Physics fields
	vector<b2FixtureDef> fixture_defs = {};
	vector<b2PolygonShape> shapes = {};
	vector<b2Fixture*> fixtures = {};

public:
	vector<Vec2> adj_tiles = {};
	vector<int> adj_values = {};

	void parallaxTranslate(float reference_dx);

	void initGrid(vector<int>* grid, Vec2 start, Vec2 map_dimen);

	void setRelativeSpeed(float relative_speed) { _relative_speed = relative_speed; }

	float getRelativeSpeed() { return _relative_speed; }

	Rect getPlatformRect();

	Size getPlatformSize();

	Vec2 getMinCorner();

	Vec2 getMaxCorner();

	void reset();

	shared_ptr<Platform> duplicate();

	static shared_ptr<Platform> allocWithGrid(vector<int>* grid, Vec2 start, Vec2 map_dimen);

	//Override Physics Functions
	void createFixtures() override;

	void releaseFixtures() override;
    
};
