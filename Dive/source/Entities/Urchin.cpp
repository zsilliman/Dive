#include "Urchin.h"

using namespace cugl;
using namespace std;

//Note start pos center
void Urchin::initUrchin(Vec2 start_pos, Rect map_rect) {
	_dimensions = Size(0.8, 0.8);
	_position = Vec2(start_pos + _dimensions / 2);
	_start_pos = Vec2(start_pos + _dimensions / 2);
	_box = WheelObstacle::alloc(_start_pos, 0.4);
	_box_dup = WheelObstacle::alloc(_start_pos + Vec2(map_rect.size.width, 0), 0.4);
	_can_float = false;
	_box->setFriction(URCHIN_FRICTION);
	_box->setAngularDamping(URCHIN_ANG_DAMP);
	_box->setRestitution(URCHIN_RESTITUTION);
	_box->setDensity(URCHIN_DENSITY);
	_box->setLinearDamping(URCHIN_LIN_DAMP);
	_box->setBodyType(b2BodyType::b2_dynamicBody);

	_box_dup->setFriction(URCHIN_FRICTION);
	_box_dup->setAngularDamping(URCHIN_ANG_DAMP);
	_box_dup->setRestitution(URCHIN_RESTITUTION);
	_box_dup->setDensity(URCHIN_DENSITY);
	_box_dup->setLinearDamping(URCHIN_LIN_DAMP);
	_box_dup->setBodyType(b2BodyType::b2_dynamicBody);
}

shared_ptr<Urchin> Urchin::alloc(Vec2 start_pos, Rect map_rect) {
	shared_ptr<Urchin> urchin = make_shared<Urchin>();
	urchin->initUrchin(start_pos, map_rect);
	return urchin;
}