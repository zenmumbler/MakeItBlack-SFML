// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#ifndef __MakeItBlack__State__
#define __MakeItBlack__State__

#include <memory>
#include <chrono>
#include <vector>
#include <utility>
#include <random>


class Map;
class Entity;
class EntityDelegate;
using EntityRef = std::shared_ptr<Entity>;


namespace Globals {
	constexpr static int STAGE_W = 320;
	constexpr static int STAGE_H = 192;
	constexpr static int TILE_DIM = 8;
	
	constexpr static int GRAVITY_SEC = 300;
}


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
	
	float bile, disgust;
	
	std::unique_ptr<Map> map;
	int exposedTiles, tarnishedTiles;
	float completion;
	TimePoint completionTime;
	
	std::vector<EntityRef> entities;
	Entity * player = nullptr;

	int frameCtr = 0;
	float cameraX;
	
	std::mt19937 randEngine;
	std::uniform_real_distribution<float> normalDistribution { 0.0f, 1.0f };
	float random() { return normalDistribution(randEngine); }
	
	template <typename E, typename ...Args>
	EntityRef makeEntity(Args... args);

	template <typename E>
	EntityRef makeEntity();
};

using StateRef = std::shared_ptr<State>;


template<typename E, typename ...Args>
EntityRef State::makeEntity(Args... args) {
	std::unique_ptr<EntityDelegate> delegate { new E(std::forward<Args...>(args...)) };
	EntityRef ent = std::make_shared<Entity>(*this, std::move(delegate));
	entities.push_back(ent);
	return std::move(ent);
}

template<typename E>
EntityRef State::makeEntity() {
	std::unique_ptr<EntityDelegate> delegate { new E() };
	EntityRef ent = std::make_shared<Entity>(*this, std::move(delegate));
	entities.push_back(ent);
	return std::move(ent);
}


#endif /* defined(__MakeItBlack__State__) */
