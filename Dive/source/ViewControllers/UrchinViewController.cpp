#include "UrchinViewController.h"
#include "Util.h"

void UrchinViewController::draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) {}

void UrchinViewController::update(shared_ptr<GameState> state) {
	/*Vec2 start_pos = state->_urchins[_urchin_index]->getPosition();
	if (canMove(state, Direction::DOWN))
		fall(state);
	Vec2 tile_pos = state->_urchins[_urchin_index]->getPosition();
	Vec2 map_pos = state->_map->tileToMapCoords(tile_pos.y, tile_pos.x, _node->getWidth());
	_node->setPosition(map_pos);*/
}

void UrchinViewController::dispose() {}

void UrchinViewController::reset() {

}

bool UrchinViewController::canMove(shared_ptr<GameState> state, Direction direction) {
	/*int width = state->_map->getColumnCount();
	int height = state->_map->getRowCount();

	Vec2 position = state->_urchins[_urchin_index]->getPosition();
	int new_x = 0;

	switch (direction) {
	case UP:
		if (position.y - 1 < 0) return false;
		return state->_map->getBlock(position.y - 1, position.x) <= -1;
	case DOWN:
		if (position.y + 1 >= height) return false;
		return state->_map->getBlock(position.y + 1, position.x) <= -1;
	case LEFT:
		new_x = Util::mod(position.x - 1, width);
		return state->_map->getBlock(position.y, new_x) <= -1;
	case RIGHT:
		new_x = Util::mod(position.x + 1, width);
		return state->_map->getBlock(position.y, new_x) <= -1;
	}
	return state->_map->getBlock(position.y, position.x) <= -1;*/
	return false;
}

void UrchinViewController::fall(shared_ptr<GameState> state) {
	//Continuous downward force
	/*if (state->_urchins[_urchin_index]->canFloat()) return;
	if (canMove(state, Direction::DOWN))
		state->_urchins[_urchin_index]->move(Direction::DOWN, state->_map->getColumnCount());*/
}

shared_ptr<UrchinViewController> UrchinViewController::alloc(shared_ptr<GameState> init_state, shared_ptr<Texture> texture, Size display, int urchin_index) {
	shared_ptr<UrchinViewController> urchin_vc = make_shared<UrchinViewController>();
	/*float grid_size = display.width / init_state->_map->getColumnCount();
	Vec2 start_pos = init_state->_urchins[urchin_index]->getPosition();
	urchin_vc->_node = PolygonNode::allocWithTexture(texture);
	urchin_vc->_display = display;
	urchin_vc->_node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	float scalex = grid_size / texture->getWidth();
	float scaley = grid_size / texture->getHeight();
	urchin_vc->_node->setScale(scalex, scaley);
	urchin_vc->_node->setPosition(init_state->_map->tileToMapCoords(start_pos.y, start_pos.x, grid_size));
	urchin_vc->_urchin_index = urchin_index;
	*/
	urchin_vc->_node = PolygonNode::allocWithTexture(texture);
	urchin_vc->_node->setScale(0.005f, 0.005f);
	//urchin_vc->_body->setGravityScale(-9.8f);
	urchin_vc->_display = display;
	return urchin_vc;
}

//Note that init physics must be called first
void UrchinViewController::setPhysicsPosition(float x, float y) {
	CULog("setting urchin physics");
	_body->setPosition(x, y);
	_node->setPosition(x, y);
}

void UrchinViewController::updateNodePosition() {
	CULog("updating urchin node position");
	_node->setPosition(_body->getPosition());
	_node->setAngle(_body->getAngle());
}

void UrchinViewController::initPhysics(shared_ptr<ObstacleWorld> world) {
	CULog("initializing urchin physics");
	_body = WheelObstacle::alloc(Vec2(_node->getPositionX(), _node->getPositionY()), _node->getWidth() / 2);
	_body->setBodyType(b2BodyType::b2_dynamicBody);
	_body->setLinearDamping(1.0f);
	_body->setName("urchin");
	world->addObstacle(_body);
}

