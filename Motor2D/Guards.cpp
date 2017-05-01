#include "Guards.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "CollisionFilters.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "PlayerManager.h"
#include "JungleCampManager.h"
#include "Entity.h"
#include "j1Entity.h"
#include "p2Log.h"
#include "Spell.h"
#include "j1XMLLoader.h"
#include "Quest_Manager.h"

#define GUARD_H 32
#define GUARD_W 32

#define FOLLOW_RANGE 150
#define ATTACK_RANGE 60

#define HALFMAP 81*32

Guards::Guards(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(GUARD_H, GUARD_W), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_npc, 0);

	game_object->CreateCollisionSensor(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	game_object->CreateCollision(iPoint(0, 15), 7, fixture_type::f_t_collision_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);

	pugi::xml_document doc;
	App->xml->LoadXML("guards.xml", doc);
	pugi::xml_node stats_node = doc.child("file").child("stats");
	rupee_reward = stats_node.attribute("rupees").as_int();

	stats.life = stats.base_hp = stats.max_life = stats_node.attribute("hp").as_int();
	stats.base_power = stats.power = stats_node.attribute("power").as_int();
	stats.base_speed = stats.speed = stats.restore_speed = stats_node.attribute("speed").as_int();

	float dmg_mult = stats_node.child("ability1").attribute("mult").as_float();
	float cd = stats_node.child("ability1").attribute("cd").as_float();
	int bd = stats_node.child("ability1").attribute("bd").as_int();
	AddAbility(0, cd, bd, dmg_mult);

	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));

	cd_timer.Start();

	name = "guards";
}

Guards::~Guards()
{
}

bool Guards::Start()
{
	bool ret = true;

	show_life_bar = true;

	game_object->SetAnimation("guard_down");

	return ret;
}

bool Guards::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool Guards::Update(float dt)
{
	bool ret = true;

	if (to_delete)
		return true;



	return ret;
}

bool Guards::Draw(float dt)
{
	bool ret = true;

	return ret;
}

bool Guards::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool Guards::CleanUp()
{
	bool ret = true;

	return ret;
}

void Guards::OnCollEnter(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
}

iPoint Guards::GetPos() const
{
	return game_object->GetPos();
}

void Guards::MoveUp(float speed)
{
	game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y - speed });
}

void Guards::MoveDown(float speed)
{
	game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y + speed });
}

void Guards::MoveLeft(float speed)
{
	game_object->SetPos({ game_object->fGetPos().x - speed, game_object->fGetPos().y });
}

void Guards::MoveRight(float speed)
{
	game_object->SetPos({ game_object->fGetPos().x + speed, game_object->fGetPos().y });
}

void Guards::MoveUpRight(float speed)
{
	fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
	game_object->SetPos({ game_object->fGetPos().x + s.x, game_object->fGetPos().y - s.y });
}

void Guards::MoveDownRight(float speed)
{
	fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
	game_object->SetPos({ game_object->fGetPos().x + s.x, game_object->fGetPos().y + s.y });
}

void Guards::MoveUpLeft(float speed)
{
	fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
	game_object->SetPos({ game_object->fGetPos().x - s.x, game_object->fGetPos().y - s.y });
}

void Guards::MoveDownLeft(float speed)
{
	fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
	game_object->SetPos({ game_object->fGetPos().x - s.x, game_object->fGetPos().y + s.y });
}

void Guards::RunUp()
{
	game_object->SetAnimation("guard_move_up");
	flip = false;
	anim_state = run_up;
}

void Guards::RunDown()
{
	game_object->SetAnimation("guard_move_down");
	flip = false;
	anim_state = run_down;
}

void Guards::RunLeft()
{
	game_object->SetAnimation("guard_move_lateral");
	flip = true;
	anim_state = run_left;
	draw_offset.x = 8;
}

void Guards::RunRight()
{
	game_object->SetAnimation("guard_move_lateral");
	flip = false;
	anim_state = run_right;
	draw_offset.x = -8;
}

void Guards::IdleUp()
{
	game_object->SetAnimation("guard_up");
	flip = false;
	anim_state = guard_up;
}

void Guards::IdleDown()
{
	game_object->SetAnimation("guard_down");
	flip = false;
	anim_state = guard_down;
}

void Guards::IdleLeft()
{
	game_object->SetAnimation("guard_lateral");
	flip = true;
	anim_state = guard_lateral;
	draw_offset.x = 8;
}

void Guards::IdleRight()
{
	game_object->SetAnimation("guard_lateral");
	flip = false;
	anim_state = guard_lateral;
	draw_offset.x = 8;
}
