// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#include "Map.h"
#include "View.h"


View::View(StateRef theState, std::shared_ptr<sf::RenderWindow> theWindow)
	: state(theState)
	, window(theWindow)
{
}

void View::load(const std::function<void ()> & done) {
	window->SetFramerateLimit(60);
	
	font.LoadFromFile("/Library/Fonts/Arial.ttf");
	spriteMap.LoadFromFile("data/tiles.png");
	spriteMap.SetSmooth(false);

	done();
}

sf::Vector2f View::scaleCoord(float x, float y) {
	return { x * VIEW_SCALE, y * VIEW_SCALE };
}

void View::drawMeters() {
	sf::String bileLabel { "Bile", font, 18.0f };
	bileLabel.SetPosition(30, 10);
	sf::String disgustLabel { "Disgust", font, 18.0f };
	disgustLabel.SetPosition(Globals::STAGE_W * VIEW_SCALE - 324.f, 10);

	sf::Color disgustColor { 0x49, 0xa2, 0x55 };

	// Bile Meter
	window->Draw(sf::Shape::Rectangle(24, 24, 24 + 306, 24 + 18, sf::Color::White));
	window->Draw(sf::Shape::Rectangle(27, 27, 27 + (state->bile * 3), 27 + 12, sf::Color::Black));
	window->Draw(bileLabel);
	bileLabel.Move(-1.f, -1.f);
	bileLabel.SetColor(sf::Color::Black);
	window->Draw(bileLabel);
	
	// Disgust Meter
	if (state->levelIndex > -1 && state->levelIndex < 3) {
		float left = (VIEW_SCALE * Globals::STAGE_W) - 306.f - 24.f;
		window->Draw(sf::Shape::Rectangle(left, 24, left + 306, 24 + 18, sf::Color::White));
		window->Draw(sf::Shape::Rectangle(left + 3, 27, left + 3 + (state->disgust * 3), 27 + 12, disgustColor));

		disgustLabel.SetColor(sf::Color::Black);
		window->Draw(disgustLabel);
		disgustLabel.Move(-1.f, -1.f);
		disgustLabel.SetColor(disgustColor);
		window->Draw(disgustLabel);
	}
}

void View::drawSprites() {
	
}

static sf::Color colerp(const sf::Color & from, const sf::Color & to, float ratio) {
	decltype(sf::Color::r) r, g, b;
	r = from.r + ((to.r - from.r) * ratio);
	g = from.g + ((to.g - from.g) * ratio);
	b = from.b + ((to.b - from.b) * ratio);
	return { r, g, b };
}

void View::drawBG() {
	sf::Color skyColor = colerp(sf::Color(67, 155, 248), sf::Color(0, 14, 48), state->completion);
	sf::Color lightColor = colerp(sf::Color(255, 246, 144), sf::Color(0,  8, 16), state->completion);

	window->Clear(skyColor);
	sf::Shape light;
	light.AddPoint(0, 0, skyColor);
	light.AddPoint(240, 0, lightColor);
	light.AddPoint(240, Globals::STAGE_H * VIEW_SCALE, lightColor);
	light.AddPoint(0, Globals::STAGE_H * VIEW_SCALE, skyColor);
	light.SetPosition((Globals::STAGE_W * VIEW_SCALE) - 240.f, 0);
	window->Draw(light);
	
	// BG tiles
	sf::Sprite ti;
	sf::IntRect tRect;
	ti.SetImage(spriteMap);
	ti.SetScale(VIEW_SCALE, VIEW_SCALE);
	ti.SetCenter(1.0f, -8.0f); // wut

	float tX, tY;

	for (int y=0; y < Globals::STAGE_H/8; y++) {
		auto tiles = state->map->layer(0)->rangeOnRow(y, state->cameraX / Globals::TILE_DIM, (Globals::STAGE_W / 8) + 1);
		tY = (float)y * Globals::TILE_DIM * VIEW_SCALE;
		tX = -((int)state->cameraX & (Globals::TILE_DIM - 1));

		for (auto it = tiles.first; it < tiles.second; ++it) {
			Tile tilex = *it;
			if (tilex) {
				tilex--;
				tRect.Left = (tilex & 7) * 8;
				tRect.Top = tilex & 0xf8;
				tRect.Right = tRect.Left + Globals::TILE_DIM;
				tRect.Top = tRect.Top + Globals::TILE_DIM;
				ti.SetSubRect(tRect);
				ti.SetPosition(tX, tY);
				window->Draw(ti);
			}
			
			tX += Globals::TILE_DIM * VIEW_SCALE;
		}
	}
}

void View::render() {
	state->frameCtr++;
	drawBG();
	drawSprites();
	drawMeters();

	sf::String text { std::to_string(window->GetFrameTime()), font, 48.0f };
	text.SetColor(sf::Color::White);
	text.SetPosition(scaleCoord(Globals::STAGE_W / 2.0f - 50.f, 30.0f));
	window->Draw(text);

	window->Display();
}

void View::levelChanged() {
	state->frameCtr = 0;
	state->cameraX = 0.0f;
	
	// build clouds
}
