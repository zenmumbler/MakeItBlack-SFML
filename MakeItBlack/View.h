// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#ifndef __MakeItBlack__View__
#define __MakeItBlack__View__

#include <memory>
#include <functional>

#include <SFML/Graphics.hpp>

#include "State.h"


class View {
	StateRef state;
	std::shared_ptr<sf::RenderWindow> window;
	
	sf::Font font;
	
	inline sf::Vector2f scaleCoord(float x, float y);
	
public:
	constexpr static float VIEW_SCALE = 3.0f;
	
	View(StateRef theState, std::shared_ptr<sf::RenderWindow> theWindow);
	void load(const std::function<void()> & done);
	
	void render();
};

#endif /* defined(__MakeItBlack__View__) */
