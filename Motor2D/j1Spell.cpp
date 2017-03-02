#include "j1Spell.h"

j1Spell::j1Spell()
{
}

j1Spell::~j1Spell()
{
}

bool j1Spell::Awake(pugi::xml_node &)
{
	bool ret = true;



	return ret;
}

bool j1Spell::Start()
{
	bool ret = true;



	return ret;
}

bool j1Spell::PreUpdate()
{
	bool ret = true;



	return ret;
}

bool j1Spell::Update(float dt)
{
	bool ret = true;



	return ret;
}

bool j1Spell::PostUpdate()
{
	bool ret = true;



	return ret;
}

bool j1Spell::CleanUp()
{
	bool ret = true;



	return ret;
}

void j1Spell::OnCollision(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
}

Spell * j1Spell::CreateSpell(spell_name entity, iPoint pos, Entity * owner)
{
	return nullptr;
}

void j1Spell::DeleteSpell(Spell * entity)
{
}

void j1Spell::ClearSpells()
{
}

void j1Spell::RemoveSpells()
{
}

