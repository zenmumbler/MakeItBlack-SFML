// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#include "Heart.h"


EntityRef makeHeart(State & state, float startX, float startY) {
	auto ent = state.makeEntity<HeartDelegate>();
	ent->locX = startX;
	ent->locY = startY;
	return ent;
}


std::string HeartDelegate::type() const {
	return "heart";
}

void HeartDelegate::init(Entity & me, State & state) {
	me.enemy = true;
}

TileIndex HeartDelegate::tileIndex(Entity & me, State & state) const {
	if (me.alive())
		return { 8, 15 };
	return { 24, 24 };
}

void HeartDelegate::act(Entity & me, State & state, const sf::Input & input) {
	if (me.alive() && state.completion == 1.0f) {
		if (floorRow == -1) {
			floorRow = (me.locY + 2.f) / Globals::TILE_DIM;
			floorCol = (me.locX + 2.f) / Globals::TILE_DIM;
		}
		
		Tile floorTilex = state.map->layer(0)->tileAt(floorRow, floorCol);
		if (floorTilex < 16) { // normal
			if (state.random() > 0.98)
				state.map->layer(0)->setTileAt(floorRow, floorCol, floorTilex + 16);
		}
		else { // corrupted
			if (state.random() > 0.98)
				me.HP = 0;
		}
	}
}

void HeartDelegate::collidedWithEntity(Entity & me, State & state, Entity & other) {
}
