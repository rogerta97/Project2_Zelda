#include "Link.h"
#include "GameObject.h"
#include "Scene.h"
#include "j1App.h"
#include "j1Input.h"
#include "CollisionFilters.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "Functions.h"
#include "j1Textures.h"
#include "p2Log.h"
#include "j1Viewports.h"
#include "j1Entity.h"


Link::Link(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_link, 0);

	game_object->CreateCollision(iPoint(0, 0), 30, 40, fixture_type::f_t_null);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);

	pugi::xml_document doc;
	App->LoadXML("link.xml", doc);
	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));

	draw_offset = restore_draw_offset = { 16, 26 };
}

Link::~Link()
{
}

bool Link::Start()
{
	bool ret = true;

	game_object->SetAnimation("idle_down");

	can_move = true;
	stats.speed = 200;

	cds = AbilityCds(69, 69, 69, 69, 69, 69, 69, 69);

	return ret;
}

bool Link::PreUpdate()
{
	bool ret = true;



	return ret;
}

bool Link::Update(float dt)
{
	bool ret = true;

	App->view->CenterCamera(camera, game_object->GetPos().x + 23, game_object->GetPos().y + 35);

	return ret;
}

bool Link::Draw(float dt)
{
	bool ret = true;
	
	if(flip)
		App->view->LayerBlit(2, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 3, game_object->GetPos().y - draw_offset.y}, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);
	else
		App->view->LayerBlit(2, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x, game_object->GetPos().y - draw_offset.y}, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);


	// End atacking (It's down the blit because of a reason)
	if (attacking)
	{
		if (game_object->animator->IsCurrentAnimation("basic_attack_up"))
		{
			if (game_object->animator->GetCurrentAnimation()->Finished())
			{
				game_object->animator->GetCurrentAnimation()->Reset();
				attacking = false;
				can_move = true;
				draw_offset = restore_draw_offset;
			}
		}
		else if (game_object->animator->IsCurrentAnimation("basic_attack_down"))
		{
			if (game_object->animator->GetCurrentAnimation()->Finished())
			{
				game_object->animator->GetCurrentAnimation()->Reset();
				attacking = false;
				can_move = true;
			}
		}
		else if (game_object->animator->IsCurrentAnimation("basic_attack_lateral"))
		{
			if (game_object->animator->GetCurrentAnimation()->Finished())
			{
				game_object->animator->GetCurrentAnimation()->Reset();
				attacking = false;
				can_move = true;
				draw_offset = restore_draw_offset;
			}
		}
	}

	return ret;
}

bool Link::PostUpdate()
{
	bool ret = true;



	return ret;
}

bool Link::CleanUp()
{
	bool ret = true;



	return ret;
}

void Link::MoveUp(float speed)
{
	if(can_move)
		game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y - speed });
}

void Link::MoveDown(float speed)
{
	if (can_move)
		game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y + speed });
}

void Link::MoveLeft(float speed)
{
	if (can_move)
		game_object->SetPos({ game_object->fGetPos().x - speed, game_object->fGetPos().y });
}

void Link::MoveRight(float speed)
{
	if (can_move)
		game_object->SetPos({ game_object->fGetPos().x + speed, game_object->fGetPos().y });
}

void Link::MoveUpRight(float speed)
{
	if (can_move)
	{
		fPoint s(speed * cos(45), speed * sin(45));
		game_object->SetPos({ game_object->fGetPos().x + s.x, game_object->fGetPos().y - s.y });
	}
}

void Link::MoveDownRight(float speed)
{
	if (can_move)
	{
		fPoint s(speed * cos(45), speed * sin(45));
		game_object->SetPos({ game_object->fGetPos().x + s.x, game_object->fGetPos().y + s.y });
	}
}

void Link::MoveUpLeft(float speed)
{
	if (can_move)
	{
		fPoint s(speed * cos(45), speed * sin(45));
		game_object->SetPos({ game_object->fGetPos().x - s.x, game_object->fGetPos().y - s.y });
	}
}

void Link::MoveDownLeft(float speed)
{
	if (can_move)
	{
		fPoint s(speed * cos(45), speed * sin(45));
		game_object->SetPos({ game_object->fGetPos().x - s.x, game_object->fGetPos().y + s.y });
	}
}

void Link::RunUp()
{
	if (can_move)
	{
		game_object->SetAnimation("run_up");
		flip = false;
	}
}

void Link::RunDown()
{
	if (can_move)
	{
		game_object->SetAnimation("run_down");
		flip = false;
	}
}

void Link::RunLeft()
{
	if (can_move)
	{
		game_object->SetAnimation("run_lateral");
		flip = true;
	}
}

void Link::RunRight()
{
	if (can_move)
	{
		game_object->SetAnimation("run_lateral");
		flip = false;
	}
}

void Link::IdleUp()
{
	if (can_move)
	{
		game_object->SetAnimation("idle_up");
		flip = false;
	}
}

void Link::IdleDown()
{
	if (can_move)
	{
		game_object->SetAnimation("idle_down");
		flip = false;
	}
}

void Link::IdleLeft()
{
	if (can_move)
	{
		game_object->SetAnimation("idle_lateral");
		flip = true;
	}
}

void Link::IdleRight()
{
	if (can_move)
	{
		game_object->SetAnimation("idle_lateral");
		flip = false;
	}
}

void Link::BasicAttackUp()
{
	game_object->SetAnimation("basic_attack_up");
	draw_offset = { draw_offset.x, 48 };
	attacking = true;
	can_move = false;
	flip = false;
}

void Link::BasicAttackDown()
{
	game_object->SetAnimation("basic_attack_down");
	attacking = true;
	can_move = false;
	flip = false;
}

void Link::BasicAttackLeft()
{
	game_object->SetAnimation("basic_attack_lateral");
	draw_offset = { 26, draw_offset.y };
	attacking = true;
	can_move = false;
	flip = true;
}

void Link::BasicAttackRight()
{
	game_object->SetAnimation("basic_attack_lateral");
	attacking = true;
	can_move = false;
	flip = false;
}


void Link::Ability1()
{
}

void Link::Ability2()
{
}

void Link::Ability3()
{
}

void Link::OnColl(PhysBody* bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	switch (bodyA->type)
	{
	case pbody_type::p_t_link:
		if (fixtureB->type == fixture_type::f_t_atack)
		{
			hit_by = App->entity->FindEntityByBodyType(bodyB->type);
			hit = true;
		}
		break;
	
	}
}

void Link::SetCamera(int id)
{
	if (id > 0 && id < 5)
	{
		camera = id;
	}
}

iPoint Link::GetPos() const
{
	return game_object->GetPos();
}
