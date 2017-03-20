#include "Boomerang.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "p2Log.h"
#include "Entity.h"

#define ACCELERATION -1300
#define TIME 0.65f
#define DESTRUCTION_TIME 1.8f
#define SLOW_TIME 1.5f
#define SLOW_MULTIPLICATOR 0.5f
Boomerang::Boomerang(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(20, 20), App->cf->CATEGORY_SCENERY, App->cf->MASK_SCENERY, pbody_type::p_t_boomerang, 0);
	hit_box = game_object->CreateCollisionSensor(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_attack);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);
	game_object->pbody->body->SetBullet(true);

	pugi::xml_document doc;
	App->LoadXML("boomerang.xml", doc);
	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));
	App->UnloadXML(doc);

	draw_offset = restore_draw_offset = { 7, 9 };

	name = "boomerang";

	timer.Start();

	starting_pos = pos;
}

Boomerang::~Boomerang()
{
}

bool Boomerang::Start()
{
	bool ret = true;

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

	// Speed calculations
	initial_speed = ((BOOMERANG_RANGE - (0.5 * ACCELERATION * (TIME*TIME))) / TIME);

	float speed = ((initial_speed) + (ACCELERATION * timer.ReadSec())) * dt;

	// Can be taken when is returning
	if (!can_delete && speed < 0)
		can_delete = true;

	//LOG("initial speed: %f, range: %d, current speed: %f", initial_speed, range, speed);

	// Reducing speed when boomerang returns
	if (can_delete)
		speed = speed * 0.5;

	// Reduce damage and slow
	if (!can_delete)
	{
		if (DistanceFromTwoPoints(starting_pos.x, starting_pos.y, game_object->GetPos().x, game_object->GetPos().y) < BOOMERANG_RANGE * 0.5f)
		{
			stats.stun_duration = 1.0f;
			stats.damage_multiplicator = 1.3f;
		}
		else if (DistanceFromTwoPoints(starting_pos.x, starting_pos.y, game_object->GetPos().x, game_object->GetPos().y) > BOOMERANG_RANGE * 0.5f)
		{
			stats.stun_duration = 0.0f;
			stats.damage_multiplicator = 0.7f;
			stats.slow_duration = SLOW_TIME;
			stats.slow_multiplicator = SLOW_MULTIPLICATOR;
		}
	}
	else
	{
		stats.stun_duration = 0.0f;
		stats.damage_multiplicator = 0.7f;
		stats.slow_duration = 0.0f;
		stats.slow_multiplicator = 0.0f;
	}

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

	if (timer.ReadSec() > DESTRUCTION_TIME)
		App->spell->DeleteSpell(this);

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

void Boomerang::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{

}

void Boomerang::Set(direction _dir)
{
	dir = _dir;
}
