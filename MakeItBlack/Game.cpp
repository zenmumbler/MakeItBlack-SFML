// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#include <thread>
#include <string>
#include <utility>
#include "Game.h"

#include "Player.h"


Game::Game(StateRef theState, const sf::Input & theInput)
	: state(theState)
	, input(theInput)
{
}


void Game::load(const std::function<void()> & done) {
	done();
}

void Game::moveCamera() {
	using namespace Globals;

	if ((state->player->locX - state->cameraX) > STAGE_W / 2) {
		state->cameraX = std::min(((float)state->map->getWidth() * TILE_DIM) - STAGE_W - 1.0f, state->player->locX - (STAGE_W / 2));
	}
	if ((state->player->locX - state->cameraX) < STAGE_W / 2) {
		state->cameraX = std::max(0.0f, state->player->locX - (STAGE_W / 2));
	}
}

void Game::step(float dt) {
	state->completion = std::min(1.0, ((float)state->tarnishedTiles / state->exposedTiles) / 0.82); // need to cover 82% of exposed tiles to complete level

	for (auto ent : state->entities)
		ent->act(dt, input);

	moveCamera();
}

void Game::loadLevel(int index, const std::function<void()> & done) {
	state->map.reset(new Map("data/level" + std::to_string(index) + ".tmx"));
	done();
}

void Game::startLevel(int index, const std::function<void()> & done) {
	state->bile = 100;
	state->disgust = 0;
	
	loadLevel(index, [this, index, done]{
		state->exposedTiles = state->map->layer(0)->countExposedTiles();
		state->tarnishedTiles = 0;
		state->completion = 0;
		state->completionTime = TimePoint::min();

		switch (index) {
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
				
			default: break;
		}
		
		auto player = makePlayer(*this, 20, 10);
		state->player = player.get(); // poor man's weak_ptr

		done();
	});
}

