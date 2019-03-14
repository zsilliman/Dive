#include <cugl/cugl.h>
#include "PlatformMap.h"

using namespace cugl;
using namespace std;

shared_ptr<PlatformMap> PlatformMap::alloc() {
	shared_ptr<PlatformMap> map = make_shared<PlatformMap>();
	map->platforms = {};
	return map;
}

void PlatformMap::addPlatform(shared_ptr<Platform> platform) {
	platforms.push_back(platform);
}

void PlatformMap::parallaxTranslatePlatforms(float reference_dx) {
	for (int i = 0; i < platforms.size(); i++) {
		platforms[i]->parallaxTranslate(reference_dx);
	}
}

void PlatformMap::reset() {
	for (int i = 0; i < platforms.size(); i++) {
		platforms[i]->reset();
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

vector<int> getPropertyIntList(shared_ptr<JsonValue> properties, string key) {
	for (int i = 0; i < properties->size(); i++) {
		shared_ptr<JsonValue> prop = properties->get(i);
		string jsonKey = prop->get("name")->asString();
		if (jsonKey.compare(key) == 0) {
			string lst = prop->get("value")->asString();
			return JsonValue::allocWithJson(lst)->asIntArray();
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

	//Tile sheet
	shared_ptr<JsonValue> properties = json->get("properties");
	string tileasset = getPropertyString(properties, "asset");
	vector<int> rel_speeds = getPropertyIntList(properties, "speeds");
	shared_ptr<Texture> texture = _assets->get<Texture>(tileasset);

	//Map properties
	int tile_height = json->get("height")->asInt();
	int tile_width = json->get("width")->asInt();
	map->_height = tile_height;
	map->_width = tile_width;

	//Layer properties
	shared_ptr<JsonValue> layers = json->get("layers");

	//Grabs first layer
	shared_ptr<JsonValue> tile_layer = layers->get(0);
	shared_ptr<JsonValue> obj_layer = layers->get(0);
	vector<int> data = tile_layer->get("data")->asIntArray();
	Vec2 map_dimen = Vec2(tile_width, tile_height);

	int blk_counter = 0;
	for (int y = 0; y < tile_height; y++) {
		for (int x = 0; x < tile_width; x++) {
			int index = (tile_height - y - 1) * map->_width + x;
			//If a block exists
			if (data[index] > 0) {
				//Create platform from this and adjacent blocks
				Vec2 start = Vec2(x, y);
				shared_ptr<Platform> platform = Platform::allocWithGrid(&data, start, map_dimen);
				map->addPlatform(platform);
			}
		}
	}

	return map;
}