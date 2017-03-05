#ifndef _BOOMERANG_
#define _BOOMERANG_

#include "j1App.h"
#include "j1Render.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Physics.h"
#include "Animation.h"
#include "Spell.h"
#include "Entity.h"

class b2Fixture;
class PhysBody;
class GameObject;

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

	virtual void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB) {};

	void Set(direction dir);

private:
	direction dir = direction::null;
	int				range = 0;

};

#endif // _BOOMERANG_