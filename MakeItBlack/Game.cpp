// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#include <thread>
#include <string>
#include "Game.h"

Game::Game(StateRef theState)
	: state(theState)
{
}

void Game::load(const std::function<void()> & done) {
	done();
}

void Game::step(double dt) {
	
}

void Game::loadLevel(int index, const std::function<void()> & done) {
	map.reset(new Map(std::string { "data/level" } + std::to_string(index) + ".tmx" ));
	done();
}

void Game::startLevel(int index, const std::function<void()> & done) {
	state->bile = 100;
	state->disgust = 0;
	
	loadLevel(index, [this, index, done]{
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
