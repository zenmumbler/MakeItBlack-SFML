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
	
	void loadLevel(int index, const std::function<void()> & done);
	void moveCamera();
	
public:
	Game(StateRef theState, const sf::Input & theInput);

	template <typename E>
	EntityRef makeEntity(const std::string & type);
	
	void load(const std::function<void()> & done);
	
	void step(float dt);
	
	void startLevel(int index, const std::function<void()> & done);
};


template<typename E>
EntityRef Game::makeEntity(const std::string & type) {
	std::unique_ptr<EntityDelegate> delegate { new E() };
	EntityRef ent = std::make_shared<Entity>(type, state, std::move(delegate));
	state->entities.push_back(ent);
	return ent;
}


#endif /* defined(__MakeItBlack__Game__) */
