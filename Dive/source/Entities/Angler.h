#pragma once

#include <cugl/cugl.h>
#include "Entity.h"

#define ANGLER_FRICTION 0.3
#define ANGLER_RESTITUTION 0.3
#define ANGLER_ANG_DAMP 0.0
#define ANGLER_LIN_DAMP 0.0
#define ANGLER_DENSITY 0
#define ANGLER_HORI_MAX_SPEED 2.8
#define ANGLER_VERT_MAX_SPEED 6.5
#define ANGLER_VIEW_DIST 5

using namespace cugl;
using namespace std;

class Angler : public Entity {

public:

	void initAngler(Vec2 start_pos, Rect map_rect);

	static shared_ptr<Angler> alloc(Vec2 start_pos, Rect map_rect, int index);

};
