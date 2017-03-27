#include "Snakes.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "CollisionFilters.h"
#include "j1App.h"
#include "j1Map.h"
#include "PlayerManager.h"
#include "JungleCampManager.h"
#include "Entity.h"
#include "j1Entity.h"
#include <vector>
#include "p2Log.h"
#include "MainScene.h"
#include "j1Scene.h"
#include "j1Spell.h"
#include "Spell.h"


Snakes::Snakes(iPoint pos)
{
	
}

Snakes::~Snakes()
{
	
}

bool Snakes::Start()
{
	bool ret = true;


	return ret;
}

bool Snakes::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool Snakes::Update(float dt)
{
	bool ret = true;


	return ret;
}

bool Snakes::Draw(float dt)
{
	bool ret = true;


	return ret;
}

bool Snakes::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool Snakes::CleanUp()
{
	bool ret = true;

	return ret;
}

iPoint Snakes::GetPos() const
{
	return{ 0,0 };
}

void Snakes::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
}
