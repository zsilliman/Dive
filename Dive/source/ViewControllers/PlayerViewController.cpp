#include "PlayerViewController.h"
#include "Util.h"

void PlayerViewController::draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) {}

void PlayerViewController::update(shared_ptr<GameState> state) {
	state->_player->rotateEntity(state->_map->getMapRect());
	
    //new version:
	state->_player->setLinearVelocity(Vec2(1, -1));
    
	_oc_node->setPosition(state->_player->_box->getPosition() * _grid_size);
	_oc_node->setAngle(state->_player->_box->getAngle());

	//Set positions/rotations of duplicate according to duplicate physics object
	_dup_node->setPosition(state->_player->_box_dup->getPosition() * _grid_size);
	_dup_node->setAngle(state->_player->_box_dup->getAngle());

	//old version:
	/*Vec2 start_pos = state->_player->getPosition();
	//CULog("start_pos x, y: %d %d", start_pos.x, start_pos.y);
	if (isBlocked(state)) {
		CULog("should move but won't");
		state->_player->move(Direction::RIGHT, state->_map->getColumnCount());
	}

	else if (canMove(state, Direction::DOWN)) {
		fall(state);
	}
	else {
		int row = start_pos.y;
		//CULog("current row %d row count %d", row, state->_map->getRowCount());
		if (canMove(state, state->_player->getCurrentDirection()))
		{
			state->_player->move(state->_player->getCurrentDirection(), state->_map->getColumnCount());
		}
	}
	Vec2 tile_pos = state->_player->getPosition();
	float grid_size = _display.width / state->_map->getColumnCount();
	Vec2 map_pos = state->_map->tileToMapCoords(tile_pos.y, tile_pos.x, grid_size);
	_node->setPosition(map_pos);
	_node->setVisible(true);*/
}


void PlayerViewController::dispose() {}

void PlayerViewController::reset() {

}

shared_ptr<PlayerViewController> PlayerViewController::alloc(shared_ptr<GameState> init_state, shared_ptr<Texture> texture, Size display) {
	shared_ptr<PlayerViewController> player_vc = make_shared<PlayerViewController>();
	player_vc->_grid_size = display.width / init_state->_map->getWidth();
	player_vc->_node = Node::allocWithPosition(Vec2(0, 0));
	player_vc->_oc_node = PolygonNode::allocWithTexture(texture);
	player_vc->_oc_node->setPosition(init_state->_player->getPosition());
	player_vc->_oc_node->setScale(player_vc->_grid_size / texture->getWidth(), player_vc->_grid_size / texture->getHeight());
	player_vc->_dup_node = PolygonNode::allocWithTexture(texture);
	player_vc->_dup_node->setScale(player_vc->_grid_size / texture->getWidth(), player_vc->_grid_size / texture->getHeight());
	player_vc->_dup_node->setPosition(init_state->_player->getPosition());
    player_vc->_display = display;

	player_vc->_node->addChild(player_vc->_oc_node);
	player_vc->_node->addChild(player_vc->_dup_node);

	return player_vc;
}
