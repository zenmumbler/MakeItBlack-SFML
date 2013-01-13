// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#ifndef __MakeItBlack__Fuzzle__
#define __MakeItBlack__Fuzzle__

#include <string>
#include "Entity.h"
#include "Map.h"

EntityRef makeFuzzle(State & state, float startX, float startY);

class FuzzleDelegate : public EntityDelegate {
	enum class Action : int {
		IDLE, WANDER, ENGAGE
	};
	Action action = Action::IDLE;
	bool movementBlocked = false;

public:
	virtual std::string type() const override;
	virtual void init(Entity & me, State & state) override;
	virtual TileIndex tileIndex(Entity & me, State & state) const override;
	virtual void act(Entity & me, State & state, const sf::Input & input) override;
	
	virtual void collidedWithWall(Entity & me, State & state, const TileIndex & hitCoord) override;
	virtual void collidedWithEntity(Entity & me, State & state, Entity & other) override;
	
	virtual float attackPower() const override { return 16.f; }
};


#endif /* defined(__MakeItBlack__Fuzzle__) */
