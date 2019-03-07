#pragma once
#include "GridRow.h"
#include "TiledTexture.h"
#include "cugl/cugl.h"

using namespace std;
using namespace cugl;

class GridMap
{
protected:
	//Properties of map
	int _height, _width, _tile_width, _tile_height;
	double _opacity = 1;
	bool _visible = true;

	//Layer tile data
	shared_ptr<TiledTexture> _tilesheet;
	vector<std::shared_ptr<GridRow>> _rows = {};
	

public:
	
	//Grabs tile information from layer location
	int getBlock(int row, int column);

	//Grabs tile information from layer location
	int getBlock(Vec2 pos);

	//Grabs tile information from layer location
	void setBlock(int row, int column, int value);

	int getRowCount();

	int getColumnCount();

	int getTileWidth();

	int getTileHeight();

	Vec2 tileToMapCoords(int row, int col, float grid_size);

	Vec2 mapToTileCoords(float x, float y, float grid_size);

	//Set the relative speed of a given layer. Should be relative to 1.0f
	void setRelativeSpeed(int row, float relative_speed);

	//Translates a layer. Also translates other layers accordingly
	void translate(int reference_offset);

	/*
	 * Parse a map from a json file generated with map editor software named "Tiled"
	 */
	static shared_ptr<GridMap> parseFromJSON(string file, shared_ptr<AssetManager> _assets);

};