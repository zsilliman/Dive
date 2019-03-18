#include "Platform.h"
#include "../Util.h"

using namespace cugl;
using namespace std;


int getIndex(Vec2 pos, Vec2 map_dimen) {
	return (int)((map_dimen.y - pos.y - 1) * map_dimen.x + Util::mod(pos.x, map_dimen.x));
}

/** Returns the location after moving either up,down,left or right */
Vec2 getAdjacent(Vec2 d, Vec2 pos, Vec2 map_dimen) {
	pos.x = Util::mod(pos.x + d.x, map_dimen.x);
	pos.y += d.y;

	//Check for y is in bounds
	if (pos.y < 0 || pos.y >= map_dimen.y) {
		pos.y -= d.y;
		return pos;
	}
	return pos;
}

/** Returns the value at the corresponding block or -1 if the value is out of bounds */
int getAdjacentValue(vector<int>* grid, Vec2 d, Vec2 pos, Vec2 map_dimen) {
	Vec2 adj = getAdjacent(d, pos, map_dimen);
	int block = (*grid)[getIndex(adj, map_dimen)];

	return block;
}

bool contains(vector<Vec2>* lst, Vec2 pos, Vec2 map_dimen) {
	vector<Vec2>& lst_ref = *lst;
	Vec2 modded = Vec2(pos);
	modded.x = Util::mod(modded.x, map_dimen.x);
	for (int i = 0; i < lst_ref.size(); i++) {
		if (lst_ref[i].equals(pos) || lst_ref[i].equals(modded))
			return true;
	}
	return false;
}

shared_ptr<Platform> Platform::duplicate() {
	shared_ptr<Platform> platform = make_shared<Platform>();
	//copy values
	platform->adj_tiles = adj_tiles;
	platform->adj_values = adj_values;
	platform->_initial_pos.set(_initial_pos);
	platform->_relative_speed = _relative_speed;
	return platform;
}

shared_ptr<Platform> Platform::allocWithGrid(vector<int>* grid, Vec2 start, Vec2 map_dimen) {
	shared_ptr<Platform> platform = make_shared<Platform>();
	platform->initGrid(grid, start, map_dimen);
	platform->_initial_pos.set(platform->getMinCorner());
	return platform;
}

void Platform::initGrid(vector<int>* grid, Vec2 start, Vec2 map_dimen) {
	rec_init(grid, start, map_dimen);
	//set all adjacent values to 0 to prevent overlap of platforms
	for (int i = 0; i < adj_tiles.size(); i++) {
		int index = getIndex(adj_tiles[i], map_dimen);
		(*grid)[index] = 0;
	}
}

/** DFS search of all neighboring tiles */
void Platform::rec_init(vector<int>* grid, Vec2 current, Vec2 map_dimen) {
	vector<int>& grid_ref = *grid;
	Vec2 dir_buf[4] = { Vec2(1,0), Vec2(-1,0), Vec2(0,1), Vec2(0,-1) };

	//Add current to list of visited
	adj_tiles.push_back(current);
	int index = getIndex(current, map_dimen);
	int blk = grid_ref[index];
	adj_values.push_back(blk);

	//Iterate over directions
	for (int i = 0; i < 4; i++) {
		Vec2 adj = current + dir_buf[i]; //getAdjacent(dir_buf[i], current, map_dimen);
		//Checks whether adj is out of bounds in y direction. x direction is not necessary to check
		if (adj.y < 0 || adj.y >= map_dimen.y)
			adj.set(current);

		int blk = grid_ref[getIndex(adj, map_dimen)];
		//If the block exists and was not put in adj_tiles yet
		if (blk > 0 && !contains(&adj_tiles, adj, map_dimen)) {
			rec_init(grid, adj, map_dimen);
		}
	}
}

void Platform::parallaxTranslate(float reference_dx) {
	//Vec2 new_pos = getPosition() + Vec2(reference_dx * _relative_speed, 0);
	//setPosition(new_pos);
	setLinearVelocity(Vec2(reference_dx * _relative_speed, 0));
}

Rect Platform::getPlatformRect() {
	Vec2 plat_pos = getPosition();
	Size plat_size = getPlatformSize();
	return Rect(plat_pos.x, plat_pos.y, plat_size.width, plat_size.height);
}

Vec2 Platform::getMinCorner() {
	Vec2 min = Vec2(999, 999);
	for (int i = 0; i < adj_tiles.size(); i++) {
		if (min.x > adj_tiles[i].x)
			min.x = adj_tiles[i].x;
		if (min.y > adj_tiles[i].y)
			min.y = adj_tiles[i].y;
	}
	return min;
}

Vec2 Platform::getMaxCorner() {
	Vec2 max = Vec2(-999, -999);
	for (int i = 0; i < adj_tiles.size(); i++) {
		if (max.x < adj_tiles[i].x)
			max.x = adj_tiles[i].x;
		if (max.y < adj_tiles[i].y)
			max.y = adj_tiles[i].y;
	}
	//Add 1 to each because each index corresponds to lower left corner
	return max + Vec2(1, 1);
}

Size Platform::getPlatformSize() {
	Vec2 max = getMaxCorner();
	Vec2 min = getMinCorner();
	return Size(max - min);
}

void Platform::createFixtures() {
	fixture_defs.resize(adj_tiles.size());
	shapes.resize(adj_tiles.size());
	fixtures.resize(adj_tiles.size());
	Vec2 min_corner = getMinCorner();
	for (int i = 0; i < adj_tiles.size(); i++) {
		fixture_defs[i].density = PLATFORM_DENSITY;
		fixture_defs[i].friction = PLATFORM_FRICTION;
		fixture_defs[i].restitution = PLATFORM_RESTITUTION;

		shapes[i] = b2PolygonShape();
		Vec2 center = adj_tiles[i] - min_corner + Vec2(0.5, 0.5);
		shapes[i].SetAsBox(0.5, 0.5, b2Vec2(center.x, center.y), 0);
		fixture_defs[i].shape = &shapes[i];
		fixtures[i] = _body->CreateFixture(&fixture_defs[i]);
		setBodyType(b2BodyType::b2_kinematicBody);
		setGravityScale(0);
	}
	setPosition(min_corner);
	_initial_pos.set(min_corner);
}

void Platform::releaseFixtures() {
	for (int i = 0; i < fixtures.size(); i++) {
		_body->DestroyFixture(fixtures[i]);
	}
}

void Platform::reset() {
	setPosition(_initial_pos);
	setAngle(0);
}


/*
Polygon building algorithm. Ran into issues. Usefulness is tbd
int Platform::countNeighbors(Vec2 map_dimen, Vec2 pos) {
	vector<int>& grid_ref = *grid;
	int count = 0;
	//Compute blocks that surround given position
	int top_right = getAdjacentValue(grid, Vec2(0,0), pos, map_dimen);
	int bottom_right = getAdjacentValue(grid, Vec2(0, -1), pos, map_dimen);
	int top_left = getAdjacentValue(grid, Vec2(-1, 0), pos, map_dimen);
	int bottom_left = getAdjacentValue(grid, Vec2(-1, -1), pos, map_dimen);

	//Count adjacent blocks
	if (top_right >= 0) count++;
	if (bottom_right >= 0) count++;
	if (top_left >= 0) count++;
	if (bottom_left >= 0) count++;

	return count;
}

void Platform::buildPolygon(vector<int>* grid, Vec2 map_dimen) {
	_poly = Poly2();
	vector<Vec2> outline = {};
	//Determine outline of platform
	vector<Vec2> all_vertices = {};
	//Step 1: Enumerate Vertices of all blocks
	for (int i = 0; i < adj_tiles.size(); i++) {
		//enumerate vertices of a single block
		Vec2 bottom_left = adj_tiles[i];
		Vec2 bottom_right = bottom_left + Vec2(1,0);
		Vec2 top_left = bottom_left + Vec2(0,1);
		Vec2 top_right = bottom_left + Vec2(1,1);
		//add them to list of vertices
		if (!contains(&all_vertices, bottom_left))
			all_vertices.push_back(bottom_left);
		if (!contains(&all_vertices, bottom_right))
			all_vertices.push_back(bottom_right);
		if (!contains(&all_vertices, top_left))
			all_vertices.push_back(top_left);
		if (!contains(&all_vertices, top_right))
			all_vertices.push_back(top_right);
	}
	//Step 2: Filter out non-essential vertices
	for (int i = 0; i < all_vertices.size(); i++) {
		int neighbors = countNeighbors(map_dimen, all_vertices[i]);
		if (neighbors == 1 || neighbors == 3) {
			outline.push_back(all_vertices[i]);
		}
	}

	//Step 3: reorder vertices (???)

	_poly.set(outline);
	_body = PolygonObstacle::alloc(_poly);
}*/