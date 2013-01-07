// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#ifndef __MakeItBlack__Game__
#define __MakeItBlack__Game__

#include <functional>
#include <memory>
#include "State.h"
#include "Map.h"


class Game {
	StateRef state;
	std::unique_ptr<Map> map;
	
	void loadLevel(int index, const std::function<void()> & done);
	
public:
	Game(StateRef theState);
	
	void load(const std::function<void()> & done);
	
	void step(double dt);
	
	void startLevel(int index, const std::function<void()> & done);
};


#endif /* defined(__MakeItBlack__Game__) */
