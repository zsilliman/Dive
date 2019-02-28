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
    map->goal_layers = {};
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

void PlatformMap::addPlatform(shared_ptr<Goal> goal) {
    goal->setMapSize(_x_start, _x_end);
    goal_layers.push_back(goal);
    _node->addChild(goal->_node);
}

void PlatformMap::updatePlatformPositions(float dt) {
	for (int i = 0; i < layers.size(); i++) {
		layers[i]->updatePosition();
	}
    
    for (int i = 0; i < goal_layers.size(); i++) {
        goal_layers[i]->updatePosition(dt);
    }
}


void PlatformMap::parallaxTranslatePlatforms(float reference_x, float reference_y, float reference_dx) {
	for (int i = 0; i < layers.size(); i++) {
		layers[i]->parallaxTranslate(reference_x, reference_y, reference_dx);
	}
    
    for (int i = 0; i < goal_layers.size(); i++) {
        goal_layers[i]->parallaxTranslate(reference_x, reference_y, reference_dx);
    }
}

void PlatformMap::parallaxTranslatePlatforms(Vec2 reference, float reference_dx) {
	for (int i = 0; i < layers.size(); i++) {
		layers[i]->parallaxTranslate(reference.x, reference.y, reference_dx);
	}
    
    for (int i = 0; i < goal_layers.size(); i++) {
        goal_layers[i]->parallaxTranslate(reference.x, reference.y, reference_dx);
    }
}

void PlatformMap::setMapSize(int x_start, int x_end) {
	_map_size = x_end-x_start;
	_x_start = x_start;
	_x_end = x_end;
	for (int i = 0; i < layers.size(); i++) {
		layers[i]->setMapSize(x_start, x_end);
	}
    
    for (int i = 0; i < goal_layers.size(); i++) {
        goal_layers[i]->setMapSize(x_start, x_end);
    }
}

void PlatformMap::anchorCameraTo(float x, float y, float _player_screen_x, float _player_screen_y) {
	_node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	_node->setPositionX(-x + _player_screen_x);
	_node->setPositionY(-y + _player_screen_y);
}

void PlatformMap::anchorCameraTo(Vec2 position, Vec2 player_screen_pos) {
	anchorCameraTo(position.x, position.y, player_screen_pos.x, player_screen_pos.y);
}
