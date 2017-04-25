#ifndef _BOOMERANG_
#define _BOOMERANG_

#include "j1App.h"
#include "j1Render.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Physics.h"
#include "Animation.h"
#include "Spell.h"

class b2Fixture;
class PhysBody;
class GameObject;
class Spell;

#define BOOMERANG_RANGE 170

enum direction
{
	null,
	up,
	down,
	left,
	right,
};

class Boomerang : public Spell
{
public:
	Boomerang(iPoint pos);

	virtual ~Boomerang();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool Draw(float dt);
	bool PostUpdate();
	bool CleanUp();

	void CleanSpell();

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

	void Set(direction dir);

	void Effects(Entity * spell_owner, Entity* reciever, Ability * ability);

private:
	direction dir = direction::null;
	float			initial_speed = 0;
	iPoint			starting_pos = NULLPOINT;

	j1Timer			timer;

	float			damage_multiplicator_first = 0.0f;
	float			damage_multiplicator_second = 0.0f;
	float			damage_multiplicator_return = 0.0f;

};

#endif // _BOOMERANG_