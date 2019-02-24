#include <cugl/cugl.h>
#include "PlatformMap.h"

using namespace cugl;
using namespace std;


shared_ptr<PlatformMap> PlatformMap::parseFromJSON(string filename, shared_ptr<AssetManager> _assets) {
	return make_shared<PlatformMap>();
}

shared_ptr<PlatformMap> PlatformMap::alloc() {
	shared_ptr<PlatformMap> map = make_shared<PlatformMap>();
	map->_node = Node::alloc();
	map->layers = {};
	return map;
}

void PlatformMap::setScale(float x, float y) {
	_node->setScale(x, y);
}

void PlatformMap::setPosition(float x, float y) {
	_node->setPositionX(x);
	_node->setPositionY(y);
}

void PlatformMap::addPlatform(shared_ptr<Platform> platform) {
	platform->setMapSize(_x_start, _x_end);
	layers.push_back(platform);
	_node->addChild(platform->_node);
}

void PlatformMap::updatePlatformPositions() {
	for (int i = 0; i < layers.size(); i++) {
		layers[i]->updatePosition();
	}
}


void PlatformMap::parallaxTranslatePlatforms(float reference_x, float reference_y, float reference_dx) {
	for (int i = 0; i < layers.size(); i++) {
		layers[i]->parallaxTranslate(reference_x, reference_y, reference_dx);
	}
}

void PlatformMap::parallaxTranslatePlatforms(Vec2 reference, float reference_dx) {
	for (int i = 0; i < layers.size(); i++) {
		layers[i]->parallaxTranslate(reference.x, reference.y, reference_dx);
	}
}

void PlatformMap::setMapSize(int x_start, int x_end) {
	_map_size = x_end-x_start;
	_x_start = x_start;
	_x_end = x_end;
	for (int i = 0; i < layers.size(); i++) {
		layers[i]->setMapSize(x_start, x_end);
	}
}