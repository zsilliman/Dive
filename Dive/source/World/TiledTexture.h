#pragma once

#include <cugl/cugl.h>

using namespace std;
using namespace cugl;

class TiledTexture
{

protected:
	bool _set = false;
	int _tile_width, _tile_height; //Number of pixels per tile
	int _columns, _rows, _tile_count;	   //Number of tiles
	shared_ptr<Texture> _texture;
	vector<shared_ptr<Texture>> _tiles = {};

public:
	TiledTexture();
	~TiledTexture();

	shared_ptr<Texture> getTile(int i);

	static shared_ptr<TiledTexture> alloc(shared_ptr<Texture> texture, int width, int height);

	void setTexture(shared_ptr<Texture> texture, int width, int height);

	/**
	 * Number of pixels along x-axis of tile
	 */
	int getTileWidth() { return _tile_width;  }

	/**
	 * Number of pixels along y-axis of tile
	 */
	int getTileHeight() { return _tile_height; }

	/**
	 * Number of tiles along x-axis of texture
	 */
	int getColumns() { return _columns; }

	/**
	 * Number of tiles along y-axis of texture
	 */
	int getRows() { return _rows; }

};