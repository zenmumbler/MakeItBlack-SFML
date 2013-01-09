// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#include <memory>
#include <thread>
#include <SFML/System.hpp>

#include "State.h"
#include "Game.h"
#include "View.h"


// ----------------------------------------------------------------------------
//                  _
//  _ __ ___   __ _(_)_ __
// | '_ ` _ \ / _` | | '_ \
// | | | | | | (_| | | | | |
// |_| |_| |_|\__,_|_|_| |_|
//
// ----------------------------------------------------------------------------
class MakeItBlack {
	StateRef state { std::make_shared<State>() };
	std::unique_ptr<Game> game;
	std::unique_ptr<View> view;
	
	std::shared_ptr<sf::RenderWindow> window;

	void mainLoop();
	void handleEvents();
	
	bool running = false;
	
	TimePoint lastStep, lastRender;
	
public:
	MakeItBlack() = default;
	void run();
};


void MakeItBlack::handleEvents() {
	sf::Event event;

	while (window->GetEvent(event)) {
		if (event.Type == sf::Event::Closed)
			running = false;

		if (event.Type == sf::Event::KeyPressed) {
			if (event.Key.Code == sf::Key::Escape)
				running = false;
		}
	}
}

void MakeItBlack::mainLoop() {
	using namespace std::chrono;
	
	int timeSinceStep, timeSinceRender;

	while (running) {
		handleEvents();

		// -- timers to limit game logic and rendering rate
		state->timeNow = high_resolution_clock::now();
		timeSinceStep = static_cast<int>(duration_cast<milliseconds>(state->timeNow - lastStep).count());
		timeSinceRender = static_cast<int>(duration_cast<milliseconds>(state->timeNow - lastRender).count());
		
		if (state->phase == GamePhase::LOADNEXT) {
			state->levelIndex++;
			game->startLevel(state->levelIndex, [this, &timeSinceStep, &timeSinceRender]{
				state->timeNow = lastStep = lastRender = std::chrono::high_resolution_clock::now();
				timeSinceStep = timeSinceRender = 0;
				state->phase = GamePhase::PLAY;
			});
		}

		if (state->phase == GamePhase::PLAY || state->phase == GamePhase::FADEOUT) {
			if (timeSinceStep > 4) {
				if (timeSinceStep > 20)	// limit slowdown
					timeSinceStep = 20;

				game->step(static_cast<float>(timeSinceStep) / 1000.0f);	// time in seconds
				lastStep = state->timeNow;
			}
		}

		if (state->phase != GamePhase::LOADNEXT) {
			if (timeSinceRender >= 8) { // max 125fps
				view->render();
				lastRender = state->timeNow;
			}
		}
	}
}

void MakeItBlack::run() {
	// asinine initialization sequence because of SFML's decision to
	// make Window the Application, essentially
	int viewW = Globals::STAGE_W * View::VIEW_SCALE,
		viewH = Globals::STAGE_H * View::VIEW_SCALE;
	
	window.reset(new sf::RenderWindow(sf::VideoMode(viewW, viewH, 32), "Make It Black", sf::Style::Close));
	
	game.reset(new Game(state, window->GetInput()));
	view.reset(new View(state, window));

	game->load([this] {
		view->load([this] {
			running = true;
			state->phase = GamePhase::LOADNEXT;
			lastStep = lastRender = std::chrono::high_resolution_clock::now();
			mainLoop();
		});
	});
}


int main(int argc, const char * argv[]) {
	MakeItBlack mib;
	mib.run();
}
