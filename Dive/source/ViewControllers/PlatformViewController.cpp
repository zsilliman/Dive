#include "PlatformViewController.h"

void PlatformViewController::draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) {}

void PlatformViewController::update(shared_ptr<GameState> state) {
	_node->setPosition(_platform->getPosition() * _grid_size);
	_node->setAngle(_platform->getAngle());
	CULog(_platform->getPosition().toString().c_str());
}

void PlatformViewController::dispose() {

}

void PlatformViewController::reset() {

}

shared_ptr<PlatformViewController> PlatformViewController::alloc(shared_ptr<Platform> platform, shared_ptr<TiledTexture> tilesheet, float grid_size) {
	shared_ptr<PlatformViewController> p = make_shared<PlatformViewController>();
	p->_grid_size = grid_size;
	p->_platform = platform;
	//Iterate over every tile
	Vec2 min_corner = platform->getMinCorner();
	p->_node = Node::allocWithPosition(min_corner * grid_size);
	for (int i = 0; i < platform->adj_tiles.size(); i++) {
		int block = platform->adj_values[i]-1; //subtract 1 so it can be indexed into the tilesheet
		if (block >= 0) { //This if statement should always be true
			shared_ptr<Texture> subtexture = tilesheet->getTile(block);
			shared_ptr<PolygonNode> node = PolygonNode::allocWithTexture(subtexture);
			node->setScale(grid_size/subtexture->getWidth());
			node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
			Vec2 position = (platform->adj_tiles[i] - min_corner) * grid_size;
			node->setPosition(position);
			p->_nodes.push_back(node);
			p->_node->addChild(node, 1);
			CULog("Success Creating Platform Nodes");
			CULog(std::to_string(block).c_str());
			CULog(std::to_string(grid_size).c_str());
			CULog(position.toString().c_str());
		}
		else {
			CULog("Issue with getting block data correctly");
			CULog(std::to_string(block).c_str());
		}
	}
	return p;
}