// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#include "View.h"


View::View(StateRef theState, std::shared_ptr<sf::RenderWindow> theWindow)
	: state(theState)
	, window(theWindow)
{
}

void View::load(const std::function<void ()> & done) {
	window->SetFramerateLimit(60);
	
	font.LoadFromFile("/Library/Fonts/Arial.ttf");

	done();
}

sf::Vector2f View::scaleCoord(float x, float y) {
	return { x * VIEW_SCALE, y * VIEW_SCALE };
}

void View::render() {
	window->Clear();

	sf::String text { "This is a test", font, 48.0f };
	text.SetColor(sf::Color::White);
	text.SetPosition(scaleCoord(Globals::STAGE_W / 2.0f - 50.f, 30.0f));
	window->Draw(text);

	window->Display();
}
