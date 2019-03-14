#include "PlatformObstacle.h"

void PlatformObstacle::createFixtures() {
	b2FixtureDef fixture;
	fixture.density = 0;
	fixture.friction = 0;
	fixture.restitution = 0;
	b2PolygonShape rect = b2PolygonShape();
	rect.SetAsBox(1, 1, b2Vec2(0,0), 0);
	fixture.shape = &rect;
	_body->CreateFixture(&fixture);
}

void PlatformObstacle::releaseFixtures() {

}