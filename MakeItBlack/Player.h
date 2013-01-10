// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#ifndef __MakeItBlack__Player__
#define __MakeItBlack__Player__

#include "Entity.h"

class Game;

EntityRef makePlayer(Game & game, float startX, float startY);


class PlayerEntity : public EntityDelegate {
public:
	void init(Entity & me, StateRef state) override;
	void tileIndex(Entity & me, StateRef state) override;
	void act(Entity & me, StateRef state, const sf::Input & input) override;
	
	void collidedWithWall(Entity & me, StateRef state) override;
	void collidedWithFloor(Entity & me, StateRef state) override;
	void collidedWithCeiling(Entity & me, StateRef state) override;
	void collidedWithEntity(Entity & me, StateRef state, Entity & other) override;
};

#endif /* defined(__MakeItBlack__Player__) */
