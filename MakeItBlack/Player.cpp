// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#include "Player.h"
#include "DarknessBlob.h"
#include <SFML/Window.hpp>

namespace {
	constexpr static float PLAYER_SPEED_SEC = 60.0f;
	constexpr static float PLAYER_JUMP_SPEED_SEC = 100.f;
	constexpr static float PLAYER_HIT_SPEED_BOOST = 1000.f;
}

EntityRef makePlayer(State & state, float startX, float startY) {
	EntityRef player = state.makeEntity<PlayerDelegate>();
	player->locX = startX;
	player->locY = startY;
	return player;
}

std::string PlayerDelegate::type() const {
	return "player";
}

void PlayerDelegate::init(Entity & me, State & state) {
	me.width = 1;
	me.height = 2;
}

TileIndex PlayerDelegate::tileIndex(Entity & me, State & state) const {
	if (me.lookLeft) {
		if (me.velY < 0)
			return { 50, 50 };
		if (me.velX)
			return  { 54, 52 };
		if (glarbl)
			return { 48, 48 };
		return { 54, 54 };
	}
	else {
		if (me.velY < 0)
			return { 51, 51 };
		if (me.velX)
			return { 55, 53 };
		if (glarbl)
			return { 49, 49 };
		return { 55, 55 };
	}
}

void PlayerDelegate::act(Entity & me, State & state, const sf::Input & input) {
	if (! me.alive()) {
		me.velX = me.velY = 0.f;
		return;
	}

	if (input.IsKeyDown(sf::Key::Left)) {
		walkSpeed = -PLAYER_SPEED_SEC;
		me.lookLeft = true;
	}
	else if (input.IsKeyDown(sf::Key::Right)) {
		walkSpeed = PLAYER_SPEED_SEC;
		me.lookLeft = false;
	}
	else
		walkSpeed = 0.f;

	hitSpeed *= 0.6f;
	if (hitSpeed < 10.f) hitSpeed = 0.f;
	me.velX = walkSpeed + hitSpeed;
	
	if (me.isOnFloor() && input.IsKeyDown(sf::Key::Up)) {
		me.velY = -PLAYER_JUMP_SPEED_SEC;
	}
	
	if (input.IsKeyDown(sf::Key::Space)) {
		if (state.bile <= 0)
			return;
		
		float blOff = me.lookLeft ? -2.f : 1.f;
		bool lowBeam = input.IsKeyDown(sf::Key::Down);
		makeDarknessBlob(state, me.locX + blOff, me.locY - 7.f, me.lookLeft, lowBeam);
		
		glarbl = true;
		state.bile = std::max(0.f, state.bile - 1.5f);
	}
	else {
		glarbl = false;
		if (state.frameCtr & 1)
			state.bile = std::min(100.0f, state.bile + 1.5f);
	}
}


void PlayerDelegate::collidedWithWall(Entity & me, State & state, const TileIndex & hitCoord) {}

void PlayerDelegate::collidedWithFloor(Entity & me, State & state, const TileIndex & hitCoord) {
}

void PlayerDelegate::collidedWithCeiling(Entity & me, State & state, const TileIndex & hitCoord) {}

void PlayerDelegate::collidedWithEntity(Entity & me, State & state, Entity & other) {
	if (me.alive() && (! me.invulnerable) && other.enemy && other.alive()) {
		me.takeDamage(other.attackPower());
		state.disgust = 100.f - me.HP;

		if (! me.alive()) { // this last hit killed us
			state.timeOfDeath = state.timeNow;
		}
		else {
			hitSpeed = me.locX > other.locX ? PLAYER_HIT_SPEED_BOOST : -PLAYER_HIT_SPEED_BOOST;
			me.invulnerableFor(2000);
		}
	}
}
