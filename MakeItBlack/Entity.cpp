// MakeItBlack Native
// (c) 2013 by Arthur Langereis

#include <cmath>
#include <SFML/Window.hpp>
#include "Entity.h"
#include "Map.h"


// static incrementing id
int Entity::globalID = 0;

using namespace Globals;


Entity::Entity(std::string typeName, StateRef theState, std::unique_ptr<EntityDelegate> del)
	: type{ typeName }
	, state{ theState }
	, delegate{ std::move(del) }
{
	delegate->init(*this, state);
}

bool Entity::isOnFloor() {
	int locRow = std::floor(locY / TILE_DIM),
		locCol = std::floor((locX + 1.0f) / TILE_DIM),
		tileVOffset = (int)(std::round(locY)) & ((int)TILE_DIM - 1),
		tileHOffset = (int)(std::round(locX)) & ((int)TILE_DIM - 1);
	bool onFloor = false;
	
	// -- rather critical test
	if((tileVOffset == TILE_DIM - 1) && velY >= 0.0f) {
		if (state->map->layer(0)->tileAt(locRow + 1, locCol))
			onFloor = true;
		if ((tileHOffset > 0) && state->map->layer(0)->tileAt(locRow + 1, locCol + 1))
			onFloor = true;
	}
	
	return onFloor;
}

void Entity::move(float dt) {
	MapLayer & backLayer { *state->map->layer(0) };
	int locRow = std::floor(locY / TILE_DIM),
		locCol = std::floor((locX + 1.0f) / TILE_DIM),
		tileHOffset = ((int)std::round(locX)) & ((int)TILE_DIM - 1);
	
	float tryX = locX + (velX * dt),
		tryY = locY + (velY * dt);
	int testX = (int)(std::round(tryX)),
		testY = (int)(std::round(tryY)),
		testCol, testRow;
	Tile tileA, tileB;

	// -- normal movement is handled by the entity, but gravity is applied globally
	if (! isOnFloor())
		velY += GRAVITY_SEC * dt;
	
	// -- move and collide HORIZONTAL
	if (tryX != locX) {
		if (tryX < locX) {
			testCol = std::floor(testX / TILE_DIM);
			tileA = backLayer.tileAt(locRow, testCol);
			tileB = backLayer.tileAt(locRow - height + 1, testCol);
			
			if (tileA || tileB) {
				tryX = ((testCol + 1) * TILE_DIM);
				delegate->collidedWithWall(*this, state);
				velX = 0;
			}
		}
		else {
			testCol = std::floor((testX + (width * TILE_DIM)) / TILE_DIM);
			tileA = backLayer.tileAt(locRow, testCol);
			tileB = backLayer.tileAt(locRow - height + 1, testCol);
			
			if (tileA || tileB) {
				tryX = ((testCol - width) * TILE_DIM);
				delegate->collidedWithWall(*this, state);
				velX = 0;
			}
		}
		
		// -- update speed
		locX = tryX;
		testX = std::round(tryX);
		locCol = std::floor((tryX + 1) / TILE_DIM);
	}

	// -- move and collide VERTICAL
	if (tryY != locY) {
		if (tryY < locY) {
			testRow = std::floor((testY - (height * TILE_DIM)) / TILE_DIM);
			tileA = backLayer.tileAt(testRow, locCol);
			tileB = backLayer.tileAt(testRow, locCol + 1); // + intf.width
			
			if (tileA || (((tileHOffset > 0) || (width > 1.0f)) && tileB)) {
				tryY = ((testRow + height + 1) * TILE_DIM) - 1;
				delegate->collidedWithCeiling(*this, state);
				velY = 0;
			}
		}
		else {
			testRow = std::floor(testY / TILE_DIM);
			tileA = backLayer.tileAt(testRow, locCol);
			tileB = backLayer.tileAt(testRow, locCol + 1); // + intf.width
			
			if (tileA || (((tileHOffset > 0) || (width > 1.0f)) && tileB)) {
				tryY = (testRow * TILE_DIM) - 1;
				delegate->collidedWithFloor(*this, state);
				velY = 0;
			}
		}

		// -- update state
		locY = tryY;
		testY = std::round(tryY);
		locRow = std::floor(tryY / TILE_DIM);
	}
}

void Entity::checkEntityCollisions() {
	float axl = locX,
		axr = locX + (width * TILE_DIM),
		ayt = locY - (height * TILE_DIM),
		ayb = locY;
	
	for (auto other : state->entities) {
		if (other->id == id)
			continue;

		float bxl = other->locX,
			bxr = other->locX + (other->width * TILE_DIM),
			byt = other->locY - (other->height * TILE_DIM),
			byb = other->locY;

		if (axl < bxr && axr > bxl && ayt < byb && ayb > byt) {
			delegate->collidedWithEntity(*this, state, *other);
			other->collidedWithEntity(*other);
		}
	}
}

void Entity::collidedWithEntity(Entity & other) {
	delegate->collidedWithEntity(*this, state, other);
}

void Entity::act(float dt, const sf::Input & input) {
	delegate->act(*this, state, input);
	move(dt);
	checkEntityCollisions();
}
