// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#ifndef __MakeItBlack__View__
#define __MakeItBlack__View__

#include <memory>
#include <functional>

#include <SFML/Graphics.hpp>

#include "State.h"

class Cloud {
	State & state;
	float baseX, baseY;
	int period;
	
	struct Seg {
		float cx, cy, rad;
		int alpha;
	};
	
	std::vector<Seg> segs;

	void makeSeg();

public:
	Cloud(State & sta, float locX);
	void draw(sf::RenderWindow & window);
};


class View {
	StateRef state;
	std::shared_ptr<sf::RenderWindow> window;
	
	sf::Font font;
	sf::Image spriteMap;
	std::vector<Cloud> clouds;
	
	void buildClouds();
	inline sf::Vector2f scaleCoord(float x, float y);
	void drawClouds();
	void drawMeters();
	void drawSprites();
	void drawBG();
	
public:
	constexpr static float VIEW_SCALE = 3.0f;
	
	View(StateRef theState, std::shared_ptr<sf::RenderWindow> theWindow);
	void load(const std::function<void()> & done);
	
	void render();
	void drawDimmer(float alpha);
	void levelChanged();
};

#endif /* defined(__MakeItBlack__View__) */
