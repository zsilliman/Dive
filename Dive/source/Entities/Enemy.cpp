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

shared_ptr<Enemy> Enemy::allocWithTexture(shared_ptr<Texture> texture) {
    shared_ptr<Enemy> enemy = make_shared<Enemy>();
    enemy->_node = PolygonNode::allocWithTexture(texture);
    return enemy;
}

void Enemy::initPhysics(shared_ptr<ObstacleWorld> world) {
    _body = WheelObstacle::alloc(_node->getPosition(), _node->getTexture()->getWidth() * _node->getScaleX()/2);
    _body->setBodyType(b2BodyType::b2_dynamicBody);
    // Set the physics attributes
    _body->setDensity(URCHIN_DENSITY);
    _body->setFriction(ENEMY_FRICTION);
    _body->setAngularDamping(URCHIN_DAMPING);
    _body->setRestitution(BASIC_RESTITUTION);
	_body->setInertia(0.0f);
	_body->setLinearDamping(0.1f);
	_body->setGravityScale(0.3f);
	_body->setName("enemy");

	accelleration = 1.5f;
	max_speed = 0.8f;

    world->addObstacle(_body);
}