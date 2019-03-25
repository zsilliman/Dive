#include <cugl/cugl.h>
#include "PlatformMap.h"
#include "Util.h"

using namespace cugl;
using namespace std;

shared_ptr<PlatformMap> PlatformMap::alloc() {
	shared_ptr<PlatformMap> map = make_shared<PlatformMap>();
	map->platforms = {};
	map->platform_dups = {};
	return map;
}

void PlatformMap::addPlatform(shared_ptr<Platform> platform) {
	platforms.push_back(platform);
    //platform_dups.push_back(platform);
}

void PlatformMap::parallaxTranslatePlatforms(float reference_dx) {
	for (int i = 0; i < platforms.size(); i++) {
		platforms[i]->parallaxTranslate(reference_dx);
		platform_dups[i]->parallaxTranslate(reference_dx);
	}
	goal->parallaxTranslate(reference_dx);
	goal_dup->parallaxTranslate(reference_dx);
}

void PlatformMap::reset() {
	for (int i = 0; i < platforms.size(); i++) {
		platforms[i]->reset();
		platform_dups[i]->reset();
	}
}

bool PlatformMap::overlapsLeftEdge(Rect platform_rect) {
	//If there is no intersection return false
	if (!platform_rect.doesIntersect(map_rect)) 
		return false;
	//If the lowerleft corner is less than 0, then we know it hit the left side
	if (platform_rect.getMinX() <= map_rect.getMinX())
		return true;
	//Otherwise it does not overlap the left edge
	return false;
}

bool PlatformMap::overlapsRightEdge(Rect platform_rect) {
	if (!platform_rect.doesIntersect(map_rect))
		return false;
	//If the lowerright corner is greater than map right edge, then we know it hit the right side
	if (platform_rect.getMaxX() >= map_rect.getMaxX())
		return true;
	//Otherwise it does not overlap the left edge
	return false;
}

void PlatformMap::rotatePlatform(shared_ptr<Platform> oc, shared_ptr<Platform> cp) {
	Rect oc_rect = oc->getPlatformRect();     //Original
	Rect cp_rect = cp->getPlatformRect(); //duplicate
	// oc_rect intersects left ==> cp_rect needs to be on right portion of the map
	if (overlapsLeftEdge(oc_rect)) {
		cp->setPosition(oc->getPosition() + Vec2(_width, 0));
	}
	//cp_rect intersects left ==> oc_rect needs to be on the right portion of the map
	else if (overlapsLeftEdge(cp_rect)) {
		oc->setPosition(cp->getPosition() + Vec2(_width, 0));
	}
	//original intersects right ==> copy needs to be on left portion of the map
	else if (overlapsRightEdge(oc_rect)) {
		cp->setPosition(oc->getPosition() - Vec2(_width, 0));
	}
	//copy intersects right ==> original needs to be on left portion of the map
	else if (overlapsRightEdge(cp_rect)) {
		oc->setPosition(cp->getPosition() - Vec2(_width, 0));
	}
	else if (map_rect.contains(oc_rect)) {
		cp->setPosition(oc->getPosition() + Vec2(_width, 0));
	}
	else if (map_rect.contains(cp_rect)) {
		oc->setPosition(cp->getPosition() + Vec2(_width, 0));
	}
}

void PlatformMap::rotatePlatforms() {
	for (int i = 0; i < platforms.size(); i++) {
		shared_ptr<Platform> oc = platforms[i];
		shared_ptr<Platform> cp = platform_dups[i];
		rotatePlatform(oc, cp);
	}
	rotatePlatform(goal, goal_dup);
}

//Read custom properties from Tiled
string getPropertyString(shared_ptr<JsonValue> properties, string key) {
	for (int i = 0; i < properties->size(); i++) {
		shared_ptr<JsonValue> prop = properties->get(i);
		string jsonKey = prop->get("name")->asString();
		if (jsonKey.compare(key) == 0) {
			return prop->get("value")->asString();
		}
	}
	CULog("Could not read string property");
	return "";
}

//Read custom properties from Tiled
int getPropertyInt(shared_ptr<JsonValue> properties, string key) {
	for (int i = 0; i < properties->size(); i++) {
		shared_ptr<JsonValue> prop = properties->get(i);
		string jsonKey = prop->get("name")->asString();
		if (jsonKey.compare(key) == 0) {
			return prop->get("value")->asInt();
		}
	}
	CULog("Could not read integer property");
	return -1;
}

vector<float> getPropertyFloatList(shared_ptr<JsonValue> properties, string key) {
	for (int i = 0; i < properties->size(); i++) {
		shared_ptr<JsonValue> prop = properties->get(i);
		string jsonKey = prop->get("name")->asString();
		if (jsonKey.compare(key) == 0) {
			string lst = prop->get("value")->asString();
			return JsonValue::allocWithJson(lst)->asFloatArray();
		}
	}
	CULog("Could not read integer property");
	return {};
}


/*
* Parse a map from a json file generated with map editor software named "Tiled"
*/
shared_ptr<PlatformMap> PlatformMap::parseFromJSON(shared_ptr<JsonValue> json, shared_ptr<AssetManager> _assets) {
	shared_ptr<PlatformMap> map = make_shared<PlatformMap>();

	//Map properties
	int tile_height = json->get("height")->asInt();
	int tile_width = json->get("width")->asInt();
	map->_height = tile_height;
	map->_width = tile_width;

	//Custom properties
	shared_ptr<JsonValue> properties = json->get("properties");
	vector<float> speed_data = getPropertyFloatList(properties, "speeds");
	map->asset_name = getPropertyString(properties, "asset");

	//Layer properties
	shared_ptr<JsonValue> layers = json->get("layers");

	//Grabs first layer
	shared_ptr<JsonValue> tile_layer = layers->get(0);
	vector<int> data = tile_layer->get("data")->asIntArray();

	shared_ptr<JsonValue> obj_layer = layers->get(1);
	vector<int> obj_data = obj_layer->get("data")->asIntArray();
	Vec2 map_dimen = Vec2(tile_width, tile_height);
	//map->map_rect = Rect(0, 0, tile_width, tile_height);
	map->map_rect = Rect(0, 0, tile_width, tile_height);

	for (int y = 0; y < tile_height; y++) {
		for (int x = 0; x < tile_width; x++) {
			int index = (tile_height - y - 1) * map->_width + x;
			//If a block exists
			if (data[index] > 0) {
				//Create platform from this and adjacent blocks
				Vec2 start = Vec2(x, y);
				shared_ptr<Platform> platform = Platform::allocWithGrid(&data, start, map_dimen);
				platform->setRelativeSpeed(speed_data[map->platform_dups.size()]);
				map->platforms.push_back(platform);
				//Create duplicate platform for edges of map
				shared_ptr<Platform> platform_dup = platform->duplicate();
				platform_dup->setPosition(platform->getPosition() + Vec2(map->_width, 0));
				map->platform_dups.push_back(platform_dup);
			}
			if (obj_data[index] == GOAL_TILE_ID) {
				Vec2 start = Vec2(x, y);
				map->goal = Goal::allocGoal(start);
				map->goal->setRelativeSpeed(speed_data[map->platform_dups.size()]);

				map->goal_dup = map->goal->duplicateGoal();
				map->goal_dup->setPosition(map->goal->getPosition() + Vec2(map->_width, 0));
			}
		}
	}
	map->rotatePlatforms();

	return map;
}
