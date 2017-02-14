#include "Player.h"
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


Player::Player()
{
}

Player::~Player()
{
}

bool Player::LoadEntity()
{
	bool ret = true;

	player_go = new GameObject(iPoint(300, 300), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_player, 5);

	player_go->CreateCollision(iPoint(0, 0), 38, 80, fixture_type::f_t_null);
	player_go->CreateCollision(iPoint(0, -50), 20, fixture_type::f_t_down_ball);
	player_go->CreateCollision(iPoint(0, 50), 20, fixture_type::f_t_null);
	player_go->SetListener((j1Module*)App->entity);
	player_go->SetFixedRotation(true);

	player_go->SetTexture(App->tex->LoadTexture("spritesheet.png"));

	// Idle
	list<SDL_Rect> idle_rects;
	LoadAnimationFromXML(idle_rects, "player.xml", "idle");
	Animation* idle = new Animation("idle", idle_rects, 6.0f);
	player_go->AddAnimation(idle);

	// Idle-Walk
	list<SDL_Rect> idlewalk_rects;
	LoadAnimationFromXML(idlewalk_rects, "player.xml", "idle-walk");
	Animation* idlewalk = new Animation("idle-walk", idlewalk_rects, 8.5f);
	player_go->AddAnimation(idlewalk);
	player_go->animator->SetAnimationTransition("idle-walk", "idle", "walk");
	player_go->animator->SetAnimationTransition("idle-walk", "walk", "idle");

	// Walk
	list<SDL_Rect> walk_rects;
	LoadAnimationFromXML(walk_rects, "player.xml", "walk");
	Animation* walk = new Animation("walk", walk_rects, 7.0f);
	player_go->AddAnimation(walk);

	// Up
	list<SDL_Rect> up_rects;
	LoadAnimationFromXML(up_rects, "player.xml", "up");
	Animation* up = new Animation("up", up_rects, 3.3f, false);
	player_go->AddAnimation(up);

	// Down
	list<SDL_Rect> down_rects;
	LoadAnimationFromXML(down_rects, "player.xml", "down");
	Animation* down = new Animation("down", down_rects, 3.3f, false);
	player_go->AddAnimation(down);

	player_go->SetAnimation("down");

	last_height = player_go->fGetPos().y;

	return ret;
}

bool Player::Start()
{
	bool ret = true;



	return ret;
}

bool Player::PreUpdate()
{
	bool ret = true;



	return ret;
}

bool Player::Update(float dt)
{
	bool ret = true;

	float speed = (40 * dt);

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		player_go->SetPos({ player_go->fGetPos().x - speed, player_go->fGetPos().y });
		flip = true;
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		player_go->SetPos({ player_go->fGetPos().x + speed, player_go->fGetPos().y });
		flip = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && on_ground)
	{
		player_go->pbody->body->SetLinearVelocity(b2Vec2(player_go->pbody->body->GetLinearVelocity().x, -10));
	}

	// Up or down
	if (player_go->fGetPos().y > last_height)
		going_up = false;
	else if (player_go->fGetPos().y < last_height)
		going_up = true;

	last_height = player_go->fGetPos().y;
	// ----------

	return ret;
}

bool Player::Draw(float dt)
{
	bool ret = true;

	if (on_ground)
	{
		player_go->SetAnimation("idle");

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			player_go->SetAnimation("walk");

		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			player_go->SetAnimation("walk");
		
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			on_ground = false;
	}

	if (!on_ground)
	{
		if (going_up)
		{
			player_go->SetAnimation("up");
		}
		else if (!going_up)
			player_go->SetAnimation("down");
	}
	
	if(flip)
		App->scene->LayerBlit(2, player_go->GetTexture(), { player_go->GetPos().x - 46, player_go->GetPos().y - 25}, player_go->GetCurrentAnimationRect(dt), -1.0f, SDL_FLIP_HORIZONTAL);
	else
		App->scene->LayerBlit(2, player_go->GetTexture(), { player_go->GetPos().x - 30, player_go->GetPos().y - 25 }, player_go->GetCurrentAnimationRect(dt), -1.0f, SDL_FLIP_NONE);

	return ret;
}

bool Player::PostUpdate()
{
	bool ret = true;



	return ret;
}

bool Player::CleanUp()
{
	bool ret = true;



	return ret;
}

void Player::OnColl(PhysBody* bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	switch (bodyA->type)
	{
	case pbody_type::p_t_player:
		if (bodyB->type == pbody_type::p_t_world)
		{
			on_ground = true;
		}
		break;
	
	}
}
