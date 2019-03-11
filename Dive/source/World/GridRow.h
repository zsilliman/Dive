#pragma once

#include <cugl/cugl.h>
#include <cJSON/cJSON.h>
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>
#include "TiledTexture.h"

/* TiledLayer is a class used by TiledMap that holds data about a
 * single layer in a map. Currently it renders the tiles to the
 * screen without any regard for viewport. Thus it is inefficient
 * and needs improvement.  However, it does support parallax movement
 * between layers.
 */
class GridRow
{

protected:
	//Properties of a single layer
	int _row_index, _width;
	double _opacity = 1.0;
	bool visible = true;

	//A single row of the grid
	std::vector<int> data;
	//The speed relative to the reference point
	float _relative_speed = -1;
	//Number of units slid. (right is positive direction)
	int offset = 0;

public:
	
	static std::shared_ptr<GridRow> alloc(int row_index, int row_width);

	void setBlock(int index, int value);

	int getBlock(int index);

	int getRowIndex() { return _row_index; }

	int getColumnCount() { return _width; }

	void translate(int reference_offset);

	void setRelativeSpeed(float relative_speed);

	void reset();
};