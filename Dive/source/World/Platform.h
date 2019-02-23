#pragma once

#include <cugl/cugl.h>

using namespace cugl;
using namespace std;

class Platform
{

protected:
	shared_ptr<BoxObstacle> _body;

	Vec2 _initial_pos = Vec2(0, 0);

	int _map_size = 0;


public:
	shared_ptr<PolygonNode> _node;

	static shared_ptr<Platform> parseFromJSON(shared_ptr<JsonValue> json, shared_ptr<AssetManager> _assets);

	static shared_ptr<Platform> allocWithTexture(shared_ptr<Texture> texture);

	void setScale(float x, float y);

	void setInitialPosition(float x, float y);

	void initPhysics(shared_ptr<ObstacleWorld> world);

	void updatePosition();

	void parallaxTranslate(float reference_x, float reference_y, float reference_dx);

	void setMapSize(int map_size);

};