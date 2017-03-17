#ifndef __j1SPELL_H__
#define __j1SPELL_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2List.h"
#include "p2Point.h"

class PlayerManager;
class b2Fixture;
class PhysBody;
class Entity;
class Ability;

enum class pbody_type;

enum spell_name
{
	s_n_null,
	boomerang,
	t_attack,

};

class Spell;
class Entity;
class j1Spell : public j1Module
{
public:

	j1Spell();

	// Destructor
	virtual ~j1Spell();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);
	void OnCollisionEnter(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);
	void OnCollisionOut(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

	Spell* CreateSpell(spell_name entity, iPoint pos, Entity* owner);
	void DeleteSpell(Spell* entity);

	void ClearSpells();

public:
	list<Spell*> spell_list;

private:

	void RemoveSpells();


};

#endif // __j1SPELL_H__