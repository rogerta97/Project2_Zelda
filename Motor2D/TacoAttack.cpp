#include "TacoAttack.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "j1XMLLoader.h"

#define INITIAL_SPEED 150
#define ACCELERATION 100

TacoAttack::TacoAttack(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(15, 15), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_taco, 0);
	hit_box = game_object->CreateCollisionSensor(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_attack);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetListener((j1Module*)App->spell);

	stats.damage_multiplicator = 1.0f;

	pugi::xml_document doc;
	App->xml->LoadXML("tacoattack.xml", doc);
	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));

	draw_offset = restore_draw_offset = { 7, 9 };

	name = "taco_attack";
}

TacoAttack::~TacoAttack()
{
}

bool TacoAttack::Start()
{
	bool ret = true;

	game_object->SetAnimation("projectile");

	return ret;
}

bool TacoAttack::PreUpdate()
{
	bool ret = true;


	return ret;
}

bool TacoAttack::Update(float dt)
{
	bool ret = true;

	if (game_object->animator->IsCurrentAnimation("destroy"))
	{
		if (game_object->animator->GetCurrentAnimation()->Finished())
			App->spell->DeleteSpell(this);
	}
	else if (target != nullptr)
	{

		float speed = (INITIAL_SPEED + (ACCELERATION * timer.ReadSec())) * dt;

		float initial_angle = AngleFromTwoPoints(game_object->GetPos().x, game_object->GetPos().y, target->GetPos().x, target->GetPos().y);

		fPoint t;

		if (game_object->GetPos().x > target->GetPos().x)
			t = { speed * cos(DEGTORAD * initial_angle) ,speed * sin(DEGTORAD * initial_angle) };
		else
			t = { speed * cos(DEGTORAD * initial_angle) ,speed * sin(DEGTORAD * initial_angle) };

		game_object->SetPos({ game_object->GetPos().x + t.x, game_object->GetPos().y + t.y });

	}

	return ret;
}

bool TacoAttack::Draw(float dt)
{
	bool ret = true;

	App->view->LayerBlit(game_object->GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 9, game_object->GetPos().y - draw_offset.y - 7 }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);

	return ret;
}

bool TacoAttack::PostUpdate()
{
	bool ret = true;


	return ret;
}

bool TacoAttack::CleanUp()
{
	bool ret = true;


	return ret;
}

void TacoAttack::CleanSpell()
{

}

void TacoAttack::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	if (game_object != nullptr && target != nullptr && game_object->pbody == bodyA && bodyB == target->game_object->pbody && fixtureB->type == fixture_type::f_t_hit_box)
	{
		game_object->SetAnimation("destroy");
		game_object->SetCatMask(App->cf->CATEGORY_NONCOLLISIONABLE, App->cf->MASK_NONCOLLISIONABLE);
		target = nullptr;
	}

}

void TacoAttack::SetTarget(Entity * _target)
{
	target = _target;
}