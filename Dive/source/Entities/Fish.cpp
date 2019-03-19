//
//  Fish.cpp
//  Dive (Mac)
//
//  Created by Pippen Wu on 2019/2/28.
//  Copyright © 2019 Game Design Initiative at Cornell. All rights reserved.
//

#include "Fish.h"

using namespace cugl;
using namespace std;


//Note start pos center
void Fish::initFish(Vec2 start_pos, Rect map_rect) {
	_dimensions = Size(1, 1);
	_position = Vec2(start_pos + _dimensions / 2);
	_start_pos = Vec2(start_pos + _dimensions / 2);
	_box = BoxObstacle::alloc(_start_pos, Size(0.9,0.9));
	_box_dup = BoxObstacle::alloc(_start_pos + Vec2(map_rect.size.width, 0), Size(0.9,0.9));
	_can_float = false;
	_box->setFriction(FISH_FRICTION);
	_box->setAngularDamping(FISH_ANG_DAMP);
	_box->setRestitution(FISH_RESTITUTION);
	_box->setDensity(FISH_DENSITY);
	_box->setLinearDamping(FISH_LIN_DAMP);
	_box->setBodyType(b2BodyType::b2_dynamicBody);
	_box->setGravityScale(0);

	_box_dup->setFriction(FISH_FRICTION);
	_box_dup->setAngularDamping(FISH_ANG_DAMP);
	_box_dup->setRestitution(FISH_RESTITUTION);
	_box_dup->setDensity(FISH_DENSITY);
	_box_dup->setLinearDamping(FISH_LIN_DAMP);
	_box_dup->setBodyType(b2BodyType::b2_dynamicBody);
	_box_dup->setGravityScale(0);
}

shared_ptr<Fish> Fish::alloc(Vec2 start_pos, Rect map_rect) {
	shared_ptr<Fish> fish = make_shared<Fish>();
	fish->initFish(start_pos, map_rect);
	return fish;
}