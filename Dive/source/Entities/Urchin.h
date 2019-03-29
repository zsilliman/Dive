#pragma once

#include <cugl/cugl.h>
#include "Entity.h"

#define URCHIN_FRICTION 0
#define URCHIN_RESTITUTION 0.15
#define URCHIN_ANG_DAMP 0.3
#define URCHIN_LIN_DAMP 4
#define URCHIN_DENSITY 99999

using namespace cugl;
using namespace std;

class Urchin : public Entity {

public:

	void initUrchin(Vec2 start_pos, Rect map_rect);

	static shared_ptr<Urchin> alloc(Vec2 start_pos, Rect map_rect);

};
