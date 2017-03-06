#include "Boomerang.h"
#include "GameObject.h"
#include "j1Viewports.h"

Boomerang::Boomerang(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(20, 20), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_boomerang, 0);
	hit_box = game_object->CreateCollisionSensor(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_attack);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);

	pugi::xml_document doc;
	App->LoadXML("boomerang.xml", doc);
	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));
	App->UnloadXML(doc);

	draw_offset = restore_draw_offset = { 7, 9 };

	name = "boomerang";
}

Boomerang::~Boomerang()
{
}

bool Boomerang::Start()
{
	bool ret = true;

	range = 300;
	game_object->SetAnimation("spin");

	return ret;
}

bool Boomerang::PreUpdate()
{
	bool ret = true;



	return ret;
}

bool Boomerang::Update(float dt)
{
	bool ret = true;

	float speed = 100 * dt;

	switch (dir)
	{
	case direction::up:
		game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y - speed });
		break;
	case direction::down:
		game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y + speed });
		break;
	case direction::left:
		game_object->SetPos({ game_object->fGetPos().x - speed, game_object->fGetPos().y });
		break;
	case direction::right:
		game_object->SetPos({ game_object->fGetPos().x + speed, game_object->fGetPos().y });
		break;
	default:
		break;
	}

	return ret;
}

bool Boomerang::Draw(float dt)
{
	bool ret = true;

	App->view->LayerBlit(1, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 3, game_object->GetPos().y - draw_offset.y }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);

	return ret;
}

bool Boomerang::PostUpdate()
{
	bool ret = true;



	return ret;
}

bool Boomerang::CleanUp()
{
	bool ret = true;



	return ret;
}

void Boomerang::CleanSpell()
{
}

void Boomerang::Set(direction _dir)
{
	dir = _dir;
}
