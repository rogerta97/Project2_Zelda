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
}

Link::~Link()
{
}

bool Link::Start()
{
	bool ret = true;

	player_go->SetAnimation("idle_down");

	stats.speed = 200;

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

	return ret;
}

bool Link::Draw(float dt)
{
	bool ret = true;
	
	if(flip)
		App->view->LayerBlit(2, player_go->GetTexture(), { player_go->GetPos().x - 20, player_go->GetPos().y - 23}, player_go->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);
	else
		App->view->LayerBlit(2, player_go->GetTexture(), { player_go->GetPos().x - 17, player_go->GetPos().y - 23 }, player_go->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);

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
	player_go->SetPos({ player_go->fGetPos().x, player_go->fGetPos().y - speed });
}

void Link::MoveDown(float speed)
{
	player_go->SetPos({ player_go->fGetPos().x, player_go->fGetPos().y + speed });
}

void Link::MoveLeft(float speed)
{
	player_go->SetPos({ player_go->fGetPos().x - speed, player_go->fGetPos().y });
}

void Link::MoveRight(float speed)
{
	player_go->SetPos({ player_go->fGetPos().x + speed, player_go->fGetPos().y });
}

void Link::MoveUpRight(float speed)
{
	fPoint s(speed * cos(45), speed * sin(45));
	player_go->SetPos({ player_go->fGetPos().x + s.x, player_go->fGetPos().y - s.y });
}

void Link::MoveDownRight(float speed)
{
	fPoint s(speed * cos(45), speed * sin(45));
	player_go->SetPos({ player_go->fGetPos().x + s.x, player_go->fGetPos().y + s.y });
}

void Link::MoveUpLeft(float speed)
{
	fPoint s(speed * cos(45), speed * sin(45));
	player_go->SetPos({ player_go->fGetPos().x - s.x, player_go->fGetPos().y - s.y });
}

void Link::MoveDownLeft(float speed)
{
	fPoint s(speed * cos(45), speed * sin(45));
	player_go->SetPos({ player_go->fGetPos().x - s.x, player_go->fGetPos().y + s.y });
}

void Link::RunUp()
{
	player_go->SetAnimation("run_up");
	flip = false;
}

void Link::RunDown()
{
	player_go->SetAnimation("run_down");
	flip = false;
}

void Link::RunLeft()
{
	player_go->SetAnimation("run_lateral");
	flip = true;
}

void Link::RunRight()
{
	player_go->SetAnimation("run_lateral");
	flip = false;
}

void Link::IdleUp()
{
	player_go->SetAnimation("idle_up");
	flip = false;
}

void Link::IdleDown()
{
	player_go->SetAnimation("idle_down");
	flip = false;
}

void Link::IdleLeft()
{
	player_go->SetAnimation("idle_lateral");
	flip = true;
}

void Link::IdleRight()
{
	player_go->SetAnimation("idle_lateral");
	flip = false;
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
