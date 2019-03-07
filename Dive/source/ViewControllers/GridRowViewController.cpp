#include "GridRowViewController.h"

void GridRowViewController::draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) {

}

void GridRowViewController::update(shared_ptr<GameState> state) {
	//Update textures of nodes
	for (int col = 0; col < _blocks.size(); col++) {
		int block = state->_map->getBlock(_row_index, col);
		if (block >= 0) {
			//Set node to the new texture
			shared_ptr<Texture> tex = _tilesheet->getTile(block);
			_blocks[col]->setTexture(tex);
			_blocks[col]->setVisible(true);
		}
		else {
			//Make node invisible
			_blocks[col]->setVisible(false);
		}
	}
}

void GridRowViewController::dispose() {

}

void GridRowViewController::reset() {

}

void GridRowViewController::setTileSet(shared_ptr<TiledTexture> tilesheet) {
	_tilesheet = tilesheet;
}

void GridRowViewController::init(shared_ptr<GameState> init_state, int row, float grid_size) {
	int cols = init_state->_map->getColumnCount();
	int rows = init_state->_map->getRowCount();
	int tile_width = init_state->_map->getTileWidth();
	int tile_height = init_state->_map->getTileHeight();

	for (int col = 0; col < cols; col++) {
		int block = init_state->_map->getBlock(row, col);
		//Non-empty block
		//Create the corresponding node in the scene
		shared_ptr<PolygonNode> block_node;
		if (block >= 0) {
			shared_ptr<Texture> tile_texture = _tilesheet->getTile(block);
			block_node = PolygonNode::allocWithTexture(tile_texture);
		}
		else {
			shared_ptr<Texture> default_tex = _tilesheet->getTile(0);
			block_node = PolygonNode::allocWithTexture(default_tex);
			block_node->setVisible(false);
		}
		block_node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
		float x = init_state->_map->tileToMapCoords(row, col, grid_size).x;
		float scalex = grid_size / tile_width;
		float scaley = grid_size / tile_height;
		block_node->setPosition(x, 0);
		block_node->setScale(scalex, scaley);
		_blocks.push_back(block_node);
		_node->addChild(block_node);
	}
}

shared_ptr<GridRowViewController> GridRowViewController::alloc(shared_ptr<GameState> init_state, shared_ptr<TiledTexture> tilesheet, int row_index, float grid_size) {
	shared_ptr<GridRowViewController> row = make_shared<GridRowViewController>();
	row->setTileSet(tilesheet);
	row->_grid_size = grid_size;
	row->_row_index = row_index;
	row->_node = Node::alloc();
	row->init(init_state, row_index, grid_size);
	return row;
}