// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#include "Map.h"
#include "View.h"
#include "Entity.h"
#include <cmath>

using namespace Globals;


void Cloud::makeSeg() {
	Seg s;
	s.cx = baseX - 45.f + (90.f * state.random());
	s.cy = baseY - 45.f + (90.f * state.random());
	s.rad = 15 + (15.f * state.random());
	s.alpha = 38 + (20 * state.random());
	segs.push_back(s);
}

Cloud::Cloud(State & sta, float locX)
	: state(sta)
	, baseX(locX)
{
	baseY = 48.f + (state.random() * 48.f);
	period = 3000 + std::round(state.random() * 4000);
	
	int segCount = 9 + (state.random() * 7);
	for (int i=0; i<segCount; i++)
		makeSeg();
}

void Cloud::draw(sf::RenderWindow & window) {
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(state.timeNow.time_since_epoch()).count();
	float tau = 2 * 3.1415926535;
	float cycle = tau * ((float)(ms % period) / period);
	float rScale = 1.f + (0.1 * std::sin(cycle));
	for (auto & seg : segs) {
		sf::Shape shp = sf::Shape::Circle(seg.cx, seg.cy, seg.rad * rScale, sf::Color(255,255,255,seg.alpha));
		window.Draw(shp);
	}
}



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


void View::buildClouds() {
	float x = -10.f,
		endX = (state->map->getWidth() * TILE_DIM * VIEW_SCALE) + 10.f;

	clouds.clear();
	
	while (x < endX) {
		clouds.emplace_back(*state, x);
		x += 45.f + (120.f * state->random());
	}
}


sf::Vector2f View::scaleCoord(float x, float y) {
	return { x * VIEW_SCALE, y * VIEW_SCALE };
}


void View::drawClouds() {
	for (auto & cloud : clouds)
		cloud.draw(*window);
}


void View::drawMeters() {
	sf::String bileLabel { "Bile", font, 18.0f };
	bileLabel.SetPosition(30, 10);
	sf::String disgustLabel { "Disgust", font, 18.0f };
	disgustLabel.SetPosition(STAGE_W * VIEW_SCALE - 324.f, 10);

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
		float left = (VIEW_SCALE * STAGE_W) - 306.f - 24.f;
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
	using namespace Globals;

	sf::Sprite sp;
	sf::IntRect tRect;
	sp.SetImage(spriteMap);
	sp.SetScale(VIEW_SCALE, VIEW_SCALE);

	for (auto ent : state->entities) {
		float pixWidth = ent->width * TILE_DIM;
		float pixHeight  = ent->height * TILE_DIM;

		TileIndex ti = ent->tileIndex();
		int tilex = ((state->frameCtr / 30) & 1) ? ti.second : ti.first;

		float left = std::round(ent->locX - state->cameraX) * VIEW_SCALE;
		float top = std::round(ent->locY - pixHeight + 1.f) * VIEW_SCALE;
		
		tRect.Left = (tilex & 7) * 8;
		tRect.Top = tilex & 0xf8;
		tRect.Right = tRect.Left + pixWidth;
		tRect.Bottom = tRect.Top + pixHeight;

		sp.SetPosition(left, top);
		sp.SetSubRect(tRect);

		window->Draw(sp);
	}
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
	light.AddPoint(240, STAGE_H * VIEW_SCALE, lightColor);
	light.AddPoint(0, STAGE_H * VIEW_SCALE, skyColor);
	light.SetPosition((STAGE_W * VIEW_SCALE) - 240.f, 0);
	window->Draw(light);
	
	// BG tiles
	sf::Sprite ti;
	sf::IntRect tRect;
	ti.SetImage(spriteMap);
	ti.SetScale(VIEW_SCALE, VIEW_SCALE);

	float tX, tY;

	for (int y=0; y < STAGE_H/8; y++) {
		auto tiles = state->map->layer(0)->rangeOnRow(y, state->cameraX / TILE_DIM, (STAGE_W / 8) + 1);
		tY = (float)y * TILE_DIM * VIEW_SCALE;
		tX = -((int)state->cameraX & (TILE_DIM - 1)) * VIEW_SCALE;

		for (auto it = tiles.first; it < tiles.second; ++it) {
			Tile tilex = *it;
			if (tilex) {
				tilex--;
				tRect.Left = (tilex & 7) * 8;
				tRect.Top = tilex & 0xf8;
				tRect.Right = tRect.Left + TILE_DIM;
				tRect.Bottom = tRect.Top + TILE_DIM;
				ti.SetSubRect(tRect);
				ti.SetPosition(tX, tY);
				window->Draw(ti);
			}
			
			tX += TILE_DIM * VIEW_SCALE;
		}
	}
}


void View::drawDimmer(float alpha) {
	int intAlpha = alpha * 255.f;
	auto shp = sf::Shape::Rectangle(0.f, 0.f, STAGE_W * VIEW_SCALE, STAGE_H * VIEW_SCALE, sf::Color(0, 0, 0, intAlpha));
	window->Draw(shp);
}


void View::render() {
	state->frameCtr++;
	drawBG();
	drawSprites();
	drawClouds();
	drawMeters();

//	sf::String text { std::to_string(window->GetFrameTime()), font, 48.0f };
//	text.SetColor(sf::Color::White);
//	text.SetPosition(scaleCoord(STAGE_W / 2.0f - 50.f, 30.0f));
//	window->Draw(text);
}


void View::levelChanged() {
	state->frameCtr = 0;
	state->cameraX = 0.0f;
	
	buildClouds();
}
