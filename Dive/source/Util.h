#pragma once

#define PI 3.14159265358979323846
//Value used to offset IDs based on Tiled tilesheet assignments
#define OFFSET 1
//Values used to identify objects in object layer
#define PLAYER_TILE_ID      1+OFFSET
#define URCHIN_TILE_ID      2+OFFSET
#define FISH_LEFT_TILE_ID   3+OFFSET
#define ANGLER_TILE_ID      4+OFFSET
#define GOAL_TILE_ID        5+OFFSET
#define FISH_RIGHT_TILE_ID  6+OFFSET

enum State { WIN, LOSE, PLAY, PAUSE };

class Util {

public:

	/** Same as the normal '%' function except it does not return negatives */
	static int mod(int a, int n) {
		return ((a % n) + n) % n;
	}

};
