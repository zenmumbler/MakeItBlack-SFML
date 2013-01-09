// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#ifndef __MakeItBlack__State__
#define __MakeItBlack__State__

#include <memory>
#include <chrono>

class Map;

class Globals {
	Globals() = delete;

public:
	constexpr static int STAGE_W = 320;
	constexpr static int STAGE_H = 192;
	constexpr static int TILE_DIM = 8;
};


using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;


enum class GamePhase : int {
	LOADNEXT,
	START,
	FADEIN,
	PLAY,
	MESSAGE,
	END,
	FADEOUT
};


class State {
public:
	GamePhase phase = GamePhase::LOADNEXT;
	int levelIndex = -1;
	TimePoint timeNow, fadeTime;
	
	int bile, disgust;
	
	std::unique_ptr<Map> map;
	int exposedTiles, tarnishedTiles;
	float completion;
	TimePoint completionTime;
	
	int frameCtr = 0;
	float cameraX;
	
	State() = default;
};

using StateRef = std::shared_ptr<State>;


#endif /* defined(__MakeItBlack__State__) */
