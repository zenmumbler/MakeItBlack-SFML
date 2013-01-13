// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#ifndef __MakeItBlack__Game__
#define __MakeItBlack__Game__

#include <functional>
#include <memory>

#include <SFML/Window.hpp>

#include "State.h"
#include "Map.h"
#include "Entity.h"


class Game {
	StateRef state;
	const sf::Input & input;
	bool messageA = false, messageB = false, messageC = false;
	
	void loadLevel(int index, const std::function<void()> & done);
	void moveCamera();
	void checkMessages();
	
public:
	Game(StateRef theState, const sf::Input & theInput);

	void load(const std::function<void()> & done);
	
	void step(float dt);
	
	void startLevel(int index, const std::function<void()> & done);
};


#endif /* defined(__MakeItBlack__Game__) */
