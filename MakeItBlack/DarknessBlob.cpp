// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#include "DarknessBlob.h"
#include "Map.h"
#include "Sound.h"

namespace {
	constexpr static float BLOB_HORIZ_SPEED = 90;
	constexpr static float BLOB_VERT_SPEED = -60;
	constexpr static float BLOB_VERT_SPEED_LOW = 15;
}

std::uniform_real_distribution<float> BlobDelegate::spewWidthVariance { 0.f, 0.5f };
std::uniform_real_distribution<float> BlobDelegate::spewHeightVariance { 0.f, 0.3f };
TimePoint BlobDelegate::lastSplat;


EntityRef makeDarknessBlob(State & state, float startX, float startY, bool goingLeft, bool lowBeam) {
	EntityRef blob = state.makeEntity<BlobDelegate>(lowBeam);
	blob->locX = startX;
	blob->locY = startY;
	blob->lookLeft = goingLeft;

	return blob;
}


BlobDelegate::BlobDelegate(bool isLowBeam)
	: lowBeam(isLowBeam)
{}

std::string BlobDelegate::type() const {
	return "blob";
}

void BlobDelegate::init(Entity & me, State & state) {
	float baseSpeed = lowBeam ? BLOB_VERT_SPEED_LOW : BLOB_VERT_SPEED;
	me.velY = (0.85 * baseSpeed) + (spewHeightVariance(state.randEngine) * baseSpeed);
}

TileIndex BlobDelegate::tileIndex(Entity & me, State & state) const {
	return { 32, 32 };
}

void BlobDelegate::act(Entity & me, State & state, const sf::Input & input) {
	if (me.velX == 0.0f) {
		if (me.lookLeft)
			me.velX = (1.25 * -BLOB_HORIZ_SPEED) + (spewWidthVariance(state.randEngine) * BLOB_HORIZ_SPEED);
		else
			me.velX = (0.75 *  BLOB_HORIZ_SPEED) + (spewWidthVariance(state.randEngine) * BLOB_HORIZ_SPEED);
	}
}


void BlobDelegate::tarnish(State & state, int row, int col) {
	MapLayer & layer = *state.map->layer(0);

	auto tilex = layer.tileAt(row, col);
	if (tilex < 16) {
		layer.setTileAt(row, col, tilex + 16);
		state.tarnishedTiles++;
	}

	auto msSinceLast = std::chrono::duration_cast<std::chrono::milliseconds>(state.timeNow - lastSplat).count();
	if (msSinceLast > 50) {
		lastSplat = state.timeNow;
		state.sound->play("splat");
	}
}

void BlobDelegate::collidedWithWall(Entity & me, State & state, const TileIndex & hitCoord) {
	if (me.lookLeft)
		tarnish(state, hitCoord.first, hitCoord.second);
	else
		tarnish(state, hitCoord.first, hitCoord.second);
	me.removeMe = true;
}

void BlobDelegate::collidedWithFloor(Entity & me, State & state, const TileIndex & hitCoord) {
	tarnish(state, hitCoord.first, hitCoord.second);
	me.removeMe = true;
}

void BlobDelegate::collidedWithCeiling(Entity & me, State & state, const TileIndex & hitCoord) {
	tarnish(state, hitCoord.first, hitCoord.second);
	me.removeMe = true;
}

void BlobDelegate::collidedWithEntity(Entity & me, State & state, Entity & other) {
	auto otherType = other.type();
	if (otherType != "blob" && otherType != "player" && other.alive()) {
		me.removeMe = true;

		auto msSinceLast = std::chrono::duration_cast<std::chrono::milliseconds>(state.timeNow - lastSplat).count();
		if (msSinceLast > 50) {
			lastSplat = state.timeNow;
			state.sound->play("splat");
		}
	}
}
