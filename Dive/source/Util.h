#pragma once

//Value used to offset IDs based on Tiled tilesheet assignments
#define OFFSET 1
//Values used to identify objects in object layer
#define PLAYER_TILE_ID 1+OFFSET
#define URCHIN_TILE_ID 2+OFFSET
#define FISH_TILE_ID   3+OFFSET
#define JELLY_TILE_ID  4+OFFSET
#define GOAL_TILE_ID   5+OFFSET
#define PLANT_TILE_ID  6+OFFSET

class Util {

public:

	/** Same as the normal '%' function except it does not return negatives */
	static int mod(int a, int n) {
		return ((a % n) + n) % n;
	}

};
