// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#include "Pet.h"
#include "Sound.h"

namespace {
	constexpr static float PET_SPEED_SEC = 15.f;
	constexpr static float PET_JUMP_SPEED_SEC = 85.f;
}


EntityRef makePet(State & state, float startX, float startY) {
	auto ent = state.makeEntity<PetDelegate>();
	ent->locX = startX;
	ent->locY = startY;
	return ent;
}


std::string PetDelegate::type() const {
	return "pet";
}

void PetDelegate::init(Entity & me, State & state) {
	me.width = 2;
}

TileIndex PetDelegate::tileIndex(Entity & me, State & state) const {
	if (me.lookLeft)
		return { 72, 74 };
	else
		return { 80, 82 };

}

void PetDelegate::act(Entity & me, State & state, const sf::Input & input) {
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
	
	if (action == Action::IDLE)
		me.velX = 0.f;
	else {
		me.velX = me.lookLeft ? -PET_SPEED_SEC : PET_SPEED_SEC;
		if (me.isOnFloor() && state.random() > 0.98f)
			me.velY = -PET_JUMP_SPEED_SEC;
	}
	
	if (state.random() > 0.991f && state.player->locX > 300.f) {
		state.sound->play("pet");
	}
}
