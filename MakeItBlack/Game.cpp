// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#include <thread>
#include <string>
#include "Game.h"


Game::Game(StateRef theState, const sf::Input & theInput)
	: state(theState)
	, input(theInput)
{
}

void Game::load(const std::function<void()> & done) {
	done();
}

void Game::step(float dt) {
	state->completion = std::min(1.0, ((float)state->tarnishedTiles / state->exposedTiles) / 0.82); // need to cover 82% of exposed tiles to complete level
	
	if (input.IsKeyDown(sf::Key::Left)) {
		state->cameraX = std::max(0.0f, state->cameraX - (dt * 100.f));
	}

	if (input.IsKeyDown(sf::Key::Right)) {
		state->cameraX += dt * 100.f;
	}
}

void Game::loadLevel(int index, const std::function<void()> & done) {
	state->map.reset(new Map(std::string { "data/level" } + std::to_string(index) + ".tmx" ));
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
		
		done();
	});
}
