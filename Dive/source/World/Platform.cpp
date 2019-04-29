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
	platform->col_tiles = col_tiles;
	platform->adj_values = adj_values;
	platform->_initial_pos.set(_initial_pos);
	platform->_relative_speed = _relative_speed;
	platform->fixtures = {};
	platform->fixture_defs = {};
	platform->shapes = {};
	platform->setName(getName());
	return platform;
}

shared_ptr<Platform> Platform::allocWithGrid(vector<int>* collision_data, vector<int>* render_data, Vec2 start, Vec2 map_dimen, bool wrapping) {
	shared_ptr<Platform> platform = make_shared<Platform>();
	platform->initGrid(collision_data, render_data, start, map_dimen, wrapping);
	platform->_initial_pos.set(platform->getMinCorner());
	platform->setName("platform");
	return platform;
}

void Platform::initGrid(vector<int>* collision_data, vector<int>* render_data, Vec2 start, Vec2 map_dimen, bool wrapping) {
	rec_init(collision_data, render_data, start, map_dimen, wrapping);
	//set all adjacent values to 0 to prevent overlap of platforms
	for (int i = 0; i < adj_tiles.size(); i++) {
		int index = getIndex(adj_tiles[i], map_dimen);
		(*render_data)[index] = 0;
	}
}

/** DFS search of all neighboring tiles */
void Platform::rec_init(vector<int>* collision_data, vector<int>* render_data, Vec2 current, Vec2 map_dimen, bool wrapping) {
	vector<int>& render_ref = *render_data;
	Vec2 dir_buf[4] = { Vec2(1,0), Vec2(-1,0), Vec2(0,1), Vec2(0,-1) };

	//Add current to list of visited
	adj_tiles.push_back(current);
	int index = getIndex(current, map_dimen);
	if ((*collision_data)[index] > 0) {
		col_tiles.push_back(current);
	}
	int blk = render_ref[index];
	adj_values.push_back(blk);

	//Iterate over directions
	for (int i = 0; i < 4; i++) {
		Vec2 adj = current + dir_buf[i];
		//Checks whether adj is out of bounds in y direction.
		if (adj.y < 0 || adj.y >= map_dimen.y)
			adj.set(current);
		if (wrapping == false && adj.x < 0 && adj.x >= map_dimen.x)
			adj.set(current);
		//Checks whether adj is out of bounds in x direction if wrapping is disabled
		if (wrapping = false && (adj.x < 0 || adj.x >= map_dimen.x))
			adj.set(current);

		int blk = render_ref[getIndex(adj, map_dimen)];
		//If the block exists and was not put in adj_tiles yet
		if (blk > 0 && !contains(&adj_tiles, adj, map_dimen) && adj.x < map_dimen.x) {
			rec_init(collision_data, render_data, adj, map_dimen, wrapping);
		}
	}
}

void Platform::parallaxTranslate(float reference_dx) {
	//Vec2 new_pos = getPosition() + Vec2(reference_dx * _relative_speed, 0);
	//setPosition(new_pos);
	setLinearVelocity(Vec2(reference_dx * abs(_relative_speed), 0));
}

Rect Platform::getPlatformRect() {
	Vec2 plat_pos = getPosition();
	Size plat_size = getPlatformSize();
	return Rect(plat_pos.x, plat_pos.y, plat_size.width, plat_size.height);
}

Vec2 Platform::getMinCorner() {
	Vec2 min = Vec2(999, 999);
	for (int i = 0; i < col_tiles.size(); i++) {
		if (min.x > col_tiles[i].x)
			min.x = col_tiles[i].x;
		if (min.y > col_tiles[i].y)
			min.y = col_tiles[i].y;
	}
	return min;
}

Vec2 Platform::getMaxCorner() {
	Vec2 max = Vec2(-999, -999);
	for (int i = 0; i < col_tiles.size(); i++) {
		if (max.x < col_tiles[i].x)
			max.x = col_tiles[i].x;
		if (max.y < col_tiles[i].y)
			max.y = col_tiles[i].y;
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
	fixture_defs.resize(col_tiles.size());
	shapes.resize(col_tiles.size());
	fixtures.resize(col_tiles.size());
	Vec2 min_corner = getMinCorner();
	for (int i = 0; i < col_tiles.size(); i++) {
		fixture_defs[i].density = PLATFORM_DENSITY;
		fixture_defs[i].friction = PLATFORM_FRICTION;
		fixture_defs[i].restitution = PLATFORM_RESTITUTION;
		fixture_defs[i].isSensor = isSensor();

		shapes[i] = b2PolygonShape();
		Vec2 center = col_tiles[i] - min_corner + Vec2(0.5, 0.5);
		shapes[i].SetAsBox(0.5, 0.5, b2Vec2(center.x, center.y), 0);
		fixture_defs[i].shape = &shapes[i];
		fixtures[i] = _body->CreateFixture(&fixture_defs[i]);
		setBodyType(b2BodyType::b2_kinematicBody);
		setGravityScale(0);
	}
	setPosition(min_corner);
	setFixedRotation(true);
	setRestitution(0);
	_initial_pos.set(min_corner);
}

void Platform::releaseFixtures() {
	for (int i = 0; i < fixtures.size(); i++) {
		_body->DestroyFixture(fixtures[i]);
	}
}

void Platform::reset() {
	setPosition(_initial_pos);
	setLinearVelocity(Vec2(0,0));
	setAngle(0);
}
