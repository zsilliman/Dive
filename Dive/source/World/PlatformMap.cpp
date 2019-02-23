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
	platform->setMapSize(_map_size);
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

void PlatformMap::setMapSize(int map_size) {
	_map_size = map_size;
	for (int i = 0; i < layers.size(); i++) {
		layers[i]->setMapSize(map_size);
	}
}