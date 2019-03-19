#include "UrchinViewController.h"
#include "Util.h"

void UrchinViewController::draw(shared_ptr<SpriteBatch> batch, shared_ptr<GameState> state) {}

void UrchinViewController::update(shared_ptr<GameState> state) {
    state->_urchins[_urchin_index]->rotateEntity(state->_map->getMapRect());
    
    //new version:
    
    _oc_node->setPosition(state->_urchins[_urchin_index]->_box->getPosition() * _grid_size);
    _oc_node->setAngle(state->_urchins[_urchin_index]->_box->getAngle());
    
    //Set positions/rotations of duplicate according to duplicate physics object
    _dup_node->setPosition(state->_urchins[_urchin_index]->_box_dup->getPosition() * _grid_size);
    _dup_node->setAngle(state->_urchins[_urchin_index]->_box_dup->getAngle());
}


void UrchinViewController::dispose() {}

void UrchinViewController::reset() {
    
}

shared_ptr<UrchinViewController> UrchinViewController::alloc(shared_ptr<GameState> init_state, shared_ptr<Texture> texture, Size display, int urchin_index) {
    shared_ptr<UrchinViewController> urchin_vc = make_shared<UrchinViewController>();
    urchin_vc->_urchin_index = urchin_index;
    urchin_vc->_grid_size = display.width / init_state->_map->getWidth();
    urchin_vc->_node = Node::allocWithPosition(Vec2(0, 0));
    urchin_vc->_oc_node = PolygonNode::allocWithTexture(texture);
    urchin_vc->_oc_node->setPosition(init_state->_urchins[urchin_index]->getPosition());
    urchin_vc->_oc_node->setScale(urchin_vc->_grid_size / texture->getWidth(), urchin_vc->_grid_size / texture->getHeight());
    urchin_vc->_dup_node = PolygonNode::allocWithTexture(texture);
    urchin_vc->_dup_node->setScale(urchin_vc->_grid_size / texture->getWidth(), urchin_vc->_grid_size / texture->getHeight());
    urchin_vc->_dup_node->setPosition(init_state->_urchins[urchin_index]->getPosition());
    urchin_vc->_display = display;
    
    urchin_vc->_node->addChild(urchin_vc->_oc_node);
    urchin_vc->_node->addChild(urchin_vc->_dup_node);
    
    return urchin_vc;
}
