// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#ifndef __MakeItBlack__Perishable__
#define __MakeItBlack__Perishable__

#include <string>
#include "Entity.h"
#include "Map.h"

EntityRef makePerishable(State & state, float startX, float startY, Tile tilex);

class PerishableDelegate : public EntityDelegate {
	Tile tilex;
	int floorRow = -1, floorCol = -1;

public:
	PerishableDelegate(Tile tilex);

	virtual std::string type() const override;
	virtual void init(Entity & me, State & state) override;
	virtual TileIndex tileIndex(Entity & me, State & state) const override;
	virtual void act(Entity & me, State & state, const sf::Input & input) override;
	
	virtual void collidedWithEntity(Entity & me, State & state, Entity & other) override;
};

#endif /* defined(__MakeItBlack__Perishable__) */
