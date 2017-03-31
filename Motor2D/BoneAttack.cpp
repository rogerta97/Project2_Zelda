#include "BoneAttack.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "p2Log.h"
#include "Entity.h"

#define DESTRUCTION_TIME 1.8f

BoneAttack::BoneAttack(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(20, 20), App->cf->CATEGORY_ABILITIES, App->cf->MASK_ABILITIES, pbody_type::p_t_boomerang, 0);
	hit_box = game_object->CreateCollisionSensor(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_attack);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);
	game_object->pbody->body->SetBullet(true);

	pugi::xml_document doc;
	App->LoadXML("boneattack.xml", doc);
	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));
	App->UnloadXML(doc);

	draw_offset = restore_draw_offset = { 7, 9 };

	name = "bone";

	timer.Start();

	starting_pos = pos;
}

BoneAttack::~BoneAttack()
{
}

bool BoneAttack::Start()
{
	bool ret = true;

	game_object->SetAnimation("spin");

	return ret;
}

bool BoneAttack::PreUpdate()
{
	bool ret = true;



	return ret;
}

bool BoneAttack::Update(float dt)
{
	bool ret = true;

	// Speed calculations
	initial_speed = ((BOOMERANG_RANGE - (0.5 * ACCELERATION * (TIME*TIME))) / TIME);

	float speed = ((initial_speed)+(ACCELERATION * timer.ReadSec())) * dt;


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

bool BoneAttack::Draw(float dt)
{
	bool ret = true;

	App->view->LayerBlit(1, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 3, game_object->GetPos().y - draw_offset.y }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);

	return ret;
}

bool BoneAttack::PostUpdate()
{
	bool ret = true;



	return ret;
}

bool BoneAttack::CleanUp()
{
	bool ret = true;

	return ret;
}

void BoneAttack::CleanSpell()
{
}

void BoneAttack::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{

}