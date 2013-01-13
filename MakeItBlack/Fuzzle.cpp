// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#include "Fuzzle.h"

namespace {
	constexpr static float FUZZLE_SPEED_SEC = 20.f;
	constexpr static float FUZZLE_ENGAGE_SPEED_SEC = 30;
	constexpr static float FUZZLE_JUMP_SPEED_SEC = 85;
	constexpr static float FUZZLE_ENGAGE_JUMP_SPEED_SEC = 105;
}


EntityRef makeFuzzle(State & state, float startX, float startY) {
	auto ent = state.makeEntity<FuzzleDelegate>();
	ent->locX = startX;
	ent->locY = startY;
	return ent;
}


std::string FuzzleDelegate::type() const {
	return "fuzzle";
}

void FuzzleDelegate::init(Entity & me, State & state) {
	me.enemy = true;
	me.width = 2;
	me.height = 2;
}

TileIndex FuzzleDelegate::tileIndex(Entity & me, State & state) const {
	return { 38, 36 };
}

void FuzzleDelegate::act(Entity & me, State & state, const sf::Input & input) {
	float dpx = state.player->locX - me.locX;
	float dpy = state.player->locY - me.locY;
	float distToPlayer = std::sqrt((dpx*dpx) + (dpy*dpy));

	if (distToPlayer < 40.f) {
		action = Action::ENGAGE;
		me.lookLeft = me.locX > state.player->locX;
	}
	else {
		if (me.nextAction < state.timeNow) {
			if (state.random() > 0.6f) {
				action = Action::WANDER;
				me.lookLeft = (state.random() > 0.5);
			}
			else
				action = Action::IDLE;
			
			int msToNext = (action == Action::IDLE) ? 500 : (1000 + state.random() * 2000);
			me.nextAction = state.timeNow + std::chrono::milliseconds(msToNext);
		}
	}

	if (action == Action::IDLE)
		me.velX = 0.f;
	else {
		float speed = (action == Action::WANDER) ? FUZZLE_SPEED_SEC : FUZZLE_ENGAGE_SPEED_SEC;
		me.velX = (me.lookLeft) ? -speed : speed;
		
		if (me.isOnFloor() && movementBlocked) {
			movementBlocked = false;
			me.velY = (action == Action::WANDER) ? -FUZZLE_JUMP_SPEED_SEC : -FUZZLE_ENGAGE_JUMP_SPEED_SEC;
		}
	}
}

void FuzzleDelegate::collidedWithWall(Entity & me, State & state, const TileIndex & hitCoord) {
	movementBlocked = true;
}

void FuzzleDelegate::collidedWithEntity(Entity & me, State & state, Entity & other) {
	if (other.type() == "blob") {
		me.takeDamage(other.attackPower());
		if (! me.alive()) {
			me.removeMe = true;
		}
	}
}
