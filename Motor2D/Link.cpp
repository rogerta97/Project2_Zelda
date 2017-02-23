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


Link::Link(iPoint pos)
{
	bool ret = true;

	player_go = new GameObject(iPoint(pos.x, pos.y), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_player, 0);

	player_go->CreateCollision(iPoint(0, 0), 30, 40, fixture_type::f_t_null);
	player_go->SetListener((j1Module*)App->entity);
	player_go->SetFixedRotation(true);

	pugi::xml_document doc;
	App->LoadXML("link.xml", doc);
	player_go->SetTexture(player_go->LoadAnimationsFromXML(doc, "animations"));

	draw_offset = restore_draw_offset = { 13, 26 };
}

Link::~Link()
{
}

bool Link::Start()
{
	bool ret = true;

	player_go->SetAnimation("idle_down");

	can_move = true;
	stats.speed = 200;

	cds = AbilityCds(32, 32, 32, 32, 32, 32, 32, 32);

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

	App->view->CenterCamera(camera, player_go->GetPos().x + 23, player_go->GetPos().y + 35);

	// End atacking
	if (attacking)
	{
		if (player_go->animator->IsCurrentAnimation("basic_atack_up"))
		{
			if (player_go->animator->GetCurrentAnimation()->Finished())
			{
				player_go->animator->GetCurrentAnimation()->Reset();
				attacking = false;
				can_move = true;
				draw_offset = restore_draw_offset;
			}
		}
		else if (player_go->animator->IsCurrentAnimation("basic_atack_down"))
		{
			if (player_go->animator->GetCurrentAnimation()->Finished())
			{
				player_go->animator->GetCurrentAnimation()->Reset();
				attacking = false;
				can_move = true;
			}
		}
	}

	if (!attacking && !can_move)
	{

	}

	return ret;
}

bool Link::Draw(float dt)
{
	bool ret = true;
	
	if(flip)
		App->view->LayerBlit(2, player_go->GetTexture(), { player_go->GetPos().x - draw_offset.x, player_go->GetPos().y - draw_offset.y}, player_go->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);
	else
		App->view->LayerBlit(2, player_go->GetTexture(), { player_go->GetPos().x - draw_offset.x - 3, player_go->GetPos().y - draw_offset.y}, player_go->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);

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
		player_go->SetPos({ player_go->fGetPos().x, player_go->fGetPos().y - speed });
}

void Link::MoveDown(float speed)
{
	if (can_move)
		player_go->SetPos({ player_go->fGetPos().x, player_go->fGetPos().y + speed });
}

void Link::MoveLeft(float speed)
{
	if (can_move)
		player_go->SetPos({ player_go->fGetPos().x - speed, player_go->fGetPos().y });
}

void Link::MoveRight(float speed)
{
	if (can_move)
		player_go->SetPos({ player_go->fGetPos().x + speed, player_go->fGetPos().y });
}

void Link::MoveUpRight(float speed)
{
	if (can_move)
	{
		fPoint s(speed * cos(45), speed * sin(45));
		player_go->SetPos({ player_go->fGetPos().x + s.x, player_go->fGetPos().y - s.y });
	}
}

void Link::MoveDownRight(float speed)
{
	if (can_move)
	{
		fPoint s(speed * cos(45), speed * sin(45));
		player_go->SetPos({ player_go->fGetPos().x + s.x, player_go->fGetPos().y + s.y });
	}
}

void Link::MoveUpLeft(float speed)
{
	if (can_move)
	{
		fPoint s(speed * cos(45), speed * sin(45));
		player_go->SetPos({ player_go->fGetPos().x - s.x, player_go->fGetPos().y - s.y });
	}
}

void Link::MoveDownLeft(float speed)
{
	if (can_move)
	{
		fPoint s(speed * cos(45), speed * sin(45));
		player_go->SetPos({ player_go->fGetPos().x - s.x, player_go->fGetPos().y + s.y });
	}
}

void Link::RunUp()
{
	if (can_move)
	{
		player_go->SetAnimation("run_up");
		flip = false;
	}
}

void Link::RunDown()
{
	if (can_move)
	{
		player_go->SetAnimation("run_down");
		flip = false;
	}
}

void Link::RunLeft()
{
	if (can_move)
	{
		player_go->SetAnimation("run_lateral");
		flip = true;
	}
}

void Link::RunRight()
{
	if (can_move)
	{
		player_go->SetAnimation("run_lateral");
		flip = false;
	}
}

void Link::IdleUp()
{
	if (can_move)
	{
		player_go->SetAnimation("idle_up");
		flip = false;
	}
}

void Link::IdleDown()
{
	if (can_move)
	{
		player_go->SetAnimation("idle_down");
		flip = false;
	}
}

void Link::IdleLeft()
{
	if (can_move)
	{
		player_go->SetAnimation("idle_lateral");
		flip = true;
	}
}

void Link::IdleRight()
{
	if (can_move)
	{
		player_go->SetAnimation("idle_lateral");
		flip = false;
	}
}

void Link::BasicAttackUp()
{
	player_go->SetAnimation("basic_atack_up");
	draw_offset = { draw_offset.x, 48 };
	attacking = true;
	can_move = false;
	flip = false;
}

void Link::BasicAttackDown()
{
	player_go->SetAnimation("basic_atack_down");
	attacking = true;
	can_move = false;
	flip = false;
}

void Link::BasicAttackLeft()
{
}

void Link::BasicAttackRight()
{
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
	case pbody_type::p_t_player:
		if (bodyB->type == pbody_type::p_t_world)
		{
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
	return player_go->GetPos();
}
