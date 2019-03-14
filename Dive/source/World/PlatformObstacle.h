#pragma once

#include <cugl/cugl.h>

using namespace cugl;
using namespace std;

class PlatformObstacle : public BoxObstacle {

	vector<b2Fixture> fixtures = {};
	vector<b2PolygonShape> shapes = {};

	/**
	* Create new fixtures for this body, defining the shape
	*
	* This is the primary method to override for custom physics objects.
	*/
	void createFixtures() override;

	void releaseFixtures() override;

};