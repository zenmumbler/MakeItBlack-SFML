// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#include "Perishable.h"


EntityRef makePerishable(State & state, float startX, float startY, Tile tilex) {
	auto ent = state.makeEntity<PerishableDelegate>(tilex);
	ent->locX = startX;
	ent->locY = startY;
	return ent;
}


PerishableDelegate::PerishableDelegate(Tile newTilex)
	: tilex(newTilex)
{
}

std::string PerishableDelegate::type() const {
	return "perishable";
}

void PerishableDelegate::init(Entity & me, State & state) {
}

TileIndex PerishableDelegate::tileIndex(Entity & me, State & state) const {
	return { tilex, tilex };
}

void PerishableDelegate::act(Entity & me, State & state, const sf::Input & input) {
	if (tilex < 16) {
		if (floorRow == -1) {
			floorRow = (me.locY + 2.f) / Globals::TILE_DIM;
			floorCol = (me.locX + 2.f) / Globals::TILE_DIM;
		}
		
		Tile floorTilex = state.map->layer(0)->tileAt(floorRow, floorCol);
		if (floorTilex >= 16) { // corrupted
			if (state.random() > 0.99) {
				me.HP = 0;
				tilex += 16; // wither
			}
		}
	}
}

void PerishableDelegate::collidedWithEntity(Entity & me, State & state, Entity & other) {
	if (tilex < 16 && other.type() == "blob") {
		me.HP = 0;
		tilex += 16;
	}
}
