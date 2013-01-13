// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#ifndef __MakeItBlack__DarknessBlob__
#define __MakeItBlack__DarknessBlob__

#include "Entity.h"

EntityRef makeDarknessBlob(State & state, float startX, float startY, bool goingLeft, bool lowBeam);

class BlobDelegate : public EntityDelegate {
	static std::uniform_real_distribution<float> spewWidthVariance;
	static std::uniform_real_distribution<float> spewHeightVariance;
	static TimePoint lastSplat;

	bool lowBeam;
	
	void tarnish(State & state, int row, int col);
	
public:
	BlobDelegate(bool isLowBeam);

	std::string type() const override;
	void init(Entity & me, State & state) override;
	TileIndex tileIndex(Entity & me, State & state) const override;
	void act(Entity & me, State & state, const sf::Input & input) override;
	
	void collidedWithWall(Entity & me, State & state, const TileIndex & hitCoord) override;
	void collidedWithFloor(Entity & me, State & state, const TileIndex & hitCoord) override;
	void collidedWithCeiling(Entity & me, State & state, const TileIndex & hitCoord) override;
	void collidedWithEntity(Entity & me, State & state, Entity & other) override;
	
	float attackPower() const override { return 4.f; }
};

#endif /* defined(__MakeItBlack__DarknessBlob__) */
