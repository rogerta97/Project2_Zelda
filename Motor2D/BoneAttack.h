#ifndef _BONE_ATTACK_H
#define _BONE_ATTACK_H

#include "j1App.h"
#include "j1Render.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Physics.h"
#include "Animation.h"
#include "Spell.h"
#include "j1Spell.h"

class b2Fixture;
class PhysBody;
class GameObject;
class Spell;

#define BONE_RANGE 170

class BoneAttack : public Spell
{
public:
	BoneAttack(iPoint pos);

	~BoneAttack();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool Draw(float dt);
	bool PostUpdate();
	bool CleanUp();

	void SetAngle(int angle);

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


private:

	iPoint			starting_pos = NULLPOINT;
	int				angle = 0;

	j1Timer*		timer = nullptr;

};

#endif // _BONE_ATTACK_H