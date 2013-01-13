// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#include <sstream>
#include <cmath>
#include "Map.h"
#include "View.h"
#include "Entity.h"

using namespace Globals;


void Cloud::makeSeg() {
	Seg s;
	s.cx = baseX - 45.f + (90.f * state.random());
	s.cy = baseY - 18.f + (36.f * state.random());
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
	bileLabel.SetPosition(30, 8);
	sf::String disgustLabel { "Disgust", font, 18.0f };
	disgustLabel.SetPosition(STAGE_W * VIEW_SCALE - 324.f, 8);

	sf::Color disgustColor { 0x49, 0xa2, 0x55 };

	// Bile Meter
	auto border = sf::Shape::Rectangle(24, 24, 24 + 306, 24 + 18, sf::Color::White, 3.f, sf::Color::White);
	border.EnableFill(false);
	window->Draw(border);
	window->Draw(sf::Shape::Rectangle(27, 27, 27 + (state->bile * 3), 27 + 12, sf::Color::Black));
	window->Draw(bileLabel);
	bileLabel.Move(-1.f, -1.f);
	bileLabel.SetColor(sf::Color::Black);
	window->Draw(bileLabel);
	
	// Disgust Meter
	if (state->levelIndex > -1 && state->levelIndex < 3) {
		float left = (VIEW_SCALE * STAGE_W) - 306.f - 24.f;
		border = sf::Shape::Rectangle(left, 24, left + 306, 24 + 18, sf::Color::White, 3.f, sf::Color::White);
		border.EnableFill(false);
		window->Draw(border);
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
		if (ent->invulnerable && (state->frameCtr & 1))
			continue;

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
		if (ent->type() == "fuzzle") {
			int ealp = (ent->HP / 100.f) * 255.f;
			sp.SetColor({ 255, 255, 255, ealp });
		}
		else
			sp.SetColor(sf::Color::White);
		
		if (ent->type() == "player" && !ent->alive()) {
			float scale = (1.f + (state->deathRatio * 4.f)) * VIEW_SCALE;

			sp.SetRotation(-25.f + state->random() * 50.f);
			sp.SetScale(scale, scale);
			sp.SetColor({ 255, 255, 255, 50.f + 180.f * state->random() });
		}

		window->Draw(sp);
	}
}


void View::drawTextBox(const std::string & title, const std::string & message) {
	// x: 150, y: 120, w: 660, h: 300, infl: 100
	sf::Shape box;
	sf::Color gray(50, 50, 50);
	box.AddPoint(0  ,  0, sf::Color::Black);
	box.AddPoint(660,  0, sf::Color::Black);
	box.AddPoint(660,300, gray);
	box.AddPoint(0  ,300, gray);
	box.SetPosition(150.f, 120.f);
	window->Draw(box);

	sf::Shape border = sf::Shape::Rectangle(0.f, 0.f, 648.f, 288.f, sf::Color::White, 3.f, { 0xcc, 0xcc, 0xcc });
	border.EnableFill(false);
	border.SetPosition(156.f, 126.f);
	window->Draw(border);

	sf::String ttl { title, font, 30.f };
	ttl.SetColor({ 0xac, 0x16, 0x02 });
	ttl.SetCenter(ttl.GetRect().GetWidth() / 2, ttl.GetRect().GetHeight());
	ttl.SetPosition(480, 165);
	window->Draw(ttl);

	std::istringstream iss { message };
	float lineY = 210.f;
	while (!iss.eof()) {
		std::string line;
		std::getline(iss, line);
		
		sf::String slin { line, font, 24.f };
		slin.SetColor(sf::Color::White);
		slin.SetCenter(slin.GetRect().GetWidth() / 2, slin.GetRect().GetHeight());
		slin.SetPosition(480, lineY);
		lineY += 30.f;
		window->Draw(slin);
	}

	sf::String prtc { "-- press return to continue --", font, 15.f };
	prtc.SetColor({ 0xaa, 0xaa, 0xaa });
	prtc.SetCenter(prtc.GetRect().GetWidth() / 2, prtc.GetRect().GetHeight());
	prtc.SetPosition(480, 405);
	window->Draw(prtc);
}


void View::drawLevelComplete() {
	std::string title;
	auto msSinceDone = std::chrono::duration_cast<std::chrono::milliseconds>(state->timeNow - state->completionTime).count();
	float slideRatio = std::min(1.0f, msSinceDone / 1500.f);
	slideRatio *= slideRatio;

	if (state->levelIndex == 3)
		title = "All Clear";
	else
		title = "Level Complete";

	std::vector<std::string> subs = {
		"Didn't that feel good?", "Mick Jagger is proud of you.", "Your salivary glands must be HUGE.",
		"Here's your damn goat."
	};
	auto subtitle = subs[state->levelIndex];

	sf::String ttl { title, font, 90 };
	ttl.SetCenter(ttl.GetRect().GetWidth() / 2.f, ttl.GetRect().GetHeight());
	ttl.SetPosition(480.f, -60.f + (300.f * slideRatio));
	window->Draw(ttl);

	if (slideRatio == 1.0f) {
		sf::String sttl { subtitle, font, 30 };
		sttl.SetColor({ 0x49, 0xa2, 0x55 });
		sttl.SetCenter(sttl.GetRect().GetWidth() / 2.f, sttl.GetRect().GetHeight());
		sttl.SetPosition(480.f, 300.f);
		window->Draw(sttl);

		if (state->levelIndex < 3 && (state->frameCtr & 32)) {
			sf::String prtc { "-- press return to proceed --", font, 24 };
			prtc.SetCenter(prtc.GetRect().GetWidth() / 2.f, prtc.GetRect().GetHeight());
			prtc.SetPosition(480.f, 540.f);
			window->Draw(prtc);
		}
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
	
	if (state->phase == GamePhase::MESSAGE) {
		drawTextBox(state->msgTitle, state->msgText);
		
		if (window->GetInput().IsKeyDown(sf::Key::Return))
			state->phase = GamePhase::PLAY;
	}

	if (state->completion == 1.0f)
		drawLevelComplete();

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
