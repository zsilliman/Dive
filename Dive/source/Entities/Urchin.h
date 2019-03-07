#pragma once

#include <cugl/cugl.h>
#include "Entity.h"

using namespace cugl;
using namespace std;

class Urchin : public Entity {

public:

	static shared_ptr<Urchin> alloc(Vec2 start_pos);

};