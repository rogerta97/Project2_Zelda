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

	starting_pos = pos;
}

TowerAttack::~TowerAttack()
{
}

bool TowerAttack::Start()
{
	bool ret = true;

	range = 150;
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

	float speed = 100 * dt;


	return ret;
}

bool TowerAttack::Draw(float dt)
{
	bool ret = true;

	App->view->LayerBlit(1, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 3, game_object->GetPos().y - draw_offset.y }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);

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

void TowerAttack::SetTarget(Entity * target)
{
	
}

