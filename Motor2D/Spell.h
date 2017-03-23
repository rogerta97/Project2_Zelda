#ifndef _SPELL_
#define _SPELL_

#include "j1App.h"
#include "j1Render.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Physics.h"
#include "Animation.h"
#include "Entity.h"

class b2Fixture;
class PhysBody;
class GameObject;
class Entity;

struct personal_stats
{
	float damage_multiplicator = 0.0f;
	float slow_duration = 0.0f;
	float slow_multiplicator = 0.0f;
	float stun_duration = 0.0f;
};

class Spell
{
public:
	Spell() {};

	virtual ~Spell() {};

	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool Draw(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };

	void CleanSpell();

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

	virtual void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB) {};
	virtual void OnCollEnter(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB) {};
	virtual void OnCollOut(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB) {};

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

private:
	uint		     team = 0;

public:
	// Containers
	GameObject*      game_object = nullptr;
	Entity*			 owner = nullptr;
	Entity*			 target = nullptr;
	b2Fixture*		 hit_box = nullptr;

	// Delete 
	bool			 to_delete = false;
	bool			can_delete = false;

	// Name
	string			 name;

	personal_stats   stats;
protected:
	// Draw
	iPoint           draw_offset = NULLPOINT;
	iPoint		     restore_draw_offset = NULLPOINT;

};

#endif // _SPELL_