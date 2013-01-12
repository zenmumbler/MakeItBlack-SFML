// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#ifndef __MakeItBlack__Heart__
#define __MakeItBlack__Heart__

#include <string>
#include "Entity.h"
#include "Map.h"

EntityRef makeHeart(State & state, float startX, float startY);

class HeartDelegate : public EntityDelegate {
	int floorRow = -1, floorCol = -1;
	
public:
	virtual std::string type() const override;
	virtual void init(Entity & me, State & state) override;
	virtual TileIndex tileIndex(Entity & me, State & state) const override;
	virtual void act(Entity & me, State & state, const sf::Input & input) override;
	
	virtual void collidedWithEntity(Entity & me, State & state, Entity & other) override;
};


#endif /* defined(__MakeItBlack__Heart__) */
