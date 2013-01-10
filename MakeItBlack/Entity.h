// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#ifndef __MakeItBlack__Entity__
#define __MakeItBlack__Entity__

#include <string>
#include <memory>
#include "State.h"

namespace sf { class Input; }
class EntityDelegate;

class Entity {
	static int globalID;
	StateRef state;
	std::unique_ptr<EntityDelegate> delegate;

	void move(float dt);
	void checkEntityCollisions();

public:
	int id = ++Entity::globalID;
	std::string type;
	float locX = 0, locY = 0;
	float velX = 0, velY = 0;
	float width = 1, height = 1;
	bool lookLeft = false;
	bool removeMe = false;
	TimePoint nextAction;

	Entity(std::string type, StateRef theState, std::unique_ptr<EntityDelegate> delegate);
	
	void act(float dt, const sf::Input & input);
	void collidedWithEntity(Entity & other);
	bool isOnFloor();
};


class EntityDelegate {
public:
	virtual ~EntityDelegate() = default;
	
	virtual void init(Entity & me, StateRef state) = 0;
	virtual void tileIndex(Entity & me, StateRef state) = 0;
	virtual void act(Entity & me, StateRef state, const sf::Input & input) = 0;

	virtual void collidedWithWall(Entity & me, StateRef state) {}
	virtual void collidedWithFloor(Entity & me, StateRef state) {}
	virtual void collidedWithCeiling(Entity & me, StateRef state) {}
	virtual void collidedWithEntity(Entity & me, StateRef state, Entity & other) {}
};


#endif /* defined(__MakeItBlack__Entity__) */
