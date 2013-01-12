// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#ifndef __MakeItBlack__Entity__
#define __MakeItBlack__Entity__

#include <string>
#include <memory>
#include "State.h"

namespace sf { class Input; }
class EntityDelegate;

using TileIndex = std::pair<int, int>;


class Entity {
	static int globalID;
	State & state;
	std::unique_ptr<EntityDelegate> delegate;

	void move(float dt);
	void checkEntityCollisions();

public:
	int id = ++Entity::globalID;
	float locX = 0, locY = 0;
	float velX = 0, velY = 0;
	float width = 1, height = 1;
	bool lookLeft = false;
	bool removeMe = false;
	TimePoint nextAction;

	bool enemy = false;
	int HP = 100;

	Entity(State & theState, std::unique_ptr<EntityDelegate> delegate);
	Entity(const Entity &) = delete;
	Entity& operator =(const Entity &) = delete;
	
	void act(float dt, const sf::Input & input);
	void collidedWithEntity(Entity & other);
	bool isOnFloor();
	TileIndex tileIndex();
	std::string type();
	bool alive() { return HP > 0; }
};


class EntityDelegate {
public:
	virtual ~EntityDelegate() = default;
	
	virtual std::string type() const = 0;
	virtual void init(Entity & me, State & state) = 0;
	virtual TileIndex tileIndex(Entity & me, State & state) const = 0;
	virtual void act(Entity & me, State & state, const sf::Input & input) = 0;

	virtual void collidedWithWall(Entity & me, State & state, const TileIndex & hitCoord) {}
	virtual void collidedWithFloor(Entity & me, State & state, const TileIndex & hitCoord) {}
	virtual void collidedWithCeiling(Entity & me, State & state, const TileIndex & hitCoord) {}
	virtual void collidedWithEntity(Entity & me, State & state, Entity & other) {}
};


#endif /* defined(__MakeItBlack__Entity__) */
