#include "TowerAttack.h"
#include "GameObject.h"
#include "j1Viewports.h"



TowerAttack::TowerAttack(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(20, 20), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_tower_attack, 0);
	hit_box = game_object->CreateCollisionSensor(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_attack);
	game_object->SetListener((j1Module*)App->entity);

	pugi::xml_document doc;
	App->LoadXML("towerattack.xml", doc);
	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));
	App->UnloadXML(doc);

	draw_offset = restore_draw_offset = { 7, 9 };

	name = "t_attack";
}

TowerAttack::~TowerAttack()
{
}

bool TowerAttack::Start()
{
	bool ret = true;

	
	game_object->SetAnimation("projectile");

	return ret;
}

bool TowerAttack::PreUpdate()
{
	bool ret = true;



	return ret;
}

bool TowerAttack::Update(float dt)
{
	bool ret = true;

	float speed = 170 * dt;

	float initial_angle = AngleFromTwoPoints(game_object->GetPos().x, game_object->GetPos().y, target->GetPos().x, target->GetPos().y);

	fPoint t;

	if (game_object->GetPos().x > target->GetPos().x)
		t = { speed * cos(DEGTORAD * initial_angle) ,speed * sin(DEGTORAD * initial_angle) };
	else
		t = { speed * cos(DEGTORAD * initial_angle) ,speed * sin(DEGTORAD * initial_angle) };

	game_object->SetPos({ game_object->GetPos().x + t.x, game_object->GetPos().y + t.y });
	
	if (reached)
		App->spell->DeleteSpell(this);

	return ret;
}

bool TowerAttack::Draw(float dt)
{
	bool ret = true;

	App->view->LayerBlit(game_object->GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 3, game_object->GetPos().y - draw_offset.y }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);

	return ret;
}

bool TowerAttack::PostUpdate()
{
	bool ret = true;

	

	return ret;
}

bool TowerAttack::CleanUp()
{
	bool ret = true;



	return ret;
}

void TowerAttack::CleanSpell()
{

}

void TowerAttack::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	if (game_object->pbody == bodyA && bodyB == target->game_object->pbody)
	{
		reached = true;
	}
		
}

void TowerAttack::SetTarget(Entity * _target)
{
	target = _target;
}

