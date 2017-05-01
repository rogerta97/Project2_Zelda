#ifndef _SNAKEPOISON_H
#define _SNAKEPOISON_H

#include "j1App.h"
#include "j1Render.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Physics.h"
#include "Animation.h"
#include "Spell.h"
#include "Entity.h"
#include "j1Spell.h"
#include "Functions.h"

class b2Fixture;
class PhysBody;
class GameObject;

class SnakePoison : public Spell
{
public:
	SnakePoison(iPoint pos);

	~SnakePoison();

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

	void SetTarget(Entity* target);

private:
	bool			reached = false;
	j1Timer*		timer = nullptr;
};

#endif // _SNAKEPOISON_H