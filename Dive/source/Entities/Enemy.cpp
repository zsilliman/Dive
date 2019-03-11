//
//  Enemy.cpp
//  Dive (Mac)
//
//  Created by Pippen Wu on 2019/2/28.
//  Copyright © 2019 Game Design Initiative at Cornell. All rights reserved.
//

#include "Enemy.h"

using namespace cugl;
using namespace std;

shared_ptr<Enemy> Enemy::alloc(Vec2 start_pos) {
	shared_ptr<Enemy> enemy = make_shared<Enemy>();
	enemy->_position.set(start_pos);
	enemy->setCanFloat(false);
	enemy->_current_direction = Direction::RIGHT;
	enemy->_start_pos.set(start_pos);
	return enemy;
}