#include "Zelda.h"
#include "GameObject.h"

Zelda::Zelda(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(30, 40), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_npc, 0);

	game_object->CreateCollision(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);


	pugi::xml_document doc;
	App->LoadXML("zelda.xml", doc);
	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));
	App->UnloadXML(doc);
}

Zelda::~Zelda()
{
}

bool Zelda::Start()
{
	return true;
}

bool Zelda::PreUpdate()
{
	return true;
}

bool Zelda::Update(float dt)
{
	return true;
}

bool Zelda::Draw(float dt)
{
	return true;
}

bool Zelda::PostUpdate()
{
	return true;
}

bool Zelda::CleanUp()
{
	return true;
}

void Zelda::MoveUp(float speed)
{
}

void Zelda::MoveDown(float speed)
{
}

void Zelda::MoveLeft(float speed)
{
}

void Zelda::MoveRight(float speed)
{
}

void Zelda::MoveUpRight(float speed)
{
}

void Zelda::MoveDownRight(float speed)
{
}

void Zelda::MoveUpLeft(float speed)
{
}

void Zelda::MoveDownLeft(float speed)
{
}
