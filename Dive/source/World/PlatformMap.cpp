#include <cugl/cugl.h>
#include "PlatformMap.h"

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
}

void PlatformMap::parallaxTranslatePlatforms(float reference_dx) {
	for (int i = 0; i < platforms.size(); i++) {
		platforms[i]->parallaxTranslate(reference_dx);
		platform_dups[i]->parallaxTranslate(reference_dx);
	}
}

void PlatformMap::reset() {
	for (int i = 0; i < platforms.size(); i++) {
		platforms[i]->reset();
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

void PlatformMap::rotatePlatforms() {
	for (int i = 0; i < platforms.size(); i++) {
		Rect oc_rect = platforms[i]->getPlatformRect();     //Original
		Rect cp_rect = platform_dups[i]->getPlatformRect(); //duplicate
		// oc_rect intersects left ==> cp_rect needs to be on right portion of the map
		if (overlapsLeftEdge(oc_rect)) {
			platform_dups[i]->setPosition(platforms[i]->getPosition() + Vec2(_width, 0));
		}
		//cp_rect intersects left ==> oc_rect needs to be on the right portion of the map
		else if (overlapsLeftEdge(cp_rect)) {
			platforms[i]->setPosition(platform_dups[i]->getPosition() + Vec2(_width, 0));
		}
		//original intersects right ==> copy needs to be on left portion of the map
		else if (overlapsRightEdge(oc_rect)) {
			platform_dups[i]->setPosition(platforms[i]->getPosition() - Vec2(_width, 0));
		}
		//copy intersects right ==> original needs to be on left portion of the map
		else if (overlapsRightEdge(cp_rect)) {
			platforms[i]->setPosition(platform_dups[i]->getPosition() - Vec2(_width, 0));
		}
	}
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
shared_ptr<PlatformMap> PlatformMap::parseFromJSON(string file, shared_ptr<AssetManager> _assets) {
	shared_ptr<PlatformMap> map = make_shared<PlatformMap>();

	std::shared_ptr<JsonReader> reader = JsonReader::allocWithAsset(file);
	shared_ptr<JsonValue> json = reader->readJson();

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
	shared_ptr<JsonValue> obj_layer = layers->get(0);
	vector<int> data = tile_layer->get("data")->asIntArray();
	Vec2 map_dimen = Vec2(tile_width, tile_height);
	map->map_rect = Rect(0, 0, map->_width, map->_height);

	int blk_counter = 0;
	for (int y = 0; y < tile_height; y++) {
		for (int x = 0; x < tile_width; x++) {
			int index = (tile_height - y - 1) * map->_width + x;
			//If a block exists
			if (data[index] > 0) {
				//Create platform from this and adjacent blocks
				Vec2 start = Vec2(x, y);
				shared_ptr<Platform> platform = Platform::allocWithGrid(&data, start, map_dimen);
				platform->setRelativeSpeed(speed_data[map->platforms.size()]);
				map->platforms.push_back(platform);
				//Create duplicate platform for edges of map
				map->platform_dups.push_back(platform->duplicate());
			}
		}
	}

	return map;
}