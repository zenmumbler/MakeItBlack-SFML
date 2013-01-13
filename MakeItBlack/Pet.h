// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#ifndef __MakeItBlack__Pet__
#define __MakeItBlack__Pet__

#include <string>
#include "Entity.h"
#include "Map.h"

EntityRef makePet(State & state, float startX, float startY);

class PetDelegate : public EntityDelegate {
	enum class Action : int {
		IDLE, WANDER
	};
	Action action = Action::IDLE;
	
public:
	virtual std::string type() const override;
	virtual void init(Entity & me, State & state) override;
	virtual TileIndex tileIndex(Entity & me, State & state) const override;
	virtual void act(Entity & me, State & state, const sf::Input & input) override;
};


#endif /* defined(__MakeItBlack__Pet__) */
