#ifndef _ENTITY_
#define _ENTITY_

#include "j1App.h"
#include "j1Render.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Physics.h"
#include "Animation.h"

class b2Fixture;
class PhysBody;

enum states
{
	run_up,
	run_left,
	run_down,
	run_right,

	walk_up,
	walk_left,
	walk_down,
	walk_right,

	idle_up,
	idle_left,
	idle_down,
	idle_right,

	basic_atack_up,
	basic_atack_left,
	basic_atack_down,
	basic_atack_right,

	states_null,
};

enum movement
{
	move_up,
	move_down,
	move_left,
	move_right,

	move_upright,
	move_downright,
	move_upleft,
	move_downleft,

	stop,
};

class Stats
{
public:
	Stats() {};
	~Stats() {};

public:
	int speed = 0;
};

class Entity
{
public:
	Entity() {};

	virtual ~Entity() {};

	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool Draw(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };

	// Characters
	virtual void MoveUp(float speed) {};
	virtual void MoveDown(float speed) {};
	virtual void MoveLeft(float speed) {};
	virtual void MoveRight(float speed) {};

	virtual void MoveUpRight(float speed) {};
	virtual void MoveDownRight(float speed) {};
	virtual void MoveUpLeft(float speed) {};
	virtual void MoveDownLeft(float speed) {};

	virtual void RunUp() {};
	virtual void RunDown() {};
	virtual void RunLeft() {};
	virtual void RunRight() {}

	virtual void IdleUp() {};
	virtual void IdleDown() {};
	virtual void IdleLeft() {};
	virtual void IdleRight() {};

	virtual void SetCamera(int index) {};

	virtual iPoint GetPos() const { return{ 0,0 }; }

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

	virtual void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB) {};

public:
	Stats	    stats;

};

#endif