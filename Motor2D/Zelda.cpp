#include "Zelda.h"
#include "GameObject.h"
#include "j1Viewports.h"

Zelda::Zelda(iPoint pos)
{
	pugi::xml_document doc;
	App->LoadXML("zelda.xml", doc);

	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(30, 40), App->cf->CATEGORY_PAYLOAD_AREA, App->cf->MASK_PAYLOAD_AREA, pbody_type::p_t_npc, 0);

	radius = doc.child("file").child("radius").attribute("value").as_int(0);
	activation_time = doc.child("file").child("activation_time").attribute("sec").as_uint(0);

	game_object->CreateCollisionSensor(iPoint(0, 0), radius, fixture_type::f_t_hit_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);

	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));
	game_object->SetAnimation("idle_down");

	area_image = { doc.child("file").child("area").attribute("x").as_int(0),doc.child("file").child("area").attribute("y").as_int(0),doc.child("file").child("area").attribute("w").as_int(0),doc.child("file").child("area").attribute("h").as_int(0) };

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
	CheckState();

	switch (state)
	{
	case z_s_wait:
		break;
	case z_s_move_to_path:
		break;
	case z_s_idle:
		break;
	case z_s_move:
		break;
	case z_s_null:
		break;
	default:
		break;
	}

	return true;
}

bool Zelda::Draw(float dt)
{
	App->view->LayerBlit(game_object->GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 22, game_object->GetPos().y - draw_offset.y - 24 }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);
	
	if(active)
		App->view->LayerBlit(2, game_object->GetTexture(), { game_object->GetPos().x - radius, game_object->GetPos().y - radius}, area_image, 0, -1.0f, true, SDL_FLIP_NONE);

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

bool Zelda::SetTimer(j1Timer * timer)
{
	if (game_timer == nullptr)
	{
		game_timer = timer;
		return true;
	}
	return false;
}

void Zelda::SetInitialPath(std::vector<iPoint>& path)
{
	initial_path = path;
}

void Zelda::SetPath(std::vector<iPoint>& path)
{
	main_path = path;
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

void Zelda::CheckState()
{
	switch (state)
	{
	case z_s_wait:
		if (game_timer->ReadSec() > activation_time)
			state = z_s_move_to_path;
		break;
	case z_s_move_to_path:
		if (initial_path_pos == initial_path.size() - 1)
			state = z_s_idle;
		break;
	case z_s_idle:
		break;
	case z_s_move:
		break;
	case z_s_null:
		state = z_s_wait;
		break;
	default:
		break;
	}
}
