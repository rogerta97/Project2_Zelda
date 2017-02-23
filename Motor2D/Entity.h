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

struct AbilityCds
{
	AbilityCds(float _basic_attack_cd = 0, float _basic_attack_duration = 0, float _ability1_cd = 0, float _ability1_duration = 0, 
		float _ability2_cd = 0, float _ability2_duration = 0, float _ability3_cd = 0, float _ability3_duration = 0)
	{
		basic_attack_cd = _basic_attack_cd; basic_attack_duration = _basic_attack_duration; ability1_cd = _ability1_cd; ability1_duration = _ability1_duration; 
		ability2_cd = _ability2_cd; ability2_duration = _ability2_duration; ability3_cd = _ability3_cd; ability3_duration = _ability3_duration;
	};

	float basic_attack_cd = 0;
	float basic_attack_duration = 0;

	float ability1_cd = 0;
	float ability1_duration = 0;

	float ability2_cd = 0;
	float ability2_duration = 0;

	float ability3_cd = 0;
	float ability3_duration = 0;
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
	Stats	    stats;
	AbilityCds  cds;
	bool		can_move = false;
	bool        attacking = false;

	iPoint      draw_offset = NULLPOINT;
	iPoint		restore_draw_offset = NULLPOINT;
};

#endif