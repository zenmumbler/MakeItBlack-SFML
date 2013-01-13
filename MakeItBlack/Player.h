// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#ifndef __MakeItBlack__Player__
#define __MakeItBlack__Player__

#include "Entity.h"

EntityRef makePlayer(State & state, float startX, float startY);


class PlayerDelegate : public EntityDelegate {
	bool glarbl = false;
	float walkSpeed = 0.0f, hitSpeed = 0.0f;

public:
	std::string type() const override;
	void init(Entity & me, State & state) override;
	TileIndex tileIndex(Entity & me, State & state) const override;
	void act(Entity & me, State & state, const sf::Input & input) override;
	
	void collidedWithWall(Entity & me, State & state, const TileIndex & hitCoord) override;
	void collidedWithFloor(Entity & me, State & state, const TileIndex & hitCoord) override;
	void collidedWithEntity(Entity & me, State & state, Entity & other) override;
};

#endif /* defined(__MakeItBlack__Player__) */
