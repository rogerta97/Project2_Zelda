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

#define GUARD_H 72
#define GUARD_W 46

#define FOLLOW_RANGE 150
#define ATTACK_RANGE 60

#define HALFMAP 81*32

#define Half_Tile 16

Guards::Guards(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(GUARD_W, GUARD_H), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_npc, 0);

	game_object->CreateCollisionSensor(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	game_object->CreateCollision(iPoint(0, 15), 7, fixture_type::f_t_collision_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);

	initialPos = { pos.x,pos.y };

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

	if (game_object->GetPos().x < HALFMAP)
	{
		game_object->SetAnimation("guard_idle_lateral");
	}
	else
	{
		game_object->SetAnimation("guard_idle_lateral");
		flip = true;
	}
	

	state = g_s_idle;

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

	speed = stats.speed*dt;

	CheckState();

	if (!stuned)
	{
		switch (state)
		{
		case g_s_idle:
			GuardIdle();
			break;
		case g_s_follow:
			GuardMove();
			break;
		case g_s_attack:
			GuardAttack();
			break;
		case g_s_reset:
			is_attacked = false;
			GuardMove();
			break;
		default:
			break;
		}
	}
	else
	{
		SetIdleAnim();
	}
		

	LifeBar(iPoint(20, 3), iPoint(-10, -25));

	Entity* entity = nullptr;
	Ability* ability = nullptr;
	Spell* spell = nullptr;

	if (GotHit(entity, ability, spell) && stats.life)
	{
		// Enemy attacks
		if (entity != nullptr && ability != nullptr && entity->GetTeam() != GetTeam())
		{
			if (spell != nullptr)
			{
				DealDamage((entity->stats.power * spell->stats.damage_multiplicator) + ability->damage); // Spells control their own damage mutiplicator

				spell->Effects(entity, ability);
				if (entity->GetPos().DistanceTo(initialPos) < FOLLOW_RANGE)
				{
					is_attacked = true;
				}
			}
			else
				DealDamage((entity->stats.power * ability->damage_multiplicator) + ability->damage);

			if (stats.life <= 0)
			{
				Event* event_die = new Event();
				event_die->type = e_t_death;
				event_die->event_data.entity = this;
				event_thrower->AddEvent(event_die);

				App->entity->AddRupeesIfPlayer(entity, rupee_reward);
				App->scene->main_scene->jungleCamp_manager->KillJungleCamp(this);
			}
		}
	}

	return ret;
}

bool Guards::Draw(float dt)
{
	bool ret = true;

	if (flip)
		App->view->LayerBlit(GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x , game_object->GetPos().y - draw_offset.y }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);
	else
		App->view->LayerBlit(GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 8, game_object->GetPos().y - draw_offset.y - 14 }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);

	if (App->debug_mode)
		App->view->LayerDrawCircle(initialPos.x, initialPos.y, FOLLOW_RANGE, 255, 0, 0);
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
	game_object->SetAnimation("guard_idle_up");
	flip = false;
	anim_state = guard_up;
}

void Guards::IdleDown()
{
	game_object->SetAnimation("guard_idle_down");
	flip = false;
	anim_state = guard_down;
}

void Guards::IdleLeft()
{
	game_object->SetAnimation("guard_idle_lateral");
	flip = true;
	anim_state = guard_left;
	draw_offset.x = 8;
}

void Guards::IdleRight()
{
	game_object->SetAnimation("guard_idle_lateral");
	flip = false;
	anim_state = guard_right;
	draw_offset.x = 8;
}

void Guards::CheckState()
{
	if (target != nullptr)
	{
		if (target->to_delete == true)
		{
			target = nullptr;
		}
	}
	switch (state)
	{
	case g_s_idle:
		if (is_attacked && LookForTarget())
				PathToTarget();
		break;
	case g_s_follow:
	{
		switch (move_state)
		{
		case gMove_Idle:
			if (is_attacked && LookForTarget())
				PathToTarget();
			break;
		case gMove_AproachTarget:
			if (target == nullptr)
			{
				target_path_index = 0;
				target = nullptr;
				move_state = gMove_ReturnToPath;
				state = g_s_reset;
				PathToInitialPos();
			}
			else
			{
				if (GetPos().DistanceTo(target->GetPos()) < ATTACK_RANGE - ATTACK_RANGE / 4)
					state = g_s_attack;
				else
				{
					if (initialPos.DistanceTo(target->GetPos()) < FOLLOW_RANGE)
					{
						if (target != nullptr && App->map->WorldToMap(target->GetPos().x, target->GetPos().y) != *target_path.end())
						{
							PathToTarget();
						}
					}
					else
					{
						target_path_index = 0;
						target = nullptr;
						move_state = gMove_ReturnToPath;
						state = g_s_reset;
						PathToInitialPos();
					}
				}
			}
			break;
		case gMove_ReturnToPath:
			if (is_attacked && LookForTarget())
			{
				PathToTarget();
				move_state = gMove_AproachTarget;
				state = g_s_follow;
			}			
			break;
		default:
			break;
		}
		break;
	}
	case g_s_attack:
		if (target == nullptr)
		{
			target_path_index = 0;
			move_state = gMove_ReturnToPath;
			state = g_s_reset;
			PathToInitialPos();
			break;
		}
		if (game_object->animator->IsCurrentAnimation("guard_attack_up") || game_object->animator->IsCurrentAnimation("guard_attack_down")
			|| game_object->animator->IsCurrentAnimation("guard_attack_lateral"))
		{
			if (game_object->animator->GetCurrentAnimation()->Finished())
			{
				if (initialPos.DistanceTo(target->GetPos()) > FOLLOW_RANGE)
				{
					target_path_index = 0;
					target = nullptr;
					move_state = gMove_ReturnToPath;
					state = g_s_reset;
					PathToInitialPos();
				}
				if (abilities.at(0)->fixture != nullptr)
				{
					game_object->DeleteFixture(abilities.at(0)->fixture);
					abilities.at(0)->fixture = nullptr;
				}

				if (GetPos().DistanceTo(target->GetPos()) > ATTACK_RANGE)
				{
					state = g_s_follow;
					move_state = gMove_AproachTarget;
					PathToTarget();
				}
				game_object->animator->GetCurrentAnimation()->Reset();
				draw_offset.SetToZero();
				SetIdleAnim();
			}
		}
		else if (target->to_delete == true)
		{
			state = g_s_reset;
			move_state = gMove_ReturnToPath;
			PathToInitialPos();
		}
		break;
	case g_s_reset:
		if (is_attacked && LookForTarget())
		{
			PathToTarget();
			move_state = gMove_AproachTarget;
			state = g_s_follow;
		}
		else if (GetPos().DistanceTo(initialPos) == 0)
		{
			move_state = gMove_Idle;
			state = g_s_idle;
			SetIdleAnim();
		}
		break;
	default:
		break;
	}

}

void Guards::SetTargetPath(const std::list<iPoint>* path)
{
	for (std::list<iPoint>::const_iterator it = path->begin(); it != path->end(); it++)
	{
		target_path.push_back(*it);
	}
}

void Guards::PathToTarget()
{
	if (App->pathfinding->CreatePath(App->map->WorldToMap(GetPos().x, GetPos().y), App->map->WorldToMap(target->GetPos().x, target->GetPos().y)) > 0)
	{
		target_path.clear();
		SetTargetPath(App->pathfinding->GetLastPath());
		target_path_index = 0;
		state = g_s_follow;
		move_state = gMove_AproachTarget;
	}
	else
	{
		PathToInitialPos();
	}
}

void Guards::PathToInitialPos()
{
	if (App->pathfinding->CreatePath(App->map->WorldToMap(GetPos().x, GetPos().y), App->map->WorldToMap(initialPos.x,initialPos.y)) > 0)
	{
		target_path.clear();
		SetTargetPath(App->pathfinding->GetLastPath());
		target_path_index = 0;
		state = g_s_reset;
		move_state = gMove_ReturnToPath;
	}
	else
	{
		move_state = gMove_Idle;
	}
		
}

void Guards::GuardIdle()
{
	CheckState();

	SetIdleAnim();
}

void Guards::GuardMove()
{
	CheckState();
	draw_offset.SetToZero();

	iPoint map_pos = App->map->WorldToMap(GetPos().x, GetPos().y);

	switch (move_state)
	{
	case gMove_Idle:
	{
		if (target_path_index < target_path.size() - 1)
		{
			if (map_pos == target_path.at(target_path_index))
				target_path_index++;
		}
		else
		{
			state = g_s_idle;
			break;
		}

		iPoint target_pos = App->map->MapToWorld(target_path.at(target_path_index).x, target_path.at(target_path_index).y);
		target_pos.y += Half_Tile;
		target_pos.x += Half_Tile;

		Move(target_pos.x - GetPos().x, target_pos.y - GetPos().y);

		break;
	}
	case gMove_AproachTarget:
	{
		if (target_path_index < target_path.size() - 1)
		{
			if (map_pos == target_path.at(target_path_index))
				target_path_index++;
		}
		else
		{
			move_state = gMove_Idle;
			break;
		}

		iPoint target_pos = App->map->MapToWorld(target_path.at(target_path_index).x, target_path.at(target_path_index).y);
		target_pos.y += Half_Tile;
		target_pos.x += Half_Tile;

		Move(target_pos.x - GetPos().x, target_pos.y - GetPos().y);

		break;
	}
	case gMove_ReturnToPath:
	{
		if (target_path_index < target_path.size() - 1)
		{
			if (map_pos == target_path.at(target_path_index))
				target_path_index++;
		}
		else {
			move_state = gMove_Idle;
			break;
		}

		iPoint target_pos = App->map->MapToWorld(target_path.at(target_path_index).x, target_path.at(target_path_index).y);
		target_pos.y += Half_Tile;
		target_pos.x += Half_Tile;

		Move(target_pos.x - GetPos().x, target_pos.y - GetPos().y);

		break;
	}
	default:
		break;
	}
}

void Guards::GuardAttack()
{
	CheckState();

	if (cd_timer.ReadSec()>abilities.at(0)->cd)
	{
		Attack();
		cd_timer.Start();
	}
}

bool Guards::LookForTarget()
{
	bool ret = false;

	int shortest_distance = FOLLOW_RANGE;

	std::vector<Entity*> players1;
	std::vector<Entity*> players2;

	players1 = App->scene->main_scene->player_manager->GetTeamPlayers(1);
	players2 = App->scene->main_scene->player_manager->GetTeamPlayers(2);

	for (std::vector<Entity*>::iterator it = players1.begin(); it != players1.end(); it++)
	{
		if (GetPos().DistanceTo((*it)->GetPos()) < FOLLOW_RANGE && GetPos().DistanceTo((*it)->GetPos()) < shortest_distance)
		{
			shortest_distance = GetPos().DistanceTo((*it)->GetPos());
			target = *it;
			ret = true;
			break;
		}
	}
	
	if (ret == false)
	{
		for (std::vector<Entity*>::iterator it = players2.begin(); it != players2.end(); it++)
		{
			if (GetPos().DistanceTo((*it)->GetPos()) < FOLLOW_RANGE && GetPos().DistanceTo((*it)->GetPos()) < shortest_distance)
			{
				shortest_distance = GetPos().DistanceTo((*it)->GetPos());
				target = *it;
				ret = true;
				break;
			}
		}
	}

	return ret;
}

void Guards::Move(int delta_x, int delta_y)
{
	if (delta_x > 0)
	{
		if (delta_y > 0)
		{
			MoveDownRight(speed);
			RunRight();
		}
		else if (delta_y < 0)
		{
			MoveUpRight(speed);
			RunRight();
		}
		else {
			MoveRight(speed);
			RunRight();
		}
	}
	else if (delta_x < 0)
	{
		if (delta_y > 0)
		{
			MoveDownLeft(speed);
			RunLeft();
		}
		else if (delta_y < 0)
		{
			MoveUpLeft(speed);
			RunLeft();
		}
		else {
			MoveLeft(speed);
			RunLeft();
		}
	}
	else
	{
		if (delta_y > 0)
		{
			MoveDown(speed);
			RunDown();
		}
		else if (delta_y < 0)
		{
			MoveUp(speed);
			RunUp();
		}
	}
}

void Guards::BasicAttackUp()
{
	game_object->animator->SetAnimation("guard_attack_up");
	flip = false;
	anim_state = guard_attack_up;
	GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(10, -25), 5, 10, fixture_type::f_t_attack);
}

void Guards::BasicAttackDown()
{
	game_object->animator->SetAnimation("guard_attack_down");
	flip = false;
	anim_state = guard_attack_down;
	GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(-10, 25), 5, 10, fixture_type::f_t_attack);
	draw_offset.y = 10;
}

void Guards::BasicAttackLeft()
{
	game_object->animator->SetAnimation("guard_attack_lateral");
	flip = true;
	anim_state = guard_attack_left;
	GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(-23, 11), 15, 5, fixture_type::f_t_attack);
	draw_offset.x = 8;
}

void Guards::BasicAttackRight()
{
	game_object->animator->SetAnimation("guard_attack_lateral");
	flip = false;
	anim_state = guard_attack_right;
	GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(23, 11), 15, 5, fixture_type::f_t_attack);
	draw_offset.x = -8;
}

void Guards::Attack()
{
	switch (anim_state)
	{
	case run_up:
	case guard_up:
	case guard_attack_up:
		BasicAttackUp();
		break;
	case run_left:
	case guard_left:
	case guard_attack_left:
		BasicAttackLeft();
		break;
	case run_down:
	case guard_down:
	case guard_attack_down:
		BasicAttackDown();
		break;
	case run_right:
	case guard_right:
	case guard_attack_right:
		BasicAttackRight();
		break;
	default:
		break;
	}
}

void Guards::SetIdleAnim()
{
	switch (anim_state)
	{
	case run_up:
	case guard_up:
	case guard_attack_up:
		IdleUp();
		break;
	case run_left:
	case guard_left:
	case guard_attack_left:
		IdleLeft();
		break;
	case run_down:
	case guard_down:
	case guard_attack_down:
		IdleDown();
		break;
	case run_right:
	case guard_right:
	case guard_attack_right:
		IdleRight();
		break;
	default:
		break;
	}
}
