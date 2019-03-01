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

#pragma mark Physics Constants

// Physics constants for initialization
/** Density of non-urchin objects */
#define BASIC_DENSITY       0.01f
/** Density of the urchin objects */
#define URCHIN_DENSITY      1.0f
/** Friction of non-urchin objects */
#define BASIC_FRICTION      0.1f
/** Friction of the urchin objects */
#define ENEMY_FRICTION     0
/** Angular damping of the urchin objects */
#define URCHIN_DAMPING      0.0f
/** Collision restitution for all objects */
#define BASIC_RESTITUTION   0.1f


shared_ptr<Enemy> Enemy::allocWithTexture(shared_ptr<Texture> texture) {
    shared_ptr<Enemy> enemy = make_shared<Enemy>();
    enemy->_node = PolygonNode::allocWithTexture(texture);
    return enemy;
}

void Enemy::setScale(float x, float y) {
    _node->setScale(x, y);
}

//Note that init physics must be called first
void Enemy::setPhysicsPosition(float x, float y) {
    _body->setPosition(x, y);
}

void Enemy::push(Vec2 force) {
    if (force.length() < accelleration*(1.0f / 10.0f))
        return;
    Vec2 linear_velocity = _body->getLinearVelocity();
    
    //Normalize the direction from node to mouse click
    Vec2 dir = force.getNormalization();
    //Scale it to the accelleration speed
    dir.scale(accelleration);
    //Accellerate the player, but cap it at max_speed
    if ((linear_velocity + dir).length() <= max_speed) {
        linear_velocity += dir;
    }
    else {
        linear_velocity += dir;
        linear_velocity.scale(max_speed / linear_velocity.length());
    }
    //Set the velocity of the physics body
    _body->setLinearVelocity(linear_velocity);
}

void Enemy::initPhysics(shared_ptr<ObstacleWorld> world) {
    _body = WheelObstacle::alloc(_node->getPosition(), _node->getTexture()->getWidth() * _node->getScaleX()/2);
    _body->setBodyType(b2BodyType::b2_dynamicBody);
    // Set the physics attributes
    _body->setDensity(URCHIN_DENSITY);
    _body->setFriction(ENEMY_FRICTION);
    _body->setAngularDamping(URCHIN_DAMPING);
    _body->setRestitution(BASIC_RESTITUTION);
    _body->setLinearDamping(10);
    _body->setGravityScale(300);
    world->addObstacle(_body);
}

void Enemy::updatePosition() {
    _node->setPosition(_body->getPosition());
    _node->setAngle(_body->getAngle());
}
