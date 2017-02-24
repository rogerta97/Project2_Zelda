#ifndef _ENTITY_
#define _ENTITY_

#include "j1App.h"
#include "j1Render.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Physics.h"
#include "Animation.h"

class b2Fixture;
class PhysBody;
class GameObject;

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

struct Ability
{
	Ability(float _cd, float _duration)
	{
		cd = _cd; duration = _duration; cd = _cd; ;
	};

	float cd = 0;
	float duration = 0;
	b2Fixture* fixture = nullptr;
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

	virtual void BasicAttackUp() {};
	virtual void BasicAttackDown() {};
	virtual void BasicAttackLeft() {};
	virtual void BasicAttackRight() {};

	virtual void Ability1() {};
	virtual void Ability2() {};
	virtual void Ability3() {};

	bool GotHit(Entity* &entity, Ability* &ability)
	{
		if (hit)
		{
			entity = hit_by;
			ability = hit_ability;
			hit_by = nullptr;
			hit_ability = nullptr;
			hit = false;

			return true;
		}
		return false;
	};

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

	//Set Team if not set already
	void SetTeam(uint _team) 
	{
		if (team == 0) {
			team = _team;
		}
	}

	//Return entity team. 0 if neutral
	uint GetTeam() const
	{
		return team;
	}

private:
	uint		team = 0;

public:
	GameObject*     game_object = nullptr;
	Stats	        stats;
	vector<Ability*> abilities;
	bool		    can_move = false;
	bool            attacking = false;
	bool		    hit = false;
	Entity*	        hit_by = nullptr;
	Ability*		hit_ability = nullptr;

protected:
	iPoint      draw_offset = NULLPOINT;
	iPoint		restore_draw_offset = NULLPOINT;
};

#endif