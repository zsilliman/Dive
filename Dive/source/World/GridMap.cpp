#include "GridMap.h"

//Grabs tile information from layer location
int GridMap::getBlock(int row, int column) {
	return _rows[row]->getBlock(column);
}

int GridMap::getBlock(Vec2 pos) {
	return _rows[(int)pos.y]->getBlock((int)pos.x);
}

void GridMap::setBlock(int row, int column, int value) {
	_rows[row]->setBlock(column, value);
}

int GridMap::getTileWidth() { return _tile_width; }

int GridMap::getTileHeight() { return _tile_height; }

//Set the relative speed of a given layer. Should be relative to 1.0f
void GridMap::setRelativeSpeed(int row, float relative_speed) {
	_rows[row]->setRelativeSpeed(relative_speed);
}

//Translates a layer. Also translates other layers accordingly
void GridMap::translate(int reference_offset) {
	for (int i = 0; i < _rows.size(); i++) {
		_rows[i]->translate(reference_offset);
	}
}

int GridMap::getRowCount() {
	return _height;
}

int GridMap::getColumnCount() {
	return _width;
}

Vec2 GridMap::tileToMapCoords(int row, int col, float grid_size) {
	return Vec2(col*grid_size, (-row - 1) * grid_size);
}

Vec2 GridMap::mapToTileCoords(float x, float y, float grid_size) {
	int col = floor(x / grid_size);
	int row = -floor(y / grid_size) - 1;
	return Vec2(col, row);
}

void GridMap::reset() {
	for (int i = 0; i < _rows.size(); i++) {
		_rows[i]->reset();
	}
}

/*
	* Parse a map from a json file generated with map editor software named "Tiled"
	*/
shared_ptr<GridMap> GridMap::parseFromJSON(string file, shared_ptr<AssetManager> _assets) {
	shared_ptr<GridMap> map = make_shared<GridMap>();

	/*std::shared_ptr<JsonReader> reader = JsonReader::allocWithAsset(file);
	shared_ptr<JsonValue> json = reader->readJson();

	//Tile sheet
	shared_ptr<JsonValue> properties = json->get("properties");
	string tileasset = getPropertyString(properties, "asset");
	vector<int> rel_speeds = getPropertyIntList(properties, "speeds");
	map->_tile_width = json->get("tileheight")->asInt();
	map->_tile_height = json->get("tilewidth")->asInt();
	shared_ptr<Texture> texture = _assets->get<Texture>(tileasset);
	map->_tilesheet = TiledTexture::alloc(texture, map->_tile_width, map->_tile_height);

	//Map properties
	map->_height = json->get("height")->asInt();
	map->_width = json->get("width")->asInt();

	//Layer properties

	shared_ptr<JsonValue> layers = json->get("layers");

	//Grabs first layer
	shared_ptr<JsonValue> layer = layers->get(0);
	vector<int> data = layer->get("data")->asIntArray();

	int blk_counter = 0;
	for (int y = 0; y < map->_height; y++) {
		shared_ptr<GridRow> row = GridRow::alloc(y, map->_width);
		bool has_blk = false;
		for (int x = 0; x < map->_width; x++) {
			int index = y * map->_width + x;
			row->setBlock(x, data[index] - 1);	
			if (data[index] - 1 >= 0)
				has_blk = true;
		}
		if (has_blk) {
			row->setRelativeSpeed(rel_speeds[blk_counter]);
			blk_counter++;
		}
		map->_rows.push_back(row);
	}
	*/
	return map;
}
