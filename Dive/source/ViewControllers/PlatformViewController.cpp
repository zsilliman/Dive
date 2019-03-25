#include "PlatformViewController.h"

void PlatformViewController::draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) {}

void PlatformViewController::update(shared_ptr<GameState> state) {
	//Set positions/rotations of original platform according to physics
    shared_ptr<Platform> platform = state->_map->getPlatforms()[_platform_index];
	if (platform->getPosition().y != platform->getStartPosition().y)
		platform->setPosition(platform->getPosition().x, platform->getStartPosition().y);
    _oc_node->setPosition(platform->getPosition() * _grid_size - _node->getPosition());
    _oc_node->setAngle(platform->getAngle());

	//Set positions/rotations of duplicate platform according to duplicate physics object
	shared_ptr<Platform> platform_dup = state->_map->getPlatformDups()[_platform_index];
	if (platform_dup->getPosition().y != platform_dup->getStartPosition().y)
		platform_dup->setPosition(platform_dup->getPosition().x, platform_dup->getStartPosition().y);
	_cp_node->setPosition(platform_dup->getPosition() * _grid_size - _node->getPosition());
	_cp_node->setAngle(platform_dup->getAngle());
}

void PlatformViewController::dispose() {

}

void PlatformViewController::reset() {

}

shared_ptr<PlatformViewController> PlatformViewController::alloc(shared_ptr<GameState> init_state, shared_ptr<TiledTexture> tilesheet, float grid_size, int platform_index) {
	shared_ptr<PlatformViewController> p = make_shared<PlatformViewController>();
	shared_ptr<Platform> platform = init_state->_map->getPlatforms()[platform_index];
	p->_grid_size = grid_size;
	p->_platform_index = platform_index;
	//Iterate over every tile
	Vec2 min_corner = platform->getMinCorner();
	p->_node = Node::allocWithPosition(min_corner * grid_size);
	
	p->_oc_node = Node::allocWithPosition(min_corner * grid_size);
	p->_cp_node = Node::allocWithPosition(min_corner * grid_size);
	for (int i = 0; i < platform->adj_tiles.size(); i++) {
		int block = platform->adj_values[i]-1; //subtract 1 so it can be indexed into the tilesheet
		if (block >= 0) { //This if statement should always be true
			shared_ptr<Texture> subtexture = tilesheet->getTile(block);
			shared_ptr<PolygonNode> oc_node = PolygonNode::allocWithTexture(subtexture);
			oc_node->setScale(grid_size/subtexture->getWidth());
			oc_node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
			Vec2 position = (platform->adj_tiles[i] - min_corner) * grid_size;
			oc_node->setPosition(position);
			p->_nodes.push_back(oc_node);
			p->_oc_node->addChild(oc_node, 1);

			shared_ptr<PolygonNode> cp_node = PolygonNode::allocWithTexture(subtexture);
			cp_node->setScale(grid_size / subtexture->getWidth());
			cp_node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
			cp_node->setPosition(Vec2(position));
			p->_nodes.push_back(cp_node);
			p->_cp_node->addChild(cp_node, 1);
		}
	}
	p->_node->addChild(p->_oc_node, 1);
	p->_node->addChild(p->_cp_node, 1);
	return p;
}
