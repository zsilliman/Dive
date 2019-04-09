#include "Angler.h"

using namespace cugl;
using namespace std;


//Note start pos center
void Angler::initAngler(Vec2 start_pos, Rect map_rect) {
	_dimensions = Size(1, 1);
	_position = Vec2(start_pos + _dimensions / 2);
	_start_pos = Vec2(start_pos + _dimensions / 2);
	_box = BoxObstacle::alloc(_start_pos, Size(0.9, 0.9));
	_box_dup = BoxObstacle::alloc(_start_pos + Vec2(map_rect.size.width, 0), Size(0.9, 0.9));
	_can_float = false;
	_box->setFriction(ANGLER_FRICTION);
	_box->setFixedRotation(true);
	_box->setAngularDamping(ANGLER_ANG_DAMP);
	_box->setRestitution(ANGLER_RESTITUTION);
	_box->setDensity(ANGLER_DENSITY);
	_box->setLinearDamping(ANGLER_LIN_DAMP);
	_box->setBodyType(b2BodyType::b2_dynamicBody);
	_box->setGravityScale(0);

	_box_dup->setFriction(ANGLER_FRICTION);
	_box_dup->setAngularDamping(ANGLER_ANG_DAMP);
	_box_dup->setRestitution(ANGLER_RESTITUTION);
	_box_dup->setDensity(ANGLER_DENSITY);
	_box_dup->setLinearDamping(ANGLER_LIN_DAMP);
	_box_dup->setBodyType(b2BodyType::b2_dynamicBody);
	_box_dup->setGravityScale(0);
}

shared_ptr<Angler> Angler::alloc(Vec2 start_pos, Rect map_rect, int index) {
	shared_ptr<Angler> angler = make_shared<Angler>();
	angler->initAngler(start_pos, map_rect);
	string name = "angler";
	angler->setName(name.append(std::to_string(index)));
	return angler;
}

