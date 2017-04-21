#pragma once

#ifndef  _BUSHES_H_
#define _BUSHES_H_

#include "Entity.h"
#include "AestheticsManager.h"

class Bush : public Entity
{
public:

	Bush(iPoint pos);

	// Draw and change animations
	bool Draw(float dt);

	void SetBushColor(const char* color); 
	void SetMiddle(bush_color type);

	// CleanUp
	bool CleanUp();

	~Bush();

private:
	void OnCollEnter(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);
	void OnCollOut(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

private:
	bool is_middle = false; 
	bool is_moving = false; 
};

#endif