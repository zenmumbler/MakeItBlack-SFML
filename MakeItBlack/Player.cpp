// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#include "Player.h"
#include "Game.h"

namespace {
	constexpr static float PLAYER_SPEED_SEC = 60.0f;
	constexpr static float PLAYER_JUMP_SPEED_SEC = 100.f;
	constexpr static float PLAYER_HIT_SPEED_BOOST = 1000.f;
}

EntityRef makePlayer(Game & game, float startX, float startY) {
	EntityRef player = game.makeEntity<PlayerEntity>("player");
	player->locX = startX;
	player->locY = startY;
	return player;
}

void PlayerEntity::init(Entity & me, StateRef state) {
	me.height = 2;
}

void PlayerEntity::tileIndex(Entity & me, StateRef state) {
	
}

void PlayerEntity::act(Entity & me, StateRef state, const sf::Input & input) {
	if (input.IsKeyDown(sf::Key::Left)) {
		me.velX = -PLAYER_SPEED_SEC;
	}
	else if (input.IsKeyDown(sf::Key::Right)) {
		me.velX = PLAYER_SPEED_SEC;
	}
	else
		me.velX = 0.f;

	if (me.isOnFloor() && input.IsKeyDown(sf::Key::Up)) {
		me.velY = -PLAYER_JUMP_SPEED_SEC;
	}
}


void PlayerEntity::collidedWithWall(Entity & me, StateRef state) {}

void PlayerEntity::collidedWithFloor(Entity & me, StateRef state) {
}

void PlayerEntity::collidedWithCeiling(Entity & me, StateRef state) {}

void PlayerEntity::collidedWithEntity(Entity & me, StateRef state, Entity & other) {
	
}
