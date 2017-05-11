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
#include "Functions.h"

#define GUARD_H 56
#define GUARD_W 32

#define FOLLOW_RANGE 250
#define ATTACK_RANGE 35

#define HALFMAP 81*32

#define HALF_TILE 16

Guards::Guards(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(GUARD_W, GUARD_H), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_npc, 0);

	game_object->CreateCollisionSensor(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	game_object->CreateCollision(iPoint(0, 15), 7, fixture_type::f_t_collision_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);

	initial_pos = { pos.x, pos.y };

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

	current_life = stats.life;

	name = "guards";
}

Guards::~Guards()
{
}

bool Guards::Start()
{
	bool ret = true;

	cd_timer = App->AddGameplayTimer();

	game_object->SetAnimation("guard_idle_lateral");

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
				DealDamage(((float)entity->stats.power * (float)spell->stats.damage_multiplicator) + (float)ability->damage); // Spells control their own damage mutiplicator

				spell->Effects(entity, this, ability);				
			}
			else
				DealDamage(((float)entity->stats.power * (float)ability->damage_multiplicator) + (float)ability->damage);

			is_attacked = true;
		}

		Die(entity);
	}

	if (current_life < stats.life)
	{
		is_attacked = true;
		current_life = stats.life;
	}

	CheckState();

	return ret;
}

bool Guards::Draw(float dt)
{
	bool ret = true;

	if (flip)
		App->view->LayerBlit(GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 23, game_object->GetPos().y - draw_offset.y -36 }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);
	else
		App->view->LayerBlit(GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 23, game_object->GetPos().y - draw_offset.y - 36 }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);

	if (App->debug_mode)
	{
		App->view->LayerDrawCircle(initial_pos.x, initial_pos.y, FOLLOW_RANGE, 255, 0, 0);
		App->view->LayerDrawCircle(GetPos().x, GetPos().y, ATTACK_RANGE, 255, 0, 0);
	}
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

	App->DeleteGameplayTimer(cd_timer);

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

void Guards::Die(Entity* killed_by)
{
	if (killed_by != nullptr)
	{
		if (stats.life <= 0)
		{
			App->entity->AddRupeesIfPlayer(killed_by, rupee_reward);
			App->scene->main_scene->jungleCamp_manager->KillJungleCamp(this);
		}
	}
}

bool Guards::OnRangeFollow(iPoint pos)
{
	bool ret = false;

	if (abs(DistanceFromTwoPoints(initial_pos.x, initial_pos.y, pos.x, pos.y)) <= FOLLOW_RANGE)
		ret = true;

	return ret;
}

bool Guards::OnRangeAttack(iPoint pos)
{
	bool ret = false;

	if (abs(DistanceFromTwoPoints(GetPos().x, GetPos().y, pos.x, pos.y)) <= ATTACK_RANGE)
		ret = true;

	return ret;
}

void Guards::CheckState()
{
	if (target != nullptr && target->to_delete && !LookForTarget())
	{
		target = nullptr;
		state = g_s_follow;
		move_state = gMove_ReturnToPath;
	}
	
	switch (state)
	{
	case g_s_idle:
		if (is_attacked && LookForTarget() && target != nullptr)
		{
			state = g_s_follow;
			move_state = gMove_AproachTarget;
			PathToTarget();
			is_attacked = false;
			last_target_pos = target->game_object->GetPos();
		}
		IdleRight();
		break;
	case g_s_follow:
	{
		switch (move_state)
		{
		case gMove_AproachTarget:
			if (target != nullptr && OnRangeAttack(target->game_object->GetPos()))
			{
				if(abilities.at(0)->CdCompleted())
					state = g_s_attack;
			} 
			else
			{
				if (target != nullptr && OnRangeFollow(target->game_object->GetPos()))
				{
					if (abs(DistanceFromTwoPoints(last_target_pos.x, last_target_pos.y, target->game_object->GetPos().x, target->game_object->GetPos().y)) > ATTACK_RANGE)
					{
						PathToTarget();
						last_target_pos = target->game_object->GetPos();
					}

					else if (target_path.empty())
						PathToTarget();
				
					GuardMove();
				}
				else
				{
					target = nullptr;
					state = g_s_follow;
					move_state = gMove_ReturnToPath;
					PathToInitialPos();
				}
			}
			break;
		case gMove_ReturnToPath:
			if (LookForTarget() && target != nullptr && OnRangeFollow(target->game_object->GetPos()))
			{
				move_state = gMove_AproachTarget;
				PathToTarget();
			}
			else if(!LookForTarget() && target_path.empty())
			{ 
				PathToInitialPos();
			}
			else if (target_path.empty() && !attacking)
			{
				state = g_s_idle;
			}
			else
			{
				GuardMove();
			}

			break;
		default:
			break;
		}
		break;
	}
	case g_s_attack:
		if (attacking)
		{
			if (game_object->animator->IsCurrentAnimation("guard_attack_up") 
				|| game_object->animator->IsCurrentAnimation("guard_attack_down")
				|| game_object->animator->IsCurrentAnimation("guard_attack_lateral"))
			{
				if (game_object->animator->GetCurrentAnimation()->Finished())
				{
					game_object->DeleteFixture(GetAbility(0)->fixture);

					game_object->animator->GetCurrentAnimation()->Reset();

					state = g_s_follow;
					move_state = gMove_AproachTarget;
					target_path.empty();

					draw_offset.SetToZero();

					attacking = false;
				}
			}
		}
		else
		{
			GuardAttack();
		}

		break;
	default:
		break;
	}
}

void Guards::SetTargetPath(const list<iPoint>* path)
{
	target_path.clear();

	for (list<iPoint>::const_iterator it = path->begin(); it != path->end(); it++)
		target_path.push_back(*it);
}

void Guards::PathToTarget()
{
	if (App->pathfinding->CreatePath(App->map->WorldToMap(GetPos().x, GetPos().y), App->map->WorldToMap(target->game_object->GetPos().x, target->game_object->GetPos().y)) > 0)
		SetTargetPath(App->pathfinding->GetLastPath());
}

void Guards::PathToInitialPos()
{
	if (App->pathfinding->CreatePath(App->map->WorldToMap(GetPos().x, GetPos().y), App->map->WorldToMap(initial_pos.x, initial_pos.y)) > 0)
		SetTargetPath(App->pathfinding->GetLastPath());
}

void Guards::GuardIdle()
{
	SetIdleAnim();
}

void Guards::GuardMove()
{
	if (target_path.empty())
		return;

	iPoint guard_map_pos = App->map->WorldToMap(GetPos().x, GetPos().y);

	iPoint target_map_pos = target_path.front();

	iPoint target_world_pos = App->map->MapToWorld(target_map_pos.x, target_map_pos.y);

	Move(target_world_pos.x - GetPos().x, target_world_pos.y - GetPos().y);

	if (guard_map_pos == target_map_pos)
		target_path.pop_front();
}

void Guards::GuardAttack()
{
	if (cd_timer->ReadSec()>abilities.at(0)->cd)
	{
		Attack();
		cd_timer->Start();
		attacking = true;
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
		if (OnRangeFollow((*it)->GetPos()) && GetPos().DistanceTo((*it)->GetPos()) < shortest_distance && !(*it)->to_delete)
		{
			shortest_distance = GetPos().DistanceTo((*it)->GetPos());
			target = *it;
			ret = true;
		}
	}
	
	for (std::vector<Entity*>::iterator it = players2.begin(); it != players2.end(); it++)
	{
		if (OnRangeFollow((*it)->GetPos()) && GetPos().DistanceTo((*it)->GetPos()) < shortest_distance && !(*it)->to_delete)
		{
			shortest_distance = GetPos().DistanceTo((*it)->GetPos());
			target = *it;
			ret = true;
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

