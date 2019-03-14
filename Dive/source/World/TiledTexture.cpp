#include "TiledTexture.h"

TiledTexture::TiledTexture()
{
	_set = false;
}


TiledTexture::~TiledTexture()
{
	//_tiles.clear();
	//_texture->dispose();
}

shared_ptr<Texture> TiledTexture::getTile(int i) {
	return _tiles[i];
}

void TiledTexture::setTexture(shared_ptr<Texture> texture, int tile_width, int tile_height) {
	_texture = texture;
	_tile_width = tile_width;
	_tile_height = tile_height;
	_columns = (int)(texture->getWidth()/tile_width);
	_rows = (int)(texture->getHeight()/tile_height);

	for (int i = 0; i < _rows; i++) {
		for (int j = 0; j < _columns; j++) {
			float x = j * _tile_width;
			float y = i * _tile_height;
			float minS = x/texture->getWidth();
			float maxS = (x + _tile_width)/texture->getWidth();
			float minT = y/texture->getHeight();
			float maxT = (y + _tile_height)/texture->getHeight();
			_tiles.push_back(_texture->getSubTexture(minS, maxS, minT, maxT));
		}
	}
	_set = true;
}

shared_ptr<TiledTexture> TiledTexture::alloc(shared_ptr<Texture> texture, int tile_width, int tile_height) {
	shared_ptr<TiledTexture> tex = make_shared<TiledTexture>();
	tex->setTexture(texture, tile_width, tile_height);
	return tex;
}