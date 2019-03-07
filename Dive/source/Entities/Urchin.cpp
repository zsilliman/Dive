#include "Urchin.h"

using namespace cugl;
using namespace std;


shared_ptr<Urchin> Urchin::alloc(Vec2 start_pos) {
	shared_ptr<Urchin> urchin = make_shared<Urchin>();
	urchin->_position.set(start_pos);
	urchin->setCanFloat(false);
	return urchin;
}