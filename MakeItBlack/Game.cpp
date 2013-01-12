// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#include <thread>
#include <string>
#include <utility>
#include <algorithm>
#include "Game.h"

#include "Player.h"
#include "DarknessBlob.h"
#include "Perishable.h"
#include "Heart.h"

using namespace Globals;


Game::Game(StateRef theState, const sf::Input & theInput)
	: state(theState)
	, input(theInput)
{
	// this prevents a crash during play because
	// I add entities (blobs) to the entities vector while iterating over it
	// bleugh
	state->entities.reserve(200);
}


void Game::load(const std::function<void()> & done) {
	done();
}

void Game::checkMessages() {
	
}

void Game::moveCamera() {
	if ((state->player->locX - state->cameraX) > STAGE_W / 2) {
		state->cameraX = std::min(((float)state->map->getWidth() * TILE_DIM) - STAGE_W - 1.0f, state->player->locX - (STAGE_W / 2));
	}
	if ((state->player->locX - state->cameraX) < STAGE_W / 2) {
		state->cameraX = std::max(0.0f, state->player->locX - (STAGE_W / 2));
	}
}

void Game::step(float dt) {
	// need to cover 82% of exposed tiles to complete level
	state->completion = std::min(1.0, ((float)state->tarnishedTiles / state->exposedTiles) / 0.82);

	// remove entities from play that have their removeMe flag set
	state->entities.erase(
		std::remove_if(begin(state->entities), end(state->entities), [](const EntityRef & ent){ return ent->removeMe; }),
		end(state->entities)
	);

	for (EntityRef & ent : state->entities) {
		ent->act(dt, input);
	}

	moveCamera();
	
	checkMessages();
	
	if (state->completion == 1.0f) {
		if (state->completionTime > state->timeNow) // completionTime is set to time::max initially
			state->completionTime = state->timeNow;
		
		if (state->phase == GamePhase::PLAY && state->levelIndex < 3 && input.IsKeyDown(sf::Key::Return))
			state->phase = GamePhase::END;
	}
}

void Game::loadLevel(int index, const std::function<void()> & done) {
	state->map.reset(new Map("data/level" + std::to_string(index) + ".tmx"));
	done();
}

void Game::startLevel(int index, const std::function<void()> & done) {
	state->bile = 100;
	state->disgust = 0;
	
	loadLevel(index, [=]{
		state->exposedTiles = state->map->layer(0)->countExposedTiles();
		state->tarnishedTiles = 0;
		state->completion = 0;
		state->completionTime = TimePoint::max();
		
		state->entities.clear();
		
		state->map->layer(1)->eachTile([=](int row, int col, Tile tilex) {
			if (tilex == 9) { // heart
				state->exposedTiles--;
				makeHeart(*state, col * TILE_DIM, (row + 1) * TILE_DIM - 1);
			}
			else
				makePerishable(*state, col * TILE_DIM, (row + 1) * TILE_DIM - 1, tilex - 1);
		});

		switch (index) {
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
				
			default: break;
		}
		
		auto player = makePlayer(*state, 20, 10);
		state->player = player.get(); // poor man's weak_ptr

		done();
	});
}
