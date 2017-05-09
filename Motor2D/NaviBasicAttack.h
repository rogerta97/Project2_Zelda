#ifndef _NAVI_BASIC_ATTACK_
#define _NAVI_BASIC_ATTACK_

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

enum n_b_a_direction
{
	n_b_a_null,
	n_b_a_up,
	n_b_a_down,
	n_b_a_left,
	n_b_a_right,
};

class NaviBasicAttack : public Spell
{
public:
	NaviBasicAttack(iPoint pos);

	virtual ~NaviBasicAttack();

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

	void OnCollEnter(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

	void Set(n_b_a_direction dir);

	void Effects(Entity* entity, Ability * ability);

private:
	n_b_a_direction dir = n_b_a_direction::n_b_a_null;
	float			initial_speed = 0;
	iPoint			starting_pos = NULLPOINT;
	float			rotation = 0.0f;
	float			die = false;
	
	j1Timer*		timer = nullptr;
};

#endif // _NAVI_BASIC_ATTACK_